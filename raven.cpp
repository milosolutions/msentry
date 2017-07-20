/*******************************************************************************
Copyright (C) 2016 Milo Solutions
Contact: https://www.milosolutions.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*******************************************************************************/

#include "raven.h"

#include <QHostInfo>
#include <QUrl>
#include <QDir>
#include <QDirIterator>
#include <QUuid>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSysInfo>
#include <QStandardPaths>
#include <QCoreApplication>

#include <QDebug>

#include <time.h> // for time(NULL)

Q_LOGGING_CATEGORY(raven, "core.sentry")

/*!
 * Client implementation information
 */
#define RAVEN_CLIENT_NAME QString("QRaven")
#define RAVEN_CLIENT_VERSION QString("0.1")

/*!
  * \class MRaven
  * \brief Main MRaven class, responsible for managing communication between
  * the application and Sentry server.
  *
  * Often used as a singleton. Contains sending queue and maintains connectivity.
  */

/*!
 * \brief Network request handle to store unique Id of the log message
 */
const static QNetworkRequest::Attribute RavenUuidAttribute
    = static_cast<QNetworkRequest::Attribute>(QNetworkRequest::User + 19);

/*!
 * \brief Creates raven object
 * \param dsn Data Source Name, Url connection point to sentry server. e.g. http://user:name@example.com
 */
MRaven::MRaven(const QString& dsn, QObject* parent)
    : QObject(parent)
    , m_initialized(false)
    , m_networkAccessManager(new QNetworkAccessManager(this))
{
    m_eventTemplate["server_name"] = QHostInfo::localHostName();
    m_eventTemplate["logger"] = RAVEN_CLIENT_NAME;
    m_eventTemplate["platform"] = "c";
    m_eventTemplate["release"] = QCoreApplication::applicationVersion();
    m_tagsTemplate["os_type"] = QSysInfo::productType();
    m_tagsTemplate["os_version"] = QSysInfo::productVersion();
    parseDsn(dsn);
    connect(m_networkAccessManager, &QNetworkAccessManager::finished, this,
        &MRaven::requestFinished);
    connect(m_networkAccessManager, &QNetworkAccessManager::sslErrors, this,
        &MRaven::sslErrors);
    connect(
        this, &MRaven::capture, this, &MRaven::_capture, Qt::QueuedConnection);
    connect(this, &MRaven::sendAllPending, this, &MRaven::_sendAllPending,
        Qt::QueuedConnection);
}

MRaven::~MRaven()
{
    QEventLoop loop;
    connect(this, &MRaven::eventSent, &loop, &QEventLoop::quit);
    while (!m_pendingRequest.isEmpty()) {
        loop.exec(QEventLoop::ExcludeUserInputEvents);
    }
}

/*!
 * \brief Check the DSN and initialize object if DSN is valid.
 * \param dsn Data Source Name
 */
void MRaven::parseDsn(const QString& dsn)
{
    if (dsn.isEmpty()) {
        qCWarning(raven) << "DSN is empty, client disabled";
        return;
    }

    QUrl url(dsn);
    if (!url.isValid()) {
        qCWarning(raven) << "DSN is not valid, client disabled";
        return;
    }

    m_protocol = url.scheme();
    m_publicKey = url.userName();
    m_secretKey = url.password();
    m_host = url.host();
    m_path = url.path();
    m_projectId = url.fileName();

    int i = m_path.lastIndexOf('/');
    if (i >= 0)
        m_path = m_path.left(i);

    int port = url.port(80);
    if (port != 80)
        m_host.append(":").append(QString::number(port));

    qCDebug(raven) << "Raven client is ready";

    m_initialized = true;
}

/*!
 * \brief Translate log level to string
 * \param level See Level enum
 * \return String representation of the level param
 */
// TODO: use Q_ENUM automatic stringification
QString levelString(MRavenMessage::Level level)
{
    switch (level) {
    case MRavenMessage::Fatal:
        return "fatal";
    case MRavenMessage::Error:
        return "error";
    case MRavenMessage::Warning:
        return "warning";
    case MRavenMessage::Info:
        return "info";
    case MRavenMessage::Debug:
        return "debug";
    }
    return "debug";
}

/*!
 * \brief Creates internal message representaion of the log message
 * \param level Log level
 * \param culprit Log message
 * \return Log message object
 */
MRavenMessage MRaven::operator()(MRavenMessage::Level level, const QString &culprit)
{
    MRavenMessage event;
    event.m_body = QJsonObject(m_eventTemplate);
    event.m_tags = QJsonObject(m_tagsTemplate);
    event.m_body["event_id"]
        = QUuid::createUuid().toString().remove('{').remove('}').remove('-');
    event.m_body["level"] = levelString(level);
    event.m_body["timestamp"]
        = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
    event.m_body["culprit"] = culprit;
    event.m_instance = this;
    return event;
}

/*!
 * \brief Add/replace tags
 * \param tag Keypair tag/value
 * \return Self
 */
MRaven& MRaven::operator<<(const MRavenTag& tag)
{
    m_tagsTemplate[tag.first] = tag.second;
    return *this;
}

/*!
 * \brief Returns initialization state
 * \return True if object has successfully initialized
 */
bool MRaven::isInitialized() const { return m_initialized; }

