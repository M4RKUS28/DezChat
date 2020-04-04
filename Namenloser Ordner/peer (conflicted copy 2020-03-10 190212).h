#ifndef PEER_H
#define PEER_H

#include "my_tcp_lib.h"
#include "server_tcp_lib.h"

#include <QThread>


class Peer : QThread
{
public:
    Peer(CLIENT cli);
    Peer(client_TCP_Lib cli);
    const bool isCLIENT;



private:
    void run();
};

#endif // PEER_H
