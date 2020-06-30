#include "tryconnector.h"



TryConnector::TryConnector(void)
    : state(UNDEFINED)
{
    client_.autoCleanUpInTheEnd = false;

}

TryConnector::~TryConnector()
{
    //std::cout << "~TryConnector()" << std::endl;
    if(this->isRunning()) {
        this->quit();
        if( ! this->wait(500) ) {
            this->terminate();
            if( ! this->wait(3000) )
                std::cout << "Terminating Thread timeouted: " << __func__ << std::endl;
        }
    }
}


int TryConnector::tryConnectTo(std::string ip, unsigned short port)
{
    if(this->isRunning())
        return -1;

    this->ip = ip;
    this->port = port;

    this->start();
    return this->isRunning() ? 0 : 1;
}


const client_TCP_Lib &TryConnector::client()
{
    return client_;
}

const STATE &TryConnector::getState()
{
    return this->state;
}

const std::string &TryConnector::getIp()
{
    return ip;
}

const unsigned short &TryConnector::getPort()
{
    return port;
}


void TryConnector::run()
{
    this->state = STATE::UNDEFINED;
    std::string ipSolved = client_.getIpByName( ip );
    if(ipSolved == "")
        this->state = NAME_SOLVING_FAILED;
    else
        this->state = (client_.connectTo(ipSolved, port, 200, 3) == 0) ? STATE::OK : STATE::UNDEFINED_ERROR;

    emit finished(this);
    return;

}
