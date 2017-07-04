#pragma once

#ifdef SENTRY
#include "raven.h"
#endif

#include "ravendefs.h"

#include <QString>
#include <QPointer>

class Raven;

class Sentry {
public:
    static Sentry *instance();
    void setSentryDSN(const QString &dsn);
    Raven *raven() const;

private:
    Q_DISABLE_COPY(Sentry)
    explicit Sentry();
    ~Sentry();

    static Sentry *_instance;
    QPointer<Raven> _sentry;
};

RavenMessage sentry(RavenMessage::Level level, const QString &culprit);
