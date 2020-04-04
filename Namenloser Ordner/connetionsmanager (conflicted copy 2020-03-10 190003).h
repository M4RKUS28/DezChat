#ifndef CONNETIONSMANAGER_H
#define CONNETIONSMANAGER_H

#include "TryConnector.h"

#include <vector>
#include <QThread>

class ConnetionsManager : public QThread
{
public:
    std::string replace(std::string str, std::string substr1, std::string substr2);
    struct PC {
        std::string ip;
        unsigned short port;
    };


    ConnetionsManager();
    int start(std::vector<PC> computers);

private:

    TCP_SERVER * server;
    std::vector<TryConnector*> Connections;
    std::vector<PC> computers;



    int startServer();
    int startTryConnectThreads();

    void run();


    struct ThreadData {
        ThreadData(std::vector<Peer*> *clients) { this->clients = clients; }
        std::vector<Peer*> *clients;
    };



signals:
    void showMSG(QString msg);

};

#endif // CONNETIONSMANAGER_H
