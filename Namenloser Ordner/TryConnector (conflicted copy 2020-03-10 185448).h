#ifndef PEER_H
#define PEER_H

#include "my_tcp_lib.h"
#include "server_tcp_lib.h"

#include <QThread>


class TryConnector: public QThread
{
public:
    enum STATE {
        UNDEFINED,
        CONNECTING,
        OK,
        UNKNOWN_ERROR,
        TIMEOUT
    };


    TryConnector();

    int tryConnectTo(std::string ip, unsigned short port);
    STATE getState();
    std::string getIp();
    unsigned short getPort();

    client_TCP_Lib client;


private:

    void run();
    std::string ip;
    unsigned short port;
    STATE state;


signals:
    void finished();
    void startConnecting();


};

#endif // PEER_H
