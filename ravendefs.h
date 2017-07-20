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

#pragma once

#include <QString>
#include <QJsonObject>
#include <QPair>
#include <QMetaType>

#include <exception>

#define RAVEN_HERE MRavenMessage::locationInfo(__FILE__, __FUNCTION__, __LINE__)

#define RAVEN_DEBUG MRavenMessage::Debug, RAVEN_HERE
#define RAVEN_INFO MRavenMessage::Info, RAVEN_HERE
#define RAVEN_WARNING MRavenMessage::Warning, RAVEN_HERE
#define RAVEN_ERROR MRavenMessage::Error, RAVEN_HERE
#define RAVEN_FATAL MRavenMessage::Fatal, RAVEN_HERE

class MRaven;

using MRavenTag = QPair<QString, QString>;

class MRavenMessage {
    QString m_logger;
    QString m_platform;
    QString m_sdk;
    QString m_device;
    QJsonObject m_body;
    QJsonObject m_tags;

    MRaven* m_instance;

    friend class MRaven;

    friend MRavenMessage& capture(MRavenMessage& message);

public:
    enum Level { Fatal, Error, Warning, Info, Debug };

    MRavenMessage& operator<<(const QString& message);
    MRavenMessage& operator<<(const std::exception& exc);
    MRavenMessage& operator<<(const MRavenTag& tag);
    MRavenMessage& operator<<(MRavenMessage& (*pf)(MRavenMessage&));

    static QString locationInfo(const char* file, const char* func, int line);
    static MRavenMessage& send(MRavenMessage& message);
};

Q_DECLARE_METATYPE(MRavenMessage)
