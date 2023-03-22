#-------------------------------------------------
#
# Project created by QtCreator 2012-04-13T16:55:12
#
#-------------------------------------------------

QT       += core gui xml #sql

TARGET = WoTReplayManager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    readwotreplay.cpp \
    sqltable.cpp \
    infowotreplay.cpp \
    settings.cpp \
    settingwidget.cpp \
    qcheckboxpath.cpp \
    replayinfo.cpp

HEADERS  += mainwindow.h \
    readwotreplay.h \
    sqltable.h \
    infowotreplay.h \
    settings.h \
    settingwidget.h \
    qcheckboxpath.h \
    replayinfo.h

FORMS    += mainwindow.ui \
    settingwidget.ui \
    replayinfo.ui

#INCLUDEPATH += "/usr/include/qjson/"


#LIBS += "/usr/lib/libjson.a"
