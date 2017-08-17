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
