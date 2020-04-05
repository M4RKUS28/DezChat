#include "connetionsmanager.h"


ConnetionsManager::ConnetionsManager()
    : stop(false)
{
    server = new TCP_SERVER();
    server->autoCleanUpInTheEnd = false;
}

ConnetionsManager::~ConnetionsManager()
{
    std::cout << "~ConnetionsManager()" << std::endl;

    stop = true;
    if(this->isRunning()) {
        this->quit();
        if( ! this->wait(500) ) {
            this->terminate();
            if( ! this->wait(3000) )
                std::cout << " Stop connectionmanager thread failed" << std::endl;
        }
    }

    //stop server
    if( server->stopListening() != 0 )
        std::cout << "Stop server failed: " << server->getLastError() << std::endl;

    //delete server
    delete server;

    //delete all connections
    for( auto &e : Connections)
        delete e;//e->deleteLater();
}

void ConnetionsManager::sendtoAllPeers(QString msg)
{
    //send msg to all peers
    for (auto &e : Connections)
        e->send_to(msg);
}


std::vector<Peer *> &ConnetionsManager::getConnectionList()
{
    return Connections;
}



int ConnetionsManager::start_Thread(bool PORT_BASED, std::vector<ConnetionsManager::PC> computers)
{
    this->computers = computers;
    this->PORT_BASED = PORT_BASED;

    if(this->isRunning())
        return -1;
    this->start();
    return this->isRunning() ? 1 : 0;
}

void ConnetionsManager::run()
{
    //Start Server
    if(startServer() != 0)
        return emit showMSG("Fatal Error in startServer()");

    //TryConnnect to All Peers in List
    else if(startAllTryConnector() != 0)
        return emit showMSG("Fatal Error in startAllTryConnector()");

    //Accept all new Peers
    while ( ! stop )
        if(acceptClient() != 0)
            return emit showMSG("Fatal Error in acceptClient()");
}

int ConnetionsManager::startAllTryConnector()
{
    for (auto &pc : computers) {
       // std::cout << "Start tryconnect to: " << pc.ip << ":" << pc.port << std::endl;

        TryConnector * newTryConnector = new TryConnector();
        newTryConnector->deleteLater(); ///<--------------------------------Test--------------------------------------------
        connect(newTryConnector, SIGNAL(finished(TryConnector*)), this, SLOT(TryConnectorFinished(TryConnector*)));

        if( newTryConnector->tryConnectTo(pc.ip, pc.port) != 0 ) {
            perror("Start TryThread failed");
            return -1;
        }
    }
    return 0;
}

int ConnetionsManager::startServer()
{
    if(/* nicht Port based => IP_BASED*/ PORT_BASED == false ) {
        while ( server->startListening(computers.at(0).port, 3, 0) != 0 ) {
            emit showMSG("Error: Server konnte nicht gestartet werden: '"
                         + QString::fromStdString(server->getLastError())
                         + "' -> Läuft das Program vielleicht bereits? - Retry in 3 Sekunden.");
            sleep(3);
        }
        emit showMSG("SERVER gestartet: @" + QString::fromStdString(server->getHostName()) + ":" +  QString::number(computers.at(0).port));

        //clear
        computers.erase(computers.begin());
        std::string hostname = server->getHostName();

        for ( unsigned i = 0; i < computers.size(); ++i)
            if(computers.at(i).ip == hostname)
                computers.erase(computers.begin() + i);

    } else {
        //Server Starten
        for ( unsigned i = 0; i < computers.size(); ++i) {
            if( server->startListening(computers.at(i).port) != 0 ) {
                std::cout << "Start Server failed:" << server->getLastError() << std::endl;
                if(i == computers.size() -1) {
                    std::cout << "Error: Konnte keinen einzigen Server starten." << std::endl;
                    return -1;
                }
            } else {
                emit showMSG("SERVER: " + QString::fromStdString(computers.at(i).ip) + ":" + QString::number(computers.at(i).port));
                std::cout << "Startet server on: " << computers.at(i).ip << ":" << computers.at(i).port << std::endl;
                computers.erase(computers.begin() + i);
                break;
            }
        }
    }
    return 0;
}

