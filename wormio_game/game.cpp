#include "game.h"

Game::Game(QSize MainWindowSize, QWidget *parant)
    : QGraphicsView(parant)
{
    //Init Objects:
    //this:
    this->setMouseTracking(true);
    this->setGeometry( 10, 10, MainWindowSize.width() - 20, MainWindowSize.height() - 20 );
    //this->setFixedSize( MainWindowSize.width() - 20, MainWindowSize.height() - 20 );
    this->setBackgroundBrush(QImage(":/wormio_game/Ressources/slitherio_background_543a4c3cc8.jpg"));

    //scene
    scene = new QGraphicsScene(this);
    this->setScene(scene);
    scene->setSceneRect(0, 0, 500,  500);

    //mapBoarder
    mapBorder = new QGraphicsRectItem();
    mapBorder->setRect(-500, -500, 4000, 4000);
    mapBorder->setPos(0, 0);
    mapBorder->setPen(QPen(Qt::black, 1000));
    scene->addItem(mapBorder);

    //mini Map
    miniMap = new QGraphicsRectItem();
    scene->addItem(miniMap);

    miniMap->setRect(0, 0, 200, 200);
    std::cout << " maptoScene( " << this->pos().x() << ", " << this->pos().y() << " ) -> " << mapToScene(this->pos()).x() << ", " << mapToScene(this->pos()).y() << std::endl;
    miniMap->setPos(mapToScene(this->pos()).x() + this->width() - 10 - miniMap->rect().width() +280, this->mapToScene( this->pos() ).y() + 10);

    miniMap->setPen(QPen(Qt::white, 3));

    player = new Player(scene);
    enemyManager = new EnemyManager(scene);

    //Mouse Koord debug
    MouseKoords = new QGraphicsSimpleTextItem();
    MouseKoords->setBrush(Qt::white);
    scene->addItem(MouseKoords);

    //Connect Signals and Slots
    connect(this->scene, SIGNAL(sceneRectChanged(QRectF)), this, SLOT(sceneRectChanged(QRectF)));



    //create minimap
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    Koords = new QGraphicsSimpleTextItem();
    Koords->setBrush(Qt::blue);



    achsen[0].setLine(-1000, 0, 1000, 0);
    achsen[1].setLine(0, -1000, 0, 1000);
    scene->addItem(&achsen[0]);
    scene->addItem(&achsen[1]);
    achsen[1].setPen(QPen(Qt::white, 3));
    achsen[0].setPen(QPen(Qt::white, 3));

    scene->addItem(&achsen[2]);
    //achsen[2].setPen(QPen(Qt::white));

    scene->addItem(Koords);



}

#include <QMouseEvent>
#include <iostream>
#include <QGraphicsWidget>

void Game::mouseMoveEvent(QMouseEvent *event)
{
    player->rotateHead( mapToScene( event->pos() ), &achsen[2] );

    MouseKoords->setText("Mouse-Pos: x" + QString::number(event->x()) + " y=" + QString::number(event->y())
                     + "\n -> MaptoScene: x=" + QString::number(mapToScene(event->pos()).x()) + " + y=" + QString::number(mapToScene(event->pos()).y()) );
    MouseKoords->setPos(mapToScene(this->pos()).x() + 600, mapToScene(this->pos()).y() + 30);



}

void Game::mousePressEvent(QMouseEvent *event)
{
    player->boost( true );
}

void Game::mouseReleaseEvent(QMouseEvent *event)
{
    player->boost( false );
}



void Game::sceneRectChanged(const QRectF &rect)
{
    Koords->setText("SceneRect: x=" + QString::number(scene->sceneRect().x()) + " y=" + QString::number(scene->sceneRect().y())
                         + " w=" + QString::number(scene->sceneRect().x()) + " h=" + QString::number(scene->sceneRect().x())
                    + "\n RectPosMiddle: x=" + QString::number(player->Worm.at(0)->rect().x() + 50 ) + " y=" + QString::number( player->Worm.at(0)->rect().y() + 50 ) );

    Koords->setPos(mapToScene(this->pos()).x() + 600, mapToScene(this->pos()).y() );




    //re-setpos map:
        miniMap->setPos(mapToScene(this->pos()).x() + this->width() - 10 - 230, mapToScene(this->pos()).y() + 10);

}

void Game::joinGame()
{
    player->start();

}
