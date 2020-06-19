#include "tryconnector.h"
/*
Connector::Connector()
    : returnValue(-1)
{
    client.autoCleanUpInTheEnd = false;
}

Connector::~Connector()
{
    //std::cout << "~Connector()" << std::endl;
    if(this->isRunning()) {
        this->quit();
        if( ! this->wait(500) ) {
            this->terminate();
            if( ! this->wait(3000) ) {
                std::cout << "Terminating Thread timeouted: " << __func__ << std::endl;
            }
        }
    }
}

int Connector::ConnectTo()
{
    if(this->isRunning())
        return -1;
    this->start();
    return this->isRunning() ? 0 : 1;
}

int Connector::getReturnValue()
{
    return returnValue;
}

void Connector::run()
{
    returnValue = client.connectTo(client.getIpByName(ip), port);
}
*/

///----------------------------------------------------------------------------------------->


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

    //this->connector.ip = ip;
    //this->connector.port = port;
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
    this->state = (client_.connectTo(ip, port, 200, 3) == 0) ? STATE::OK : STATE::UNDEFINED_ERROR;
    emit finished(this);
    return;


    /*
    this->state = STATE::UNDEFINED;
    if(connector.ConnectTo() != 0) {
        this->state = STATE::UNDEFINED_ERROR;

    } else {
        unsigned timeout_HectSec = 200;
        // 3 Versuche bei Timeout
        for (int Try = 1; Try <= 3; ++Try) {

            for (unsigned time = 1; time <= timeout_HectSec; ++time) {

                if(connector.isRunning()) {
                    usleep(10000); // sleep 0,01s
                    if(time > 100)
                        std::cout << ( "\33[2K\r" "Waited " +  std::to_string(static_cast<double>(time) / 100 ) + "sec..." ) << std::flush;
                } else {
                    this->state = (this->connector.getReturnValue() == 0) ? STATE::OK : STATE::UNDEFINED_ERROR;
                    //std::cout << "Thread finished mit res = " << connector.getReturnValue() << std::endl;
                    break;
                }

                if(time == timeout_HectSec) {
                    this->state = TIME_OUT_TRY_OUT;
                    std::cout << "Connect to "<<connector.ip << ":" <<connector.port << " failed: TIMEOUT." << std::endl;

                    if(connector.isRunning()) {
                        connector.quit();
                        if( ! connector.wait(500) ) {
                            connector.terminate();
                            if( ! connector.wait(3000) )
                                std::cout << "Terminating Connector Thread timeouted: " << __func__ << std::endl;
                        }
                    }
                }
            }

            if(this->state != STATE::TIME_OUT_TRY_OUT)
                break;
            else {
                if(connector.ConnectTo() != 0) {
                    this->state = STATE::UNDEFINED_ERROR;
                    std::cout << "Failed to start Connector Thread" << std::endl;
                    break;
                }
                std::cout << "WARNUNG: " << Try << ". Versuch von 3 bei TryConnect." << std::endl;
            }

        }


    }
    emit finished(this);
    */
}
