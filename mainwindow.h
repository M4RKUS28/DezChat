#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidget>
#include <QMainWindow>
#include "connetionsmanager.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void timerEvent(QTimerEvent *);

private slots:
    void printInfo(QString str);
    void recvedMSG(Peer *who, QString msg);
    void updateClientList();

    void on_inputLine_returnPressed();
    void on_Connections_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    ConnetionsManager *manager;
    int timerID, timerForWarningMSG, wrongClientCountCounter;
    bool oldPrintStyle;
};

#endif // MAINWINDOW_H
