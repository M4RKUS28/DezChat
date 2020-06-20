#ifndef CONNETIONSMANAGER_H
#define CONNETIONSMANAGER_H

#include "peer.h"
#include "tryconnector.h"

#include <vector>
#include <QThread>
#include <QMap>



class ConnetionsManager : public QThread
{
    Q_OBJECT
public:
    struct PC {
        PC(std::string ip, unsigned short port) : ip(ip), port(port) {}
        std::string ip;
        unsigned short port;
    };

    ConnetionsManager();
    ~ConnetionsManager();

    int start_Thread(bool PORT_BASED, std::vector<PC> computers);

    void sendtoAllPeers(QString msg);
    std::vector<Peer *> &getConnectionList();

     bool isAdmin;


private:
    void run();

    int startAllTryConnector();
    int startServer();
    int acceptClient();

    TCP_SERVER * server;
    std::vector<PC> computers;
    std::vector<Peer*> Connections;
    bool stop, PORT_BASED;





signals:
    void showMSG(QString msg);
    void ConnectionsListChanged();
    void sendMessageWeiterLeitung(Peer * who, QString msg);

public slots:
    void MessageWeiterLeitung(Peer * who, QString msg);
    void TryConnectorFinished(TryConnector * who);
    void PeerConnectionGetsError(Peer * who);

};

#endif // CONNETIONSMANAGER_H
