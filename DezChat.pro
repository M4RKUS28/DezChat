#-------------------------------------------------
#
# Project created by QtCreator 2020-03-07T19:34:54
#
#-------------------------------------------------

SOURCES      += \
    ConnectionsStuff/connetionsmanager.cpp \
    ConnectionsStuff/peer.cpp \
    ConnectionsStuff/tryconnector.cpp \
    chatwidget.cpp \
    wormio_game/enemy.cpp \
    wormio_game/enemymanager.cpp \
    wormio_game/game.cpp \
    wormio_game/minimap.cpp \
    wormio_game/player.cpp \
    wormio_game/wormpart.cpp

HEADERS      += \
    ConnectionsStuff/connetionsmanager.h \
    ConnectionsStuff/peer.h \
    ConnectionsStuff/tryconnector.h \
    chatwidget.h \
    wormio_game/enemy.h \
    wormio_game/enemymanager.h \
    wormio_game/game.h \
    wormio_game/minimap.h \
    wormio_game/player.h \
    wormio_game/wormio_graphicsview.h \
    wormio_game/wormpart.h

INCLUDEPATH  += $$PWD/../TCP_LIB_CLIENT_V2/ \
                $$PWD/../TCP_LIB_SERVER_V2/

HEADERS      += $$PWD/../TCP_LIB_CLIENT_V2/my_tcp_lib.h \
                $$PWD/../TCP_LIB_SERVER_V2/server_tcp_lib.h
SOURCES      += $$PWD/../TCP_LIB_CLIENT_V2/my_tcp_lib.cpp \
                $$PWD/../TCP_LIB_SERVER_V2/server_tcp_lib.cpp


QT       += core gui

win32-msvc* {
    LIBS+= -lws2_32
}

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

RESOURCES += \
    resources.qrc
