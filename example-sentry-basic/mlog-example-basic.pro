QT = core
CONFIG += c++14

CONFIG+=sentry
include(../mlog.pri)

TARGET = mlog-example-basic
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp
