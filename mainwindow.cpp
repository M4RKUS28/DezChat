#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), manager(new ConnetionsManager),timerForWarningMSG(0), wrongClientCountCounter(0), oldPrintStyle(false)
{
    ui->setupUi(this);

    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor::fromRgb(238, 255, 255, 255));
    this->setPalette(pal);

    connect(manager, SIGNAL(showMSG(QString)), this, SLOT(printInfo(QString)));
    connect(manager, SIGNAL(ConnectionsListChanged()), this, SLOT(updateClientList()));
    connect(manager, SIGNAL(sendMessageWeiterLeitung(Peer *, QString)), this, SLOT(recvedMSG(Peer *,QString)));


    std::vector<ConnetionsManager::PC> pcs;
    bool Port_Home_Version = true;

    if(/* DISABLES CODE: Schul-Version */ Port_Home_Version) {
        for (unsigned short i = 0; i < 32; ++i)
            pcs.push_back(ConnetionsManager::PC("127.0.0.1", 5000 + i));
    } else {
        pcs.push_back(ConnetionsManager::PC("SERVER_PORT", 5000));
        for (int i = 0; i < 35; ++i) {
            pcs.push_back(ConnetionsManager::PC("cr01-pc" + std::string((i < 10) ? "0" : "") + std::to_string(i), 5000));
            //pcs.push_back(ConnetionsManager::PC("CR02-PC" + std::string((i < 10) ? "0" : "") + std::to_string(i), 5000));
        }
        //pcs.push_back(ConnetionsManager::PC("DESKTOP-GK2CR51", 5000));

    }

    if( ! manager->start_Thread(Port_Home_Version, pcs) )
        exit(-1);
    updateClientList();

    timerID = startTimer(100); // 100


    //------------------>Game View

    game = new Game(QSize(1200, 800), this);
    connect(game, SIGNAL(wantLeaveGame()), this, SLOT(leaveGame()));

    ui->chat->addMsg("<i><b><font size=\"12\"><span style=\"color:#5f0\">Spiele Worm.io indem du 'lol' eingibst!</span><</font></b></i>\n", Qt::AlignCenter);

}

MainWindow::~MainWindow()
{
    std::cout << "~MainWindow()" << std::endl;
    killTimer(timerID);
    delete manager;
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent * e)
{
    manager->sendtoAllPeers("PING=" + QString::number(manager->getConnectionList().size()) );
    timerForWarningMSG++;
}


void MainWindow::printInfo(QString str)
{
    ui->chat->addMsg("<PROGRAM>: " + str, Qt::AlignCenter, "gray");
}

