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
#include <QMutex>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(raven)

class QNetworkAccessManager;
class QNetworkReply;

class Raven : public QObject {
    Q_OBJECT

    bool m_initialized;

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

    void parseDsn(const QString& dsn);

    void save(const QString& uuid, const QByteArray &message);
    void send(const QJsonObject &message);

    void _capture(const RavenMessage& message);
    void _sendAllPending();

private slots:
    void requestFinished(QNetworkReply* reply);
    void sslErrors(QNetworkReply* reply, const QList<QSslError>& errors);

public:
    Raven(const QString& dsn, QObject* parent = 0);
    ~Raven();
    RavenMessage operator()(RavenMessage::Level level, const QString &culprit);

    bool isInitialized() const;

    static RavenTag tag(const QString& name, const QString& value);

    Raven& operator<<(const RavenTag& tag);

signals:
    void eventSent(const QString& uuid) const;       //!< Signal emitted on reply from server about given uuid
    void capture(const RavenMessage& message) const; //!< Signal emitted on given message captured
    void sendAllPending() const;                     //!< Signal should be emitted whenever ready to send all pending messages
};
