#include "peer.h"

Peer::Peer(const CLIENT &cli)
    : cli_v1(cli), isCLIENT(true), stop(false), is_Admin(false), inGame(false), name("UNKNOWN"), upLoad(0), downLoad(0)
{
    this->cli_v1.autoCleanUpInTheEnd = false;
    this->cli_v2.autoCleanUpInTheEnd = false;
    //Save ip in var, which will be deletet after disconnect...
    this->ip_connectedTo = cli.getIpAddress();

}

Peer::Peer(const client_TCP_Lib &cli, std::string ip_connectedTo, unsigned short port_connectedTo)
    :  cli_v2(cli), isCLIENT(false), stop(false), is_Admin(false), inGame(false), port_connectedTo(port_connectedTo), name("UNKNOWN"),
      ip_connectedTo(ip_connectedTo), upLoad(0), downLoad(0)
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
            if( ! this->wait(3000) )
                std::cout << "Terminating Thread timeouted: " << __func__ << std::endl;
        }
    }

    if( this->isConnected() && this->closeSocket() != 0)
        std::cout << "!-> CloseSocket failed: " << cli_v1.getLastError() << std::endl;
}

void Peer::send_to(QString msg)
{
    if(*msg.end() != '|')
        msg.push_back('|');

    if( ((isCLIENT) ? cli_v1.send_(msg.toStdString().c_str(), static_cast<unsigned>(msg.toStdString().length()) )   :
                      send(cli_v2.getConnectionSocket(), msg.toStdString().c_str(), static_cast<unsigned>(msg.toStdString().length()), 0)) <= 0 ) {
        std::cerr << "DEBUG: emit -> send failed to: " << this->getFullName().toStdString() << std::endl;
        emit sendFailed(this);
    }

    upLoad += msg.length();

}

int Peer::startReciver()
{
    if(this->isRunning())
        return -1;
    this->start();
    return this->isRunning() ? 0 : 1;
}

void Peer::sendInfoData(const unsigned short ownServerListeningPort, bool adminstate, bool inGame)
{
    this->send_to("PORT=" + QString::number(ownServerListeningPort));
    this->send_to("JOIN_TIME=" + QTime::currentTime().toString() );
    this->send_to("JOINED=" + getUserName() );
    this->send_to("VERSION=" + QString::number(VERSION) );

    this->send_to("adminstatus=" + QString(adminstate ? "true" : "false"));
    this->send_to("IN_GAME=" + QString(inGame ? "true" : "false"));

}

int Peer::closeSocket()
{
    return (isCLIENT) ? cli_v1.closeSocket() : cli_v2.closeSocket();
}

bool Peer::isConnected()
{
    return (isCLIENT) ? cli_v1.isConnected() : cli_v2.isConnected();
}

std::string Peer::getIp() const
{
    return ip_connectedTo;
}


std::string Peer::getLastError()
{
    return (isCLIENT) ? cli_v1.getLastError() : cli_v2.getLastError();
}

unsigned short Peer::getPort() const
{
    return port_connectedTo;
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

bool Peer::isAdmin() const
{
    return is_Admin;
}

bool Peer::isInGame() const
{
    return inGame;
}

void Peer::set_isAdmin(bool state)
{
    is_Admin = state;
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

size_t Peer::cutUpLoad()
{
    size_t ret = upLoad;
    upLoad = 0;
    return ret;
}

size_t Peer::cutDownLoad()
{
    size_t ret = downLoad;
    downLoad = 0;
    return ret;
}

void Peer::run()
{
    std::vector<char> buffer;
    std::string stringBuffer;
    char c;

    while ( ! stop ) {
        buffer.clear();
        do {
            if( ( ( isCLIENT ) ? cli_v1.recv_(&c, 1) : recv(cli_v2.getConnectionSocket(), &c, 1, 0) ) <= 0) {
                std::cerr << "DEBUG: emit -> rev failed from: " << this->getFullName().toStdString() << std::endl;
                return emit recvdFailed(this);
            } else
                buffer.push_back(c);
        } while (c != '|' && ! stop);

        downLoad += buffer.size();

        if(buffer.data() && buffer.size() > 0) {
            buffer.pop_back();
            buffer.push_back('\0');

            if( (stringBuffer = std::string(buffer.data())) .find("=") == std::string::npos) {
                std::cout << "INVALID MSG: '" << buffer.data() << "'" << std::endl;
                continue;
            } else
                emit recvdMessage(this, QString::fromStdString(stringBuffer));
        }
    }
    emit recvdFailed(this);
}

