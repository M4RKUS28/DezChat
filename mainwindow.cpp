#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), manager(new ConnetionsManager),timerForWarningMSG(0), wrongClientCountCounter(0), oldPrintStyle(false)
{
    ui->setupUi(this);

    connect(manager, SIGNAL(showMSG(QString)), this, SLOT(printInfo(QString)));
    connect(manager, SIGNAL(ConnectionsListChanged()), this, SLOT(updateClientList()));
    connect(manager, SIGNAL(sendMessageWeiterLeitung(Peer *, QString)), this, SLOT(recvedMSG(Peer *,QString)));


    std::vector<ConnetionsManager::PC> pcs;
    bool Port_Home_Version = true;

    if(/* DISABLES CODE: Schul-Version */ Port_Home_Version) {
        for (unsigned short i = 0; i < 32; ++i)
            pcs.push_back(ConnetionsManager::PC("127.0.0.1", 4100 + i));
    } else {
        pcs.push_back(ConnetionsManager::PC("SERVER_PORT", 4000));
        for (int i = 0; i < 34; ++i)
            pcs.push_back(ConnetionsManager::PC("CR01-PC" + std::string((i < 10) ? "0" : "") + std::to_string(i), 4000));
    }


    if( ! manager->start_Thread(Port_Home_Version, pcs) )
        exit(-1);
    updateClientList();

    timerID = startTimer(100);
}

MainWindow::~MainWindow()
{
    std::cout << "~MainWindow()" << std::endl;
    killTimer(timerID);
    delete manager;
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent *)
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
                 ui->chat->addMsg(QString::fromStdString("<WARNUNG> Anzahl Verbindungen stimmt nicht überein...Bitte Starte das Programm neu."), Qt::AlignCenter, "orange" );


     } else if(what == "MSG") {
        //ui->chat->append(QString::fromStdString("<" + who->name + ">: " + value) );
        //std::cout << "Recied MSG: " << value << std::endl;
        if(oldPrintStyle)
            ui->chat->addMsgOldStyle( who->getFullName() + " " + QString::fromStdString(value), who );
        else {
            ui->chat->addSenderName(who);
            ui->chat->addMsg(QString::fromStdString(value));
        }

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
        if(oldPrintStyle)
            ui->chat->addMsgOldStyle( "</MSG> " + who->getFullName() + QString::fromStdString(value), who );
        else {
            ui->chat->addSenderName(who, "  > flüstert dir zu");
            ui->chat->addMsg(QString::fromStdString(value), Qt::AlignLeft, Qt::darkGray);
        }

    } else if(what == "JOIN_TIME") {
        who->setJoinTime(value);

    } else if(what == "DEBUG") {
        ui->chat->addMsg(QString::fromStdString("<DEBUG>: " + value), Qt::AlignCenter, "gray" );

    } else if(what == "VERSION") {
        if(value != VERSION) {
            ui->chat->addMsg("<ERROR> Falsche Programmversion: Client " + who->getFullName() + ": '" +
                              QString::fromStdString( value + "' ungleich eigener: '" + VERSION + "'"), Qt::AlignCenter, "orange" );
            who->send_to("=" ); // SOLANGE KEIN PING PONG => UPDATE TCP
            if( who->closeSocket() != 0)
                perror("Closesocket failed.");
            return;
        }

    } else if(what == "") {
        std::cout << "INVALID MSG: '" << msg.toStdString() << "'" << std::endl;

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

    if(line.startsWith('/')) {
        ui->chat->addMsg("Executing: '" + line + "'...");

        if(line.startsWith("/clear", Qt::CaseInsensitive)) {
            this->ui->chat->clear();

        } else if (line.startsWith("/status", Qt::CaseInsensitive)) {
            for( unsigned i = 0; i < manager->getConnectionList().size(); ++i ) {
                this->ui->chat->addMsg(manager->getConnectionList().at(i)->getFullName() + " Color: "
                                       + ui->chat->getColorByPeer(manager->getConnectionList().at(i)).name() + " Jointime: "
                                       + manager->getConnectionList().at(i)->getJoinTime() + " VectorIndex: " + QString::number(i));
            }

        } else if (line.startsWith("/-1", Qt::CaseInsensitive)) {
            if(manager->getConnectionList().size())
                this->manager->getConnectionList().at(0)->closeSocket();

        } else if (line.startsWith("/msg <", Qt::CaseInsensitive)) {
            line.remove(0, 5);

            int pos = line.indexOf("> ");
            if(pos == -1) {
                ui->chat->addMsg("<Console>: Error: Ungültiger Client: '" + line + "'", Qt::AlignCenter, "red");
                return;
            }

            QString recver = line.left(pos + 1);
            line.remove(0, pos + 2);

            bool ok = false;
            for( auto &c : manager->getConnectionList() ) {
                if( recver == c->getFullName() ) {
                    c->send_to( "PRIVATE_MSG=" + line );
                    ok = true;
                }
            }
            if( !ok ) {
                ui->chat->addMsg("<Console>: Error: Unbekannter Client: '" + recver + "'", Qt::AlignCenter, "red");
                return;
            }


        } else if (line.startsWith("/setPrintStyle ", Qt::CaseInsensitive)) {
            if(line.length() > 15) {
                if(line[15] == '1')
                    oldPrintStyle = true;
                else if (line[15] == '2')
                    oldPrintStyle = false;
                else
                    ui->chat->addMsg("<Console>: Error: Unbekannter StyleSheet: " + QString(line.at(15)), Qt::AlignCenter, Qt::red);
            } else
                ui->chat->addMsg("<Console>: Error: StyleSheet ID fehlt.", Qt::AlignCenter, Qt::red);

        } else if (line.startsWith("/help", Qt::CaseInsensitive)) {
            ui->chat->addMsg("Liste:\t/help\t\t-> Gibt diese Liste aus.");
            ui->chat->addMsg("\t/clear\t\t-> Löscht den Chat (nur bei dir).");
            ui->chat->addMsg("\t/msg <Client> <msg>\t-> Privatnachricht an diesen Client");
            ui->chat->addMsg("\t\t\t     Tip: Doppelklick auf den Namen rechts in der Liste.");
            ui->chat->addMsg("\t/setPrintStyle <1/2>\t -> AusgabeAussehen: Style 1 oder 2.");


        } else {
            ui->chat->addMsg("<Console>: Error: Unbekannter Befehl: '" + line + "'", Qt::AlignCenter, "red");
        }


    } else {
        if(oldPrintStyle)
            ui->chat->addMsgOldStyle("<ICH>: " + line, nullptr, Qt::AlignRight );
        else {
            ui->chat->addSenderName(nullptr);
            ui->chat->addMsg(line, Qt::AlignRight);
        }
        manager->sendtoAllPeers("MSG=" + line);
    }
}




void MainWindow::on_Connections_itemDoubleClicked(QListWidgetItem *item)
{
    //std::cout << "Item gedoppelklickt: "<<item->text().toStdString() << std::endl;
    ui->inputLine->setText("/msg " + item->text() + " ");
}

