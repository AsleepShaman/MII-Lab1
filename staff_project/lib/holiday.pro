QT += core xml

QT       -= gui

TARGET = holiday
TEMPLATE = lib
DESTDIR = ../bin
DEFINES += HOLIDAY_LIBRARY

OBJECTS_DIR = .obj
MOC_DIR = .obj
RCC_DIR = .obj

DEFINES += QT_DEPRECATED_WARNINGS

SRC = $$PWD
SOURCES += \
          main.cpp
HEADERS += \
    main.h

COMMONSRC = $$PWD/../common_src

INCLUDEPATH = $$PWD \
              $$SRC \
              $$COMMONSRC \

include($$SRC/holiday.pri)
include($$COMMONSRC/common_src.pri)




target.path = $$(USR_LIB_PATH)
INSTALLS += target





