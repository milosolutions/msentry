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

#include <QtTest>
#include <QCoreApplication>

#include "msentry.h"
#include "raven.h"

class TestMSentry : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testMessages();
};

void TestMSentry::initTestCase()
{
    QCoreApplication::setApplicationName("MSentry Test");
    QCoreApplication::setOrganizationName("Milo");

    // Server offline
    //QString dsn("http://dsn:dsn.milosolutions.com/54");
    //QVERIFY(!dsn.isEmpty());
    //Q_ASSERT(MSentry::instance());

    //MSentry::instance()->setSentryDSN(dsn);

    //QVERIFY(MSentry::instance()->raven()->isInitialized());
}

void TestMSentry::cleanupTestCase()
{

}

void TestMSentry::testMessages()
{
    sentry(RAVEN_DEBUG)<<"Debug message" << RavenMessage::send;
    sentry(RAVEN_INFO)<<"Info message" << RavenMessage::send;
    sentry(RAVEN_WARNING)<<"Warning message" << RavenMessage::send;
    sentry(RAVEN_ERROR)<<"Error message" << RavenMessage::send;
    sentry(RAVEN_FATAL)<<"Fatal Message" << RavenMessage::send;
}


QTEST_MAIN(TestMSentry)

#include "tst_msentry.moc"