void MainWindow::recvedMSG(Peer *who, QString msg)
{
    //std::cout << "Recved: " << msg.toStdString() << std::endl;

    std::string what = msg.toStdString().substr(0, msg.toStdString().find("="));
    std::string value = msg.toStdString().substr(msg.toStdString().find("=") + 1);

    if(what == "PING") {
        if(manager->getConnectionList().size() < static_cast<size_t>(atoll(value.c_str())) )
            wrongClientCountCounter++;
        if(timerForWarningMSG > 100 && (timerForWarningMSG = 0) == 0)
            if(wrongClientCountCounter > 10 && (wrongClientCountCounter = 0) == 0)
                 ui->chat->addMsg("<WARNUNG> Anzahl Verbindungen stimmt nicht überein...Bitte Starte das Programm neu.", Qt::AlignCenter, "orange" );

     } else if(what == "MSG") {
        if(oldPrintStyle)
            ui->chat->addMsg( who->getFullName() + " " + QString::fromStdString(value), Qt::AlignLeft, nullptr, who );
        else
            ui->chat->addSenderNameAndMSG(who, QString::fromStdString(value));

    } else if(what == "JOINED") {
        who->setName(value);
        ui->chat->addMsg( who->getFullName() + " ist dem Chat beigetreten.", Qt::AlignCenter, "gray" );
        emit updateClientList();

    } else if(what == "LEAVED") {
        ui->chat->addMsg( QString::fromStdString(value) + " hat den Chat verlassen.", Qt::AlignCenter, "gray" );

    } else if(what == "PORT") {
        who->setConnectedToPort(static_cast<unsigned short>(stoi(value)));
        updateClientList();

    }  else if(what == "PRIVATE_MSG") {
        size_t pos1 = value.find("#");
        if(pos1 == std::string::npos) {
            std::cout << "-> Error: PRIVATE_MSG_MESSAGE is invalid: no # found" << std::endl;
            return;
        }
        size_t pos2 = value.find("#", pos1 + 1);
        if(pos2 == std::string::npos) {
            std::cout << "-> Error: PRIVATE_MSG_MESSAGE is invalid: no second # found" << std::endl;
            return;
        }

        std::string toWho = value.substr(pos1 + 1, pos2 - pos1 - 1);
        value.erase(pos1, pos2 +1);

        if(oldPrintStyle)
            ui->chat->addMsg( "</MSG> " + who->getFullName() + " -> " + QString::fromStdString(toWho) + ": " + QString::fromStdString(value), Qt::AlignLeft, nullptr, who);
        else
            ui->chat->addSenderNameAndMSG(who, QString::fromStdString(value),"  > flüstert " + QString::fromStdString(toWho) +" zu" + ((toWho == "dir") ? "" : " (ADMIN CATCHED)"));

    } else if(what == "JOIN_TIME") {
        who->setJoinTime(value);

    } else if(what == "DEBUG") {
        ui->chat->addMsg(QString::fromStdString("<DEBUG>: " + value), Qt::AlignCenter, "gray" );

    } else if(what == "VERSION") {
        if( stoi(value) != VERSION ) {
            ui->chat->addMsg("<ERROR> Falsche Programmversion: Client " + who->getFullName() + ": '" +
                              QString::fromStdString( value + "' ungleich eigener: '" + std::to_string(VERSION) + "'"), Qt::AlignCenter, "red" );

            if(stoi(value) < VERSION) {
                std::cout << " -> Selber höhere Version => Closesocket to " << who->getFullName().toStdString() << std::endl;
                if(who->isConnected()) {
                    if(who->closeSocket() != 0)
                        perror("Closesocket failed.");
                } else
                    perror("couldn't Closesocket from wrong version client: not connected");
            } else
                std::cout << " -> Selber niedrigere Version => Wait for Closesocket from " << who->getFullName().toStdString() << std::endl;
        }

    } else if(what == "") {
        std::cout << "INVALID MSG: '" << msg.toStdString() << "'" << std::endl;

    } else if(what == "adminstatus") {
        if (value == "true") {
            who->set_isAdmin(true);
            if(manager->isAdmin) {
                ui->chat->addMsg( who->getName() + " hat sich zum Admin befördert.", Qt::AlignCenter, "orange");
            }
        } else if ( value == "gofalse") {
            who->set_isAdmin(false);
            if(manager->isAdmin) {
                ui->chat->addMsg( who->getName() + " hat sich vom Admin degratiert.", Qt::AlignCenter, "orange");
            }
        }
    } else {
        std::cout << "ERROR: UNKNOWN MSG: '" << msg.toStdString() << std::endl;
        ui->chat->addMsg(QString::fromStdString("<ERROR> Unbekannter Nachrichtentype: '"+ what +"' mit value='"  + value + "'"), Qt::AlignCenter, "red" );
    }
}

void MainWindow::updateClientList()
{
    this->ui->Connections->clear();

    this->ui->Connections->addItem(QString("<Du Selbst>"));

    for( auto &e : manager->getConnectionList())
        this->ui->Connections->addItem(e->getFullName());
}

