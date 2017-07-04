#pragma once

#include <QString>
#include <QJsonObject>
#include <QPair>
#include <QMetaType>

#include <exception>

#define RAVEN_HERE RavenMessage::locationInfo(__FILE__, __FUNCTION__, __LINE__)

#define RAVEN_DEBUG RavenMessage::Debug, RAVEN_HERE
#define RAVEN_INFO RavenMessage::Info, RAVEN_HERE
#define RAVEN_WARNING RavenMessage::Warning, RAVEN_HERE
#define RAVEN_ERROR RavenMessage::Error, RAVEN_HERE
#define RAVEN_FATAL RavenMessage::Fatal, RAVEN_HERE

class Raven;

using RavenTag = QPair<QString, QString>;

class RavenMessage {
    QString m_logger;
    QString m_platform;
    QString m_sdk;
    QString m_device;
    QJsonObject m_body;
    QJsonObject m_tags;

    Raven* m_instance;

    friend class Raven;

    friend RavenMessage& capture(RavenMessage& message);

public:
    enum Level { Fatal, Error, Warning, Info, Debug };

    RavenMessage& operator<<(const QString& message);
    RavenMessage& operator<<(const std::exception& exc);
    RavenMessage& operator<<(const RavenTag& tag);
    RavenMessage& operator<<(RavenMessage& (*pf)(RavenMessage&));

    static QString locationInfo(const char* file, const char* func, int line);
    static RavenMessage& send(RavenMessage& message);
};

Q_DECLARE_METATYPE(RavenMessage)
