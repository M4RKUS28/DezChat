#ifndef TRYCONNECTOR_H
#define TRYCONNECTOR_H

#include "my_tcp_lib.h"
#include "server_tcp_lib.h"

#include <QThread>

class TryConnector: public QThread
{
    Q_OBJECT
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
    void finished(TryConnector * who);
    void startConnecting(TryConnector * who);


};

#endif // TRYCONNECTOR_H
