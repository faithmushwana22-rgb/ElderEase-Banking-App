#-------------------------------------------------
#
# Project created by QtCreator 2026-03-22T18:41:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ElderEaseBank
TEMPLATE = app


SOURCES += main.cpp\
        eldereaseapp.cpp \
    loginpage.cpp \
    dashboard.cpp

HEADERS  += eldereaseapp.h \
    loginpage.h \
    dashboard.h

FORMS    += eldereaseapp.ui \
    loginpage.ui \
    dashboard.ui
CONFIG += c++11

RESOURCES += \
    resources.qrc
