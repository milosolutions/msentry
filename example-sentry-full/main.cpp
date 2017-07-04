/*******************************************************************************
  Copyright (C) 2016 Milo Solutions
  Contact: https://www.milosolutions.com

  This file is part of Milo Code Database, which is a collection of useful code
  snippets often reused within Milo Solutions. As such, this code is present in
  many projects done by Milo Solutions and does not constitute a copyright
  infridgement.

  You are entitled to do anything you please with this code, just as with the
  rest of code you bought. Files not containing this notice were written
  specifically for this project and have not been shared with any other party
  within or without Milo.
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
    Sentry::instance()->setSentryDSN(dsn);
    qDebug() << "Sentry client was created"
           << (Sentry::instance()->raven()->isInitialized() ? "and initialized" : "but NOT initialized");

    qDebug() << "Logging debug message...";
    sentry(RAVEN_DEBUG)<<"Debug message" << RavenMessage::send;
    qDebug() << "Logging info message...";
    sentry(RAVEN_INFO)<<"Info message" << RavenMessage::send;
    qDebug() << "Logging warning message...";
    sentry(RAVEN_WARNING)<<"Warning message" << RavenMessage::send;
    qDebug() << "Logging error message...";
    sentry(RAVEN_ERROR)<<"Error message" << RavenMessage::send;
    qDebug() << "Logging fatal message...";
    sentry(RAVEN_FATAL)<<"Fatal Message" << RavenMessage::send;

    qDebug() << "Logging tagged debug message...";
    sentry(RAVEN_DEBUG) << "Tagged debug message" << RavenTag("tag_name", "tag_value") << RavenMessage::send;

    qDebug() << "Done!";

    try {
        qDebug() << "Raising exception...";
        CallHierarchy();
    } catch (const std::exception &ex) {
        qDebug() << "Catching the exception...";
        sentry(RAVEN_FATAL) << "Exception occurred" << ex << RavenMessage::send;
    }

    return a.exec();
}
