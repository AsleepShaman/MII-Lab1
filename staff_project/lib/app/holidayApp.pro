QT += core xml
QT -= gui

CONFIG += console
CONFIG -= app_bundle


DEFINES += QT_DEPRECATED_WARNINGS


SOURCES += \
        main.cpp

TEMPLATE = app
DESTDIR = ./bin
TARGET = appholiday


OBJECTS_DIR = ./.obj
MOC_DIR = ./.obj
RCC_DIR = ./.obj

SRC = $$PWD/..
COMMONSRC = $$PWD/../../common_src

INCLUDEPATH = $$PWD \
              $$SRC \
              $$COMMONSRC \

include($$SRC/holiday.pri)
include($$COMMONSRC/common_src.pri)


DISTFILES += \
    calcHoliday.xml
