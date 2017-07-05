QT = core
CONFIG += c++14

CONFIG += sentry
#CONFIG += no-sentry-print
include(../msentry.pri)

TARGET = example-sentry-basic
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

