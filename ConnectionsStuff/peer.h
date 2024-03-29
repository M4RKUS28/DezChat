#ifndef PEER_H
#define PEER_H

#include "my_tcp_lib.h"
#include "server_tcp_lib.h"

#include <QTime>
#include <QThread>
#define VERSION 2


class Peer : public QThread
{
    Q_OBJECT
public:
    Peer(const CLIENT &cli);
    Peer(const client_TCP_Lib &cli, std::string ip_connectedTo, unsigned short port_connectedTo);
    ~Peer();

    void send_to(QString msg);
    int startReciver();
    void sendInfoData(const unsigned short ownServerListeningPort, bool adminstate, bool inGame);

    int closeSocket();
    bool isConnected();

    std::string getIp() const;
    std::string getLastError();
    unsigned short getPort() const;
    QString getName() const;
    QString getUserName() const;
    QString getFullName() const;
    QString getJoinTime() const;

    bool isAdmin() const;
    void set_isAdmin(bool state);

    void setConnectedToPort(unsigned short port);
    void setName(std::string name);
    void setJoinTime(std::string time);

    size_t cutUpLoad();
    size_t cutDownLoad();


    bool isInGame() const;

private:
    void run();

    CLIENT cli_v1;
    client_TCP_Lib cli_v2;

    const bool isCLIENT;
    bool stop, is_Admin, inGame;
    unsigned short port_connectedTo;
    std::string name, ip_connectedTo, joinTime;

    size_t upLoad, downLoad;

signals:
    void recvdMessage(Peer * who, QString msg);
    void recvdFailed(Peer * who);
    void sendFailed(Peer * who);

};

#endif // PEER_H
