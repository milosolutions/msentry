/*******************************************************************************
Copyright (C) 2017 Milo Solutions
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

#pragma once

#include "ravendefs.h"

#include <QMutex>
#include <QMap>
#include <QList>
#include <QByteArray>
#include <QString>
#include <QJsonObject>
#include <QObject>
#include <QSslError>
#include <QNetworkRequest>
#include <QMutex>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(raven)

class QNetworkAccessManager;
class QNetworkReply;

class Raven : public QObject {
    Q_OBJECT

public:
    Raven(const QString& dsn, QObject* parent = nullptr);
    ~Raven();
    RavenMessage operator()(RavenMessage::Level level, const QString &culprit);

    bool isInitialized() const;

    static RavenTag tag(const QString& name, const QString& value);

    Raven& operator<<(const RavenTag& tag);

signals:
    void eventSent(const QString& uuid) const;       //!< Signal emitted on reply from server about given uuid
    void capture(const RavenMessage& message) const; //!< Signal emitted on given message captured
    void sendAllPending() const;                     //!< Signal should be emitted whenever ready to send all pending messages

private slots:
    void requestFinished(QNetworkReply* reply);
    void sslErrors(QNetworkReply* reply, const QList<QSslError>& errors);
    void onCapture(const RavenMessage& message);
    void onSendAllPending();

private:
    void parseDsn(const QString& dsn);
    void save(const QString& uuid, const QByteArray &message);
    void send(const QJsonObject &message);

    bool m_initialized = false;

    QNetworkAccessManager* m_networkAccessManager;

    QString m_protocol;
    QString m_publicKey;
    QString m_secretKey;
    QString m_host;
    QString m_path;
    QString m_projectId;

    QJsonObject m_eventTemplate;
    QJsonObject m_tagsTemplate;

    QMap<QString, QByteArray> m_pendingRequest;
    QMutex m_pendingMutex;

    /*!
     * \brief Network request handle to store unique Id of the log message
     */
    const QNetworkRequest::Attribute RavenUuidAttribute
        = static_cast<QNetworkRequest::Attribute>(QNetworkRequest::User + 19);


    /*!
     * \brief Validation attributes. Internally required by onCapture method
     */
    const QList<QString> m_requiredAttributes = {
        "event_id", "message", "timestamp", "level", "logger", "platform",
        /* "sdk", "device" */ };
};
