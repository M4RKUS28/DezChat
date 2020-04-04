#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), manager(new ConnetionsManager)
{
    ui->setupUi(this);
    connect(manager, SIGNAL(showMSG(QString)), this, SLOT(printInfo(QString)));
    connect(manager, SIGNAL(ConnectionsListChanged()), this, SLOT(updateClientList()));


    std::vector<ConnetionsManager::PC> pcs;
    pcs.push_back(ConnetionsManager::PC("127.0.0.1", 4200));
    pcs.push_back(ConnetionsManager::PC("127.0.0.1", 4201));
    pcs.push_back(ConnetionsManager::PC("127.0.0.1", 4202));
    pcs.push_back(ConnetionsManager::PC("127.0.0.1", 4203));
    pcs.push_back(ConnetionsManager::PC("127.0.0.1", 4204));
    pcs.push_back(ConnetionsManager::PC("127.0.0.1", 4205));

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
    ui->chat->append(str + "\n");
    std::cout << "add: " << str.toStdString() << std::endl;
}

void MainWindow::updateClientList()
{
    this->ui->Connections->clear();
    for( auto &e : manager->Connections)
        this->ui->Connections->addItem(QString("<PeerName>@" + QString::fromStdString(e->getIp()) + ":" + QString::number(e->getPort())));
}

void MainWindow::on_inputLine_returnPressed()
{
    printInfo(ui->inputLine->text());
    ui->inputLine->clear();
}
