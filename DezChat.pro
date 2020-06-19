#-------------------------------------------------
#
# Project created by QtCreator 2020-03-07T19:34:54
#
#-------------------------------------------------

SOURCES      += \
    connetionsmanager.cpp \
    peer.cpp \
    tryconnector.cpp \
    chatwidget.cpp \
    wormio_graphicsview.cpp \
    wormio_scene.cpp
HEADERS      += connetionsmanager.h \
    peer.h \
    tryconnector.h \
    chatwidget.h \
    wormio_graphicsview.h \
    wormio_scene.h

INCLUDEPATH  += $$PWD/../My_LIB_TCP_V2/ \
                $$PWD/../My_LIB_TCP_SERVER_V2/

HEADERS      += $$PWD/../My_LIB_TCP_V2/my_tcp_lib.h \
                $$PWD/../My_LIB_TCP_SERVER_V2/server_tcp_lib.h
SOURCES      += $$PWD/../My_LIB_TCP_V2/my_tcp_lib.cpp \
                $$PWD/../My_LIB_TCP_SERVER_V2/server_tcp_lib.cpp


QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DezChat
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
