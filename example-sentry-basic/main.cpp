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
#include <QLoggingCategory>
#include <QDebug>
#include <QTimer>

#include "mlog.h"
#include "sentry.h"
Q_LOGGING_CATEGORY(coreMain, "core.main")

//! Example use of MLog class
int main(int argc, char *argv[])
{
    // Create MiloLog instance before Q*Application to capture all messages it
    // generates
    MLog::instance();
    QCoreApplication a(argc, argv);
    a.setApplicationName("Basic sentry example logger app");

    // If you want to enable pushing all logs to a file (apart from console),
    // use enableLogToFile()
    logger()->enableLogToFile(a.applicationName());

    Sentry::instance()->setSentryDSN("http://1752d1dc10aa45f88dc307fca6fd44eb:e588ded44af648bbae3db5e1675ba07f@sentry.milosolutions.com/54");

    qCDebug(coreMain) << "Logger successfully created."
                  << "\n\tApplication name is:" << a.applicationName()
                  << "\n\tPrevious log path:" << logger()->previousLogPath()
                  << "\n\tCurrent log path:" << logger()->currentLogPath();

    // Sentry test
    qCDebug(coreMain) << "Performing sentry test.";
    // This will execute after event loop starts (a.exec()).
    // You do not need to use #ifdef if you want to disable Sentry - it will still
    // compile and run just fine
    sentry(RAVEN_INFO) << "Test MiloLog message for Sentry" << RavenTag("Core", "Main") << RavenMessage::send;
    //qCDebug(coreMain) << "Sentry message has been sent.";

    QTimer::singleShot(1000, &a, &QCoreApplication::quit);
    return a.exec();
}
