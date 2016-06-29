#-------------------------------------------------
#
# Project created by QtCreator 2016-06-08T09:32:48
#
#-------------------------------------------------

QT       += core gui serialport

CONFIG+=C++11 console debug

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Arrozr_Central
TEMPLATE = app


SOURCES += mainwindow.cpp\
        main.cpp \
    devicewatcher.cpp \
    device.cpp

HEADERS  += mainwindow.h \
    devicewatcher.h \
    device.h

FORMS    += mainwindow.ui
