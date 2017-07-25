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
#include <QLoggingCategory>
#include <QDebug>
#include <QTimer>

#include "msentry.h"
Q_LOGGING_CATEGORY(coreMain, "core.main")

//! Example use of MSentry
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setApplicationName("Basic sentry example logger app");

    MSentry::instance()->setSentryDSN("<<PUT VALID DSN HERE>>");

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
