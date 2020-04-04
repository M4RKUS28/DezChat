#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&manager, SIGNAL(showMSG(QString)), this, SLOT(printInfo(QString)));


    std::vector<ConnetionsManager::PC> pcs;
    pcs.push_back(ConnetionsManager::PC("127.0.0.1", 4000));
    pcs.push_back(ConnetionsManager::PC("127.0.0.1", 4001));
    pcs.push_back(ConnetionsManager::PC("127.0.0.1", 4002));
    pcs.push_back(ConnetionsManager::PC("127.0.0.1", 4003));
    pcs.push_back(ConnetionsManager::PC("127.0.0.1", 4004));
    pcs.push_back(ConnetionsManager::PC("127.0.0.1", 4005));

    manager.start(pcs);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lineEdit_returnPressed()
{
    printInfo(ui->lineEdit->text());
}

void MainWindow::printInfo(QString str)
{
    ui->statusBar->showMessage(str, 4000);
}
