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

#include "msentry.h"

MSentry *MSentry::_instance = nullptr;

/*!
 * \class MSentry
 * \brief Enables a seamless integration with Sentry web logging server.
 *
 * MSentry class can be used very conveniently - you can add sentry() calls
 * wherever you need them. If you need to turn sentry off at any point, just
 * compile with qmake -confing no-sentry. No need to add any #ifdefs around
 * sentry calls - they will not produce compilation errors.
 *
 * See example-sentry-basic and example-sentry-full for code snippets.
 */

/*!
 * Returns the instance of MSentry (which is a singleton). You can also use the
 * sentry() function to achieve the same goal.
 *
 * \sa sentry()
 */
MSentry *MSentry::instance()
{
    if (!_instance) {
        _instance = new MSentry();
    }

    return _instance;
}

/*!
 * Sets sentry \a dsn, thus initializing MSentry object. MLog manages life
 * of MSentry object, you do not need to delete it.
 *
 * It is OK to pass an empty string here - Sentry object will be turned off.
 *
 * More info about DSN: https://docs.sentry.io/quickstart/#configure-the-dsn
 */
void MSentry::setSentryDSN(const QString &dsn)
{
#ifdef SENTRY
    _sentry = new Raven(dsn, nullptr);
#else
    Q_UNUSED(dsn);
#endif
}

/*!
 * Returns a pointer to Sentry object (Raven class).
 *
 *
 * \warning You HAVE TO inistialize MSentry object first using setSentryDSN() or
 * your app will crash!
 *
 * \sa setSentryDSN
 */
Raven *MSentry::raven() const
{
#ifdef SENTRY
    Q_ASSERT(_sentry);
    return _sentry;
#else
    return nullptr;
#endif
}

MSentry::MSentry()
{
}

MSentry::~MSentry()
{
#ifdef SENTRY
    delete _sentry.data();
#endif
}

RavenMessage sentry(RavenMessage::Level level, const QString &culprit) {
#ifdef SENTRY
    return MSentry::instance()->raven()->operator()(level, culprit);
#else
    Q_UNUSED(level)
    Q_UNUSED(culprit)
    return RavenMessage();
#endif
}
