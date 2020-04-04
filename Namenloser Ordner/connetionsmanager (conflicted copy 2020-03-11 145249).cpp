#include "connetionsmanager.h"


ConnetionsManager::ConnetionsManager()
    : stop(false)
{
    server = new TCP_SERVER();
}

ConnetionsManager::~ConnetionsManager()
{
    stop = true;
    if(this->isRunning()) {
        //this->quit();
        if( ! this->wait(500) ) {
            //this->terminate();
            if( ! this->wait(10000) ) {
                std::cout << " Stop connectionmanager thread failed" << std::endl;
            }
        }
    }

    delete server;

    for( auto &e : Connections)
        delete e;
}

int ConnetionsManager::start_Thread(std::vector<ConnetionsManager::PC> computers)
{
    this->computers = computers;

    if(this->isRunning())
        return -1;
    this->start();
    return this->isRunning() ? 1 : 0;
}

#include <QApplication>

void ConnetionsManager::run()
{
    /*
     * 1. Start Server
     * 2. Create in Loop Threads, which try to connect(QThread, which can emit signals && timeout system)
     *  -> pushpack to vector
     * 3. Accept incomming connection with NOOOO reconnect request
     *
     *
     *Bei Start versucht jeder Mit allen eine Verbingung aufzubauen =>
     *
    */


    //Server Starten
        for (unsigned i = 0; i < 6; ++i) {
            if(i == 5)
                return;
            if( server->startListening(computers.at(i).port) != 0 ) {
                std::cout << "Start Server failed:" << server->getLastError() << std::endl;
               // return;
            } else {
                std::cout << "Connected to: " << computers.at(i).ip << ":" << computers.at(i).port << std::endl;
                computers.erase(computers.begin() + i);
                for ( auto &e : computers)
                    std::cout << " => Rest: " << e.ip << ":" << e.port << std::endl;
                break;
            }
        }


    //Start Try- Verbindungsaufbau
    for (unsigned pcID = 0; pcID < computers.size(); ++pcID) {
        TryConnector * newTryConnector = new TryConnector();
        if( newTryConnector->tryConnectTo(computers.at(pcID).ip, computers.at(pcID).port) ) {
            std::cout << "Tryconnect failed to: " << computers.at(pcID).ip << ":" << computers.at(pcID).port << std::endl;
        }
        //
        connect(newTryConnector, SIGNAL(finished(TryConnector*)), this, SLOT(TryConnectorFinished(TryConnector*)));
        connect(newTryConnector, SIGNAL(startConnecting(TryConnector*)), this, SLOT(TryConnectorStartedConnecting(TryConnector*)));

    }



    //Nur noch accepten
    while ( ! stop ) {
        CLIENT newCLIENT;
        if( server->acceptClient(newCLIENT) != 0) {
            std::cout << "accept failed:" << server->getLastError() << std::endl;
            continue;
        }
        Peer * newPeer = new Peer(newCLIENT);
        Connections.push_back(newPeer);

        std::cout << " -> Accpedted: " << newCLIENT.getIpAddress() << std::endl;
    }




}

void ConnetionsManager::TryConnectorFinished(TryConnector *who)
{
    disconnect(who, SIGNAL(finished(TryConnector*)), this, SLOT(TryConnectorFinished(TryConnector*)));
    emit showMSG(who->getState() == who->OK ? "OK" : "NON OK");
    //std::cout << who->getIp() << ": State: " << who->getState() << std::endl;
    std::cout << who->getIp() << ": lError: " << who->client.getLastError() << std::endl;

    if(who->getState() == who->OK) {
        Peer * newPeer = new Peer(who->client);
        Connections.push_back(newPeer);
    }

    delete who;
}

void ConnetionsManager::TryConnectorStartedConnecting(TryConnector *who)
{
    connect(who, SIGNAL(startConnecting(TryConnector*)), this, SLOT(TryConnectorStartedConnecting(TryConnector*)));
    emit showMSG(who->getState() == who->CONNECTING ? "Connecting...." + QString::fromStdString("who->client.getLastError()") : "NON OK...");

    //std::cout << "Starting connecting to: " << who->getIp() << std::endl;

}
