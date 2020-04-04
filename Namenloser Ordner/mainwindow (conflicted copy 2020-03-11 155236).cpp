#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), manager(new ConnetionsManager)
{
    ui->setupUi(this);
    connect(manager, SIGNAL(showMSG(QString)), this, SLOT(printInfo(QString)));
    connect(manager, SIGNAL(ConnectionsListChanged()), this, SLOT(updateClientList()));
    connect(manager, SIGNAL(sendMessageWeiterLeitung(Peer *,QString)), this, SLOT(recvedMSG(Peer *,QString)));


    std::vector<ConnetionsManager::PC> pcs;
    pcs.push_back(ConnetionsManager::PC("127.0.0.1", 4000));
    pcs.push_back(ConnetionsManager::PC("127.0.0.1", 4001));
    pcs.push_back(ConnetionsManager::PC("127.0.0.1", 4002));
    pcs.push_back(ConnetionsManager::PC("127.0.0.1", 4003));
    pcs.push_back(ConnetionsManager::PC("127.0.0.1", 4004));
    pcs.push_back(ConnetionsManager::PC("127.0.0.1", 4005));

    if( ! manager->start_Thread(pcs) )
        exit(-1);
}

MainWindow::~MainWindow()
{
    delete manager;
    delete ui;
}


void MainWindow::printInfo(QString str)
{
    ui->chat->append("<PROGRAM>: " + str);
    std::cout << "add: " << str.toStdString() << std::endl;
}

void MainWindow::recvedMSG(Peer *who, QString msg)
{
    ui->chat->append(QString::fromStdString("<" + who->getName() + ">: ") + msg);

}

void MainWindow::updateClientList()
{
    this->ui->Connections->clear();
    for( auto &e : manager->Connections)
        this->ui->Connections->addItem(QString("<PeerName>@" + QString::fromStdString(e->getIp()) + ":" + QString::number(e->getPort())));
}

void MainWindow::on_inputLine_returnPressed()
{
    ui->chat->append("<ICH>: " + ui->inputLine->text());

    //sending....

    for (auto &e : manager->Connections) {
        if( e->send_to(ui->inputLine->text()) <= 0) {
            std::cout << "sendfailed: " << e->getLastError() << std::endl;
        }
    }

    ui->inputLine->clear();

}