/*!
 * \brief Validation attributes. Internally required by _capture method
 */
static const QList<QString> _requiredAttributes
    = { "event_id", "message", "timestamp", "level", "logger", "platform",
        /* "sdk", "device" */ };

/*!
 * \brief Prepare message for sending over network
 * \param message see RavenMessage
 */
void MRaven::_capture(const MRavenMessage& message)
{
    if (!isInitialized())
        return;

    QJsonObject body = QJsonObject(message.m_body);
    body["tags"] = message.m_tags;

    for (const auto& attributeName : _requiredAttributes) {
        Q_ASSERT(body.contains(attributeName));
    }

    send(body);
}

/*!
 * \brief Sends given message to the server
 * \param message Message to send
 */
void MRaven::send(const QJsonObject& message)
{
    const QString clientInfo(QString("%1/%2").arg(RAVEN_CLIENT_NAME, RAVEN_CLIENT_VERSION));
    const QString authInfo(QString("Sentry sentry_version=7,"
                               "sentry_client=%1,"
                               "sentry_timestamp=%2,"
                               "sentry_key=%3,"
                               "sentry_secret=%4")
                           .arg(clientInfo, QString::number(time(NULL)),
                               m_publicKey, m_secretKey));
    const QString url = QString("%1://%2%3/api/%4/store/")
                      .arg(m_protocol, m_host, m_path, m_projectId);

    QString uuid(message["event_id"].toString());

#ifdef SENTRY_PRINT
    qCDebug(raven) << "url=" << url << ",uuid=" << uuid;
#endif

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::UserAgentHeader, clientInfo);
    request.setAttribute(RavenUuidAttribute, uuid);
    request.setRawHeader("X-Sentry-Auth", authInfo.toUtf8());

    const QByteArray body(QJsonDocument(message).toJson(QJsonDocument::Indented));

#ifdef SENTRY_PRINT
    qCDebug(raven) << body;
#endif

    {
        QMutexLocker lk(&m_pendingMutex);
        m_pendingRequest[uuid] = body;
    }

    m_networkAccessManager->post(request, body);
}

/*!
 * \brief Network reply handler
 * \param reply
 */
void MRaven::requestFinished(QNetworkReply* reply)
{
    QUrl redirectUrl
        = reply->attribute(QNetworkRequest::RedirectionTargetAttribute)
              .toUrl();
    QString uuid = reply->request().attribute(RavenUuidAttribute).toString();
    QByteArray body = m_pendingRequest[uuid];
    if (!redirectUrl.isEmpty() && redirectUrl != reply->url()) {
        QNetworkRequest request = reply->request();
        request.setUrl(redirectUrl);
        m_networkAccessManager->post(request, body);
    }
    else {
        if (reply->error() == QNetworkReply::NoError) {
            qCDebug(raven) << "Event sent " << reply->readAll();
        }
        else {
            qCDebug(raven) << "Failed to send message to sentry: " << reply->error();
            qCDebug(raven) << "Sentry answer: " << reply->readAll();
            save(uuid, body);
        }
        {
            QMutexLocker lk(&m_pendingMutex);
            m_pendingRequest.remove(uuid);
        }
        emit eventSent(uuid);
    }
    reply->deleteLater();
}

/*!
 * \brief Method to ignore all SSL errors
 * \param reply Unused
 * \param errors Unused
 */
void MRaven::sslErrors(QNetworkReply* reply, const QList<QSslError>& errors)
{
    Q_UNUSED(reply)
    Q_UNUSED(errors)
    reply->ignoreSslErrors();
}

/*!
 * \brief Store a specified message to predefined location with uuid filename
 * \param uuid
 * \param message
 */
void MRaven::save(const QString& uuid, const QByteArray& message)
{
    QString messageDir = QStandardPaths::writableLocation(
        QStandardPaths::AppLocalDataLocation);
    messageDir = QDir::cleanPath(messageDir + QDir::separator() + "messages");

    const QString messageFile(QDir::cleanPath(messageDir + QDir::separator() + uuid));
    QFile file(messageFile);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(message);
    }
    else {
        qCWarning(raven) << "Could not save message, it will be discarded";
    }
}

/*!
 * \brief Make MRavenTag from specified key/value
 * \param name The key
 * \param value The value
 * \return See MRavenTag
 */
MRavenTag MRaven::tag(const QString& name, const QString& value)
{
    return MRavenTag(name, value);
}

/*!
 * \brief Send all (pending) messages stored locally to the server
 */
void MRaven::_sendAllPending()
{
    QString messageDir = QStandardPaths::writableLocation(
        QStandardPaths::AppLocalDataLocation);
    messageDir = QDir::cleanPath(messageDir + QDir::separator() + "messages");
    QDirIterator it(messageDir, QDir::Files);
    while (it.hasNext()) {
        auto messageFile = it.next();
        QFile file(messageFile);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray body = file.readAll();
            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(body, &error);
            if (error.error == QJsonParseError::NoError) {
                QJsonObject object = doc.object();
                send(object);
            }
            else {
                qCDebug(raven) << "Could not parse message " << messageFile << ": "
                         << error.errorString();
            }
        }
        else {
            qCDebug(raven) << "Could not open file " << messageFile;
        }
    }
}
