#include "game.h"

Game::Game(QWidget *parant)
    : QGraphicsView(parant)
{
    //create scene
    scene = new QGraphicsScene(this);
    //set scene
    this->setScene(scene);

    //Create Player
    player = new Player(scene);

    //create enemyManager with Acces on Scene
    enemyManager = new EnemyManager(scene);

    //create minimap
    miniMap = new QGraphicsRectItem();

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    Koords = new QGraphicsSimpleTextItem();
    connect(this->scene, SIGNAL(sceneRectChanged(QRectF)), this, SLOT(sceneRectChanged(QRectF)));
    Koords->setBrush(Qt::blue);

    achsen[0].setLine(-1000, 0, 1000, 0);
    achsen[1].setLine(0, -1000, 0, 1000);
    scene->addItem(&achsen[0]);
    scene->addItem(&achsen[1]);
    ///achsen[0].setPen(QPen(Qt::white));
    ///achsen[1].setPen(QPen(Qt::white));
    achsen[0].setPen(QPen(Qt::white, 3));

    scene->addItem(&achsen[2]);
    //achsen[2].setPen(QPen(Qt::white));

    this->setMouseTracking(true);
    MouseKoords = new QGraphicsSimpleTextItem();
    scene->addItem(MouseKoords);
    ///MouseKoords->setBrush(Qt::white);



    //this->scene->setMinimumRenderSize(100);
    scene->setSceneRect(0, 0, 500,  500);


    mapBorder = new QGraphicsRectItem();
    mapBorder->setRect(-500, -500, 4000, 4000);
    mapBorder->setPen(QPen(Qt::black, 1000));
    scene->addItem(mapBorder);


    //this->setBackgroundBrush(QBrush(QColor(Qt::gray)));
   setBackgroundBrush(QImage(":/wormio_game/Ressources/slitherio_background_543a4c3cc8.jpg"));
    scene->addItem(Koords);



    //mini map:
    scene->addItem(miniMap);
    miniMap->setPen(QPen(Qt::white, 3));
    miniMap->setRect(0, 0, 200, 200);


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
    player->speed ++;
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
