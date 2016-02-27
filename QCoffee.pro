#-------------------------------------------------
#
# Project created by QtCreator 2016-02-06T07:54:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QCoffee
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    brewparams.cpp \
    brewers.cpp \
    cups.cpp \
    strength.cpp \
    options.cpp

HEADERS  += mainwindow.h \
    brewparams.h \
    brewers.h \
    cups.h \
    strength.h \
    options.h

FORMS    += mainwindow.ui \
    options.ui

LIBS += -L/usr/lib -lwiringPi
