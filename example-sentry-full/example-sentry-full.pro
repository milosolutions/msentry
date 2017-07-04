QT = core
CONFIG += c++11

CONFIG += sentry
include(../msentry.pri)

TARGET = example-sentry-full
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp
