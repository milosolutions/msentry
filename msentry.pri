# Copyright (C) 2016 by Milo Solutions
#
# qmake file for QRaven module
#
# Distributed under terms specified in LICENSE file
#

QT += core network
CONFIG += c++14
DEFINES *= QT_USE_QSTRINGBUILDER

# Thin wrapper - your code will compile even if you define -config no-sentry
HEADERS += $$PWD/msentry.h
SOURCES += $$PWD/msentry.cpp

## Respect qmake command-line flags:
no-sentry {
    # Even with Sentry support turned off, calls in code to sentry() will
    # continue to compile (they will just not do anything).
    CONFIG -= sentry
}

## Feature switcher
## To enable Sentry use:
# qmake -config sentry
# OR:
# CONFIG+=sentry
##
## To disable it, use:
# qmake -config no-sentry
sentry {
    DEFINES+=SENTRY
    message("Sentry support: enabled")
    HEADERS+= $$PWD/raven.h $$PWD/ravendefs.h
    SOURCES+= $$PWD/raven.cpp $$PWD/ravendefs.cpp

    ## QRaven supports stack trace recovery. This requires additional libs.
    #  If you get compilation errors, either fix them or disable stack trace support
    #  using switch below:
    CONFIG+=qraven_stackrace

    qraven_stackrace{
        message("Stack trace support for QRaven: enabled (see milolog/sentry for more details)")
        DEFINES+=QRAVEN_STACKTRACE
        unix{
            LIBS+=-ldl
            QMAKE_LFLAGS += -rdynamic
        }
    } else {
        message("Stack trace support for QRaven: disabled")
    }
} else {
    message("Sentry support: disabled")
}
