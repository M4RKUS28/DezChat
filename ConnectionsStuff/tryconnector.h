#ifndef TRYCONNECTOR_H
#define TRYCONNECTOR_H

#include "my_tcp_lib.h"
#include "server_tcp_lib.h"

#include <QThread>

#include <QEventLoop>

enum STATE {
    OK = 0,
    UNDEFINED = 1,
    UNDEFINED_ERROR = 2,
    NAME_SOLVING_FAILED = 3
};

/*

class Connector : public QThread
{
    Q_OBJECT
public:
    Connector();
    ~Connector();

    int ConnectTo();
    int getReturnValue();

    client_TCP_Lib client;
    std::string ip;
    unsigned short port;

private:
    void run();
    int returnValue;

};
*/

class TryConnector: public QThread
{
    Q_OBJECT
public:

    TryConnector();
    ~TryConnector();

    int tryConnectTo(std::string ip, unsigned short port);


    const STATE &getState();

    const std::string &getIp();
    const unsigned short &getPort();
    const client_TCP_Lib &client();



private:
    void run();
    STATE state;
    //Connector connector;
    std::string ip;
    unsigned short port;
    client_TCP_Lib client_;

signals:
    void finished(TryConnector * who);

};

#endif // TRYCONNECTOR_H
