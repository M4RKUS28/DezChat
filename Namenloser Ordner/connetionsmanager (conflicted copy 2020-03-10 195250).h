#ifndef CONNETIONSMANAGER_H
#define CONNETIONSMANAGER_H

#include "peer.h"
#include "tryconnector.h"

#include <vector>
#include <QThread>

class ConnetionsManager : public QThread
{
    Q_OBJECT
public:
    struct PC {
        PC();
        PC(std::string ip, unsigned short port) : ip(ip), port(port) {}
        std::string ip;
        unsigned short port;
    };


    ConnetionsManager();
    int start(std::vector<PC> computers);

private:

    TCP_SERVER * server;
    std::vector<Peer*> Connections;
    std::vector<PC> computers;


    void run();


    struct ThreadData {
        ThreadData(std::vector<Peer*> *clients) { this->clients = clients; }
        std::vector<Peer*> *clients;
    };



signals:
    void showMSG(QString msg);

public slots:
    void TryConnectorFinished(TryConnector * who);
    void TryConnectorStartedConnecting(TryConnector * who);

};

#endif // CONNETIONSMANAGER_H