int ConnetionsManager::acceptClient()
{
    CLIENT newCLIENT;
    newCLIENT.autoCleanUpInTheEnd = false; // not auto clossocket in destruktor

    for (int Try = 1; Try <= 3; ++Try) {
        if( server->acceptClient(newCLIENT) != 0) {
            std::cout << "Accept Client failed: " << server->getLastError() << std::endl;
            emit showMSG("Error: Accept failed:" + QString::fromStdString(server->getLastError()) + " Retry...");
        } else
            break;
        if(Try == 3) {
            std::cout << "Fatal Error: Out of trys: Accept Client failed 3 times: " << server->getLastError() << std::endl;
            emit showMSG("Fatal Error: Out of trys: Accept Client failed 3 times. Stop...");
            sleep(3);
            return -1;
        }
    }

    //after terminate in destructer exit boost
    if(stop) {
        if( newCLIENT.closeSocket() != 0)
            std::cout << "Boost exit in acceptClient() failed." << std::endl;
        return -1;
    }


    Peer * newPeer = new Peer(newCLIENT);
    Connections.push_back(newPeer);
    //Connect connectionless peer per signal slot system
    connect(newPeer, SIGNAL(recvdMessage(Peer*,QString)), this, SLOT(MessageWeiterLeitung(Peer*,QString)));
    connect(newPeer, SIGNAL(recvdFailed(Peer*)), this, SLOT(PeerConnectionGetsError(Peer*)));
    connect(newPeer, SIGNAL(sendFailed(Peer*)), this, SLOT(PeerConnectionGetsError(Peer*)));

    if( newPeer->startReciver() != 0 ) {
        std::cout << "Start recverThread from peer failed." << std::endl;
        delete newPeer;//->deleteLater();
        return -1;
    }

    //Send name, port, ip ...
    newPeer->sendInfoData(server->getListeningPort());

    emit ConnectionsListChanged();
    //emit showMSG(QString::fromStdString(" -> New Client: " + newCLIENT.getIpAddress() + ":" + std::to_string(newCLIENT.getPort())));
    std::cout << " -> Accpedted: " << newCLIENT.getIpAddress() << std::endl;

    return 0;
}


void ConnetionsManager::TryConnectorFinished(TryConnector *who)
{
    disconnect(who, SIGNAL(finished(TryConnector*)), this, SLOT(TryConnectorFinished(TryConnector*)));

    if(who->getState() == OK) {
        std::cout << " -> Connected to: " << who->getIp() << ":" << who->getPort() << std::endl;

        Peer * newPeer = new Peer(who->client(), who->getIp(), who->getPort());
        Connections.push_back(newPeer);
        connect(newPeer, SIGNAL(recvdMessage(Peer*,QString)), this, SLOT(MessageWeiterLeitung(Peer*,QString)));
        connect(newPeer, SIGNAL(recvdFailed(Peer*)), this, SLOT(PeerConnectionGetsError(Peer*)));
        connect(newPeer, SIGNAL(sendFailed(Peer*)), this, SLOT(PeerConnectionGetsError(Peer*)));

        if( newPeer->startReciver() != 0 ) {
            std::cout << "Start recverThread from peer failed." << std::endl;
            delete newPeer; //newPeer->deleteLater();
            delete who;     //->deleteLater();
            return;
        }

        //Send name, port, ip ...
        newPeer->sendInfoData(server->getListeningPort());

        emit ConnectionsListChanged();
        //emit showMSG(QString::fromStdString(" -> New Client: " + who->getIp() + ":" + std::to_string(who->getPort())));
    } else if (who->getState() == STATE::TIME_OUT_TRY_OUT) {
        std::cout << "-> Connect to " << who->getIp() << ":" << who->getPort() << " timedout." << std::endl;

    } else if(who->getState() == STATE::UNDEFINED) {
        std::cout << "-> Thread for Connecting to " << who->getIp() << ":" << who->getPort() << " never started." << std::endl;

    } else {
        //std::cout << "-> Connect to " << who->getIp() << ":" << who->getPort() << " failed." << std::endl;
    }

    delete who;//->deleteLater();
}

void ConnetionsManager::PeerConnectionGetsError(Peer *who)
{
    //Disconnedt ist dafür da, dass wenn recv und send failed, nicht 2 mal diese Funtkion ausgeführt wird, was sonst oft passieren würde
    disconnect(who, SIGNAL(recvdMessage(Peer*,QString)), this, SLOT(MessageWeiterLeitung(Peer*,QString)));
    disconnect(who, SIGNAL(recvdFailed(Peer*)), this, SLOT(PeerConnectionGetsError(Peer*)));
    disconnect(who, SIGNAL(sendFailed(Peer*)), this, SLOT(PeerConnectionGetsError(Peer*)));

    //emit showMSG( "@" + who->getName() + " hat den Chat verlassen.");
    std::cout << " -> PeerConnectionGetsError (Disconnect Client): " << who->getFullName().toStdString() << std::endl;
    emit sendMessageWeiterLeitung(nullptr, "LEAVED=" + who->getFullName());

    int check = 0;
    for (unsigned i = 0; i < Connections.size(); ++i) {
        if(Connections.at(i) == who && (++check)) {
            Connections.erase(Connections.begin() + i);
            //std::cout << "DEBUG: Delete Peer, cause getError, with Port: " << who->getPort() << std::endl;
            delete who;//->deleteLater();
        }
    }
    //check if peer was removed
    if(check != 1)
        std::cerr << "FATAL ERROR: Got Error signal from Peer, who was not one time in ConnectionList: " << check << " schould be 1" << std::endl;

    emit ConnectionsListChanged();
}


void ConnetionsManager::MessageWeiterLeitung(Peer *who, QString msg)
{
    emit sendMessageWeiterLeitung(who, msg);
}
