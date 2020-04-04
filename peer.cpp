#include "peer.h"

Peer::Peer(const CLIENT &cli)
    : cli_v1(cli), isCLIENT(true), stop(false), name("UNKNOWN")
{
    this->cli_v1.autoCleanUpInTheEnd = false;
    this->cli_v2.autoCleanUpInTheEnd = false;
}

Peer::Peer(const client_TCP_Lib &cli, std::string ip_connectedTo, unsigned short port_connectedTo)
    :  cli_v2(cli), isCLIENT(false), stop(false), port_connectedTo(port_connectedTo), name("UNKNOWN"), ip_connectedTo(ip_connectedTo)
{
    this->cli_v1.autoCleanUpInTheEnd = false;
    this->cli_v2.autoCleanUpInTheEnd = false;
}

Peer::~Peer()
{
    std::cout << "~Peer(): port: " << getPort() << std::endl;

    if(this->isRunning()) {
        this->quit();
        if( ! this->wait(500) ) {
            this->terminate();
            if( ! this->wait(3000) ) {
                std::cout << "Terminating Thread timeouted: " << __func__ << std::endl;
            }
        }
    }

    if(this->isConnected())
        if( this->closeSocket() != 0)
            std::cout << "!-> CloseSocket failed: " << cli_v1.getLastError() << std::endl;
}

void Peer::send_to(QString msg)
{
    msg.push_back('|');

    if(isCLIENT) {
        //std::cout << "send to socket: " << cli_v1.getSocket() << std::endl;
        if( cli_v1.send_(msg.toStdString().c_str(), static_cast<unsigned>(msg.toStdString().length()) ) <= 0) {
            std::cout << "emit -> sned failed" << std::endl;

            emit sendFailed(this);
            return;
        }
    } else {
        //std::cout << "send to socket: " << cli_v2.getConnectionSocket() << std::endl;
        if( send(cli_v2.getConnectionSocket(), msg.toStdString().c_str(), static_cast<unsigned>(msg.toStdString().length()), 0) <= 0) {
            std::cout << "emit -> sned failed" << std::endl;

            emit sendFailed(this);
            return;
        }
    }
}

int Peer::closeSocket()
{
    if(isCLIENT)
        return cli_v1.closeSocket();
    else
        return cli_v2.closeSocket();
}

bool Peer::isConnected()
{
    if(isCLIENT)
        return cli_v1.isConnected();
    else
        return cli_v2.isConnected();
}

int Peer::startReciver()
{
    if(this->isRunning())
        return -1;
    this->start();
    return this->isRunning() ? 0 : 1;
}

void Peer::sendInfoData(const unsigned short ownServerListeningPort)
{
    this->send_to("PORT=" + QString::number(ownServerListeningPort));
    this->send_to("VERSION=" + QString(VERSION) );
    this->send_to("JOIN_TIME=" + QTime::currentTime().toString() );

    this->send_to("JOINED=" + getUserName() );

}

std::string Peer::getIp() const
{
    if(isCLIENT) {
        return cli_v1.getIpAddress();
    } else {
        return ip_connectedTo;
    }
}


std::string Peer::getLastError()
{
    if(isCLIENT) {
        return cli_v1.getLastError();
    } else {
        return cli_v2.getLastError();
    }
}

unsigned short Peer::getPort() const
{
    if(isCLIENT) {
        return port_connectedTo;
    } else {
        return port_connectedTo;
    }
}

QString Peer::getName() const
{
    return QString::fromStdString(name);
}

QString Peer::getUserName() const
{
    return ( (qgetenv("USER").isEmpty()) ? qgetenv("USERNAME") : qgetenv("USER") );
}

QString Peer::getFullName() const
{
    return QString("<" + getName() + "@" + QString::fromStdString(getIp()) + ":" + QString::number(getPort()) + ">");
}

QString Peer::getJoinTime() const
{
    return QString::fromStdString(joinTime);
}

void Peer::setConnectedToPort(unsigned short port)
{
    port_connectedTo = port;
}

void Peer::setName(std::string name)
{
    this->name = name;
}

void Peer::setJoinTime(std::string time)
{
    joinTime = time;
}

void Peer::run()
{
    //emit recvdMessage(this, "DEBUG=Start ReciveThread: From: " + QString::fromStdString( this->getIp() )+ ":" + QString::number( this->getPort() ) );
    std::vector<char> buffer;

    while ( ! stop ) {
        buffer.clear();

        ssize_t recvedbytes;
        char c;
        do {
            if( isCLIENT )
                recvedbytes = cli_v1.recv_(&c, 1);
            else
                recvedbytes = recv(cli_v2.getConnectionSocket(), &c, 1, 0);

            if(recvedbytes <= 0) {
                std::cout << "emit -> rev failed" << std::endl;
                emit recvdFailed(this);
                return;
            } else
                buffer.push_back(c);

        } while (c != '|');

        if(buffer.size() > 0)
            buffer.pop_back();
        buffer.push_back('\0');

        std::string stringBuffer;
        if(!buffer.data() || (stringBuffer = std::string(buffer.data())) .find("=") == std::string::npos) {
            std::cout << "INVALID MSG: '" << buffer.data() << "'" << std::endl;
            continue;
        }

        emit recvdMessage(this, QString::fromStdString(stringBuffer));
    }
}
