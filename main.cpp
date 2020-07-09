#include "mainwindow.h"
#include <QApplication>

/*
 * MainWindow
 *      Connectionmanager
 *          <Vector> Connections: (Peers)
 *              CLIENT oder client_TCP_Lib   -> RecivThread + SendeFunktion
 *          Via Signal: TryConnectors
 *              client_TCP_Lib               -> Try to Connect
 *          Client-Reciver-Thread            -> Look for incomming Clients
 *
 *
*/




int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
