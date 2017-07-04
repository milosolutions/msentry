#pragma once

#ifdef SENTRY
#include "raven.h"
#endif

#include <QString>
#include <QPointer>

class Raven;

class Sentry {
public:
    static Sentry *instance();
    void setSentryDSN(const QString &dsn);
    Raven *sentry() const;

private:
    Q_DISABLE_COPY(Sentry)
    explicit Sentry();
    ~Sentry();

    static Sentry *_instance;
    QPointer<Raven> _sentry;
};

// Class which mutes sentry when SENTRY is not defined.
class SentrySpirit {
public:
    template<typename T>
    SentrySpirit& operator<<( T arg );
};

template<typename T>
SentrySpirit& SentrySpirit::operator<<(T arg) {
    Q_UNUSED(arg);
    return *this;
}

/*!
 * Returns the sentry message if SENTRY is defined, otherwise does nothing.
 * If you need actual Sentry object (Raven class
 * instance), use Sentry::sentry() instead.
 *
 * The easiest way to use it is to use it is like this:
 \code
 sentry(RAVEN_INFO) << "Test log message for Sentry" << RavenTag("Core", "Main") << Raven::send;
 \endcode
 */
template<typename Level, typename Culprit>
SentrySpirit sentry(Level level, Culprit culprit) {
    #ifdef SENTRY
        return Sentry::instance()->sentry()->operator()(level, culprit);
    #else
        Q_UNUSED(level)
        Q_UNUSED(culprit)
        return SentrySpirit();
    #endif
}
