#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidget>
#include <QMainWindow>
#include "ConnectionsStuff/connetionsmanager.h"

#include "wormio_game/game.h"

#include <iostream>
#include <algorithm>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void timerEvent(QTimerEvent * e);
private slots:
    void printInfo(QString str);
    void recvedMSG(Peer *who, QString msg);
    void updateClientList();

    void on_inputLine_returnPressed();
    void on_Connections_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    ConnetionsManager *manager;
    int timerID, timerForWarningMSG, wrongClientCountCounter, printUpDownLoadStatsTimer;
    bool oldPrintStyle;



//Game

public:
    void joinGame();

public slots:
    void leaveGame();
    void sendGameMsg( QString msg );
    void sendGameMsgTo(QString msg, Peer * who);

private:

    Game * game;

signals:
    void gotGameMsg(QString msg, Peer * who);


};

#endif // MAINWINDOW_H
