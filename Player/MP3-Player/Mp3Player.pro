#-------------------------------------------------
#
# Project created by QtCreator 2017-02-28T08:51:57
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mp3Player
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        audiofile.cpp \
        mainwindow.cpp \
        operationswindow.cpp \
        propertieswindow.cpp

HEADERS  += mainwindow.h \
    audiofile.h \
    operationswindow.h \
    players.h \
    propertieswindow.h

FORMS    += mainwindow.ui \
    operationswindow.ui \
    propertieswindow.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    Mp3Player.rc

RC_FILE = Mp3Player.rc
