# Copyright (C) 2016 by Milo Solutions
#
# qmake file for QRaven module
#
# Distributed under terms specified in LICENSE file
#

QT += core network
CONFIG += c++11
DEFINES *= QT_USE_QSTRINGBUILDER

# Thin wrapper - your code will compile even if you define -config no-sentry
HEADERS += $$PWD/msentry.h $$PWD/ravendefs.h
SOURCES += $$PWD/msentry.cpp $$PWD/ravendefs.cpp
INCLUDEPATH += $$PWD

INCLUDEPATH += $$PWD

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
    DEFINES += SENTRY
    message("Sentry support: enabled")
    HEADERS += $$PWD/raven.h
    SOURCES += $$PWD/raven.cpp

    ## QRaven supports stack trace recovery. This requires additional libs.
    no-qraven-stacktrace {
        CONFIG -= qraven-stacktrace
    }

    qraven-stacktrace{
        message("Stack trace support for QRaven: enabled")
        DEFINES += QRAVEN_STACKTRACE
        unix{
            LIBS += -ldl
            QMAKE_LFLAGS += -rdynamic
        }
    } else {
        message("Stack trace support for QRaven: disabled")
    }

    # On by default
    CONFIG += sentry-print
    # In addition to sending via network, messages will be printed to console
    no-sentry-print {
        CONFIG -= sentry-print
    }

    sentry-print {
        message("Print sentry messages to console: enabled")
        DEFINES += SENTRY_PRINT
    } else {
        message("Print sentry messages to console: disabled")
    }
} else {
    message("Sentry support: disabled")
}
