#include "msentry.h"

Sentry *Sentry::_instance = nullptr;

/*!
 * \class Sentry
 * \brief Enables a seamless integration with Sentry web logging server.
 *
 * Sentry class can be used very conveniently - you can add sentry() calls
 * wherever you need them. If you need to turn sentry off at any point, just
 * compile with qmake -confing no-sentry. No need to add any #ifdefs around
 * sentry calls - they will not produce compilation errors.
 *
 * See example-sentry-basic and example-sentry-full for code snippets.
 */

/*!
 * Returns the instance of Sentry (which is a singleton). You can also use the
 * sentry() function to achieve the same goal.
 *
 * \sa sentry()
 */
Sentry *Sentry::instance()
{
    if (!_instance) {
        _instance = new Sentry();
    }

    return _instance;
}

/*!
 * Sets sentry \a dsn, thus initializing Sentry object. MiloLog manages life
 * of Sentry object, you do not need to delete it.
 *
 * It is OK to pass an empty string here - Sentry object will be turned off.
 *
 * More info about DSN: https://docs.sentry.io/quickstart/#configure-the-dsn
 */
void Sentry::setSentryDSN(const QString &dsn)
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
 * \warning You HAVE TO inistialize Sentry object first using setSentryDSN() or
 * your app will crash!
 *
 * \sa setSentryDSN
 */
Raven *Sentry::raven() const
{
#ifdef SENTRY
    Q_ASSERT(_sentry);
    return _sentry;
#else
    return nullptr;
#endif
}

Sentry::Sentry()
{
}

Sentry::~Sentry()
{
#ifdef SENTRY
    delete _sentry.data();
#endif
}

RavenMessage sentry(RavenMessage::Level level, const QString &culprit) {
#ifdef SENTRY
    return Sentry::instance()->raven()->operator()(level, culprit);
#else
    Q_UNUSED(level)
    Q_UNUSED(culprit)
    return RavenMessage();
#endif
}