void MainWindow::on_inputLine_returnPressed()
{
    QString line = ui->inputLine->text();
    ui->inputLine->clear();

    if(  line == "lol" ) {
        this->joinGame();
        return;



    } else if(line.startsWith('/')) {
        ui->chat->addMsg("Executing: '" + line + "'...");

        if(line.startsWith("/clear", Qt::CaseInsensitive)) {
            this->ui->chat->clear();

        } else if (line.startsWith("/status", Qt::CaseInsensitive)) {
            for( unsigned i = 0; i < manager->getConnectionList().size(); ++i ) {
                this->ui->chat->addMsg(manager->getConnectionList().at(i)->getFullName() + " Color: "
                                       + ui->chat->getColorByPeer(manager->getConnectionList().at(i)).name() + " Jointime: "
                                       + manager->getConnectionList().at(i)->getJoinTime() + " VectorIndex: " + QString::number(i));
            }

        } else if (line.startsWith("/-", Qt::CaseInsensitive)) {


        } else if (line.startsWith("/msg <", Qt::CaseInsensitive)) {
            line.remove(0, 5);

            int pos = line.indexOf("> ");
            if(pos == -1)
                return ui->chat->addMsg("<Console>: Error: Ungültiger Client: '" + line + "'", Qt::AlignCenter, "red");


            QString recver = line.left(pos + 1);
            line.remove(0, pos + 2);

            bool ok = false;
            for( auto &c : manager->getConnectionList() ) {
                if( recver == c->getFullName() || c->isAdmin() ) {
                                                            //Wenn empfänger überinstimmt auch wenn dieser admin ist, dann verwende "dir"
                    c->send_to( "PRIVATE_MSG=#" + QString( ( (recver == c->getFullName()) ? "dir" : recver ) ) + "#" + line );
                    //Nur wenn ein nicht admin empfänger gefunden worden ist, setze on auf true, sonst kommt keine "kein passender client" nachricht
                    if( recver == c->getFullName() )
                        ok = true;
                }
            }
            if( !ok )
                return ui->chat->addMsg("<Console>: Error: Unbekannter Client: '" + recver + "'", Qt::AlignCenter, "red");

        } else if (line.startsWith("/setPrintStyle ", Qt::CaseInsensitive)) {
            if(line.length() > 15) {
                if(line[15] == '1')
                    oldPrintStyle = true;
                else if (line[15] == '2')
                    oldPrintStyle = false;
                else
                    ui->chat->addMsg("<Console>: Error: Unbekannter StyleSheet: " + QString(line.at(15)), Qt::AlignCenter, "red");
            } else
                ui->chat->addMsg("<Console>: Error: StyleSheet ID fehlt.", Qt::AlignCenter, "red");

        } else if (line.startsWith("/help", Qt::CaseInsensitive)) {
            ui->chat->addMsg("Liste:\t/help\t\t-> Gibt diese Liste aus.");
            ui->chat->addMsg("\t/clear\t\t-> Löscht den Chat (nur bei dir).");
            ui->chat->addMsg("\t/msg <Client> <msg>\t-> Privatnachricht an diesen Client");
            ui->chat->addMsg("\t\t\t     Tip: Doppelklick auf den Namen rechts in der Liste.");
            ui->chat->addMsg("\t/setPrintStyle <1/2>\t-> AusgabeAussehen: Style 1 oder 2.");


        } else if (line.startsWith("/login ", Qt::CaseInsensitive)) {
            line.remove(0, 7);
            if (line.contains("admin")){
                manager->sendtoAllPeers("adminstatus=true");
                manager->isAdmin = true;
                ui->chat->addMsg("<Console> Adminstatus confirmed", Qt::AlignCenter, "red");
            }

        } else if (line.startsWith("/logout", Qt::CaseInsensitive)) {
            if(manager->isAdmin) {
                manager->sendtoAllPeers("adminstatus=gofalse");
                manager->isAdmin = false;
                ui->chat->addMsg("<Console> Adminstatus removed", Qt::AlignCenter, "red");
            }

        } else {
            ui->chat->addMsg("<Console>: Error: Unbekannter Befehl: '" + line + "'", Qt::AlignCenter, "red");
        }


    } else {
        if(oldPrintStyle)
            ui->chat->addMsg("<ICH>: " + line, Qt::AlignRight );
        else
            ui->chat->addSenderNameAndMSG(nullptr, line);
        //--------------> SEND MESSAGES
        manager->sendtoAllPeers("MSG=" + line);
    }
}

void MainWindow::on_Connections_itemDoubleClicked(QListWidgetItem *item)
{
    //std::cout << "Item gedoppelklickt: "<<item->text().toStdString() << std::endl;
    ui->inputLine->setText("/msg " + item->text() + " ");
}

void MainWindow::joinGame()
{
    ui->chat->addMsg(QTime::currentTime().toString() + ": Wechsle in die Spiel Lobby...", Qt::AlignCenter, Qt::gray);
    manager->sendtoAllPeers("MSG=...spielt jetzt Worm.io !");


    this->setFixedSize(1200, 800);

    //set chat things invisible
    this->ui->chat->hide();
    this->ui->Connections->hide();
    this->ui->inputLine->hide();
    this->ui->label->hide();


    //set game visible
    this->game->show();
    this->game->setFocus();
    game->showGameLobby();


}

void MainWindow::leaveGame()
{
    this->setFixedSize(1000, 550);

    //make game invisible
    this->game->hide();



    //set old things visible
    this->ui->chat->show();
    this->ui->Connections->show();
    this->ui->inputLine->show();
    this->ui->label->show();
}

