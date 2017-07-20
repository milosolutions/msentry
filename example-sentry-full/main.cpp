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


#include <QCoreApplication>
#include <QDebug>

#include "msentry.h"

class CallHierarchy
{
public:
    CallHierarchy(){topLevelMethod(0);}
    virtual ~CallHierarchy(){}
    void topLevelMethod(int){middleLevelMethod();}
    void middleLevelMethod(){deepestMethod(true);}
    void deepestMethod(bool)
    {
        // real exception here
        dynamic_cast<QObject&>(*this);
    }
};

int main(int argc, char *argv[])
{
    qDebug()<<"Initializing...";
    QCoreApplication a(argc, argv);
    a.setApplicationName("Full sentry example logger app");

    const QString dsn("<<PUT VALID DSN HERE>>");
    qDebug() << "DSN(Data Source Name) is defined:" << dsn;
    qDebug() << "Creating Sentry client...";
    MSentry::instance()->setSentryDSN(dsn);
    qDebug() << "Sentry client was created"
           << (MSentry::instance()->raven()->isInitialized() ? "and initialized" : "but NOT initialized");

    qDebug() << "Logging debug message...";
    sentry(RAVEN_DEBUG)<<"Debug message" << MRavenMessage::send;
    qDebug() << "Logging info message...";
    sentry(RAVEN_INFO)<<"Info message" << MRavenMessage::send;
    qDebug() << "Logging warning message...";
    sentry(RAVEN_WARNING)<<"Warning message" << MRavenMessage::send;
    qDebug() << "Logging error message...";
    sentry(RAVEN_ERROR)<<"Error message" << MRavenMessage::send;
    qDebug() << "Logging fatal message...";
    sentry(RAVEN_FATAL)<<"Fatal Message" << MRavenMessage::send;

    qDebug() << "Logging tagged debug message...";
    sentry(RAVEN_DEBUG) << "Tagged debug message" << MRavenTag("tag_name", "tag_value") << MRavenMessage::send;

    qDebug() << "Done!";

    try {
        qDebug() << "Raising exception...";
        CallHierarchy();
    } catch (const std::exception &ex) {
        qDebug() << "Catching the exception...";
        sentry(RAVEN_FATAL) << "Exception occurred" << ex << MRavenMessage::send;
    }

    return a.exec();
}
