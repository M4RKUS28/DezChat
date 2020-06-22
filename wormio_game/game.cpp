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

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    Koords = new QGraphicsSimpleTextItem();
    scene->addItem(Koords);
    connect(this->scene, SIGNAL(sceneRectChanged(QRectF)), this, SLOT(sceneRectChanged(QRectF)));
    Koords->setBrush(Qt::white);

    achsen[0].setLine(-1000, 0, 1000, 0);
    achsen[1].setLine(0, -1000, 0, 1000);
    scene->addItem(&achsen[0]);
    scene->addItem(&achsen[1]);
    achsen[0].setPen(QPen(Qt::white));
    achsen[1].setPen(QPen(Qt::white));

    scene->addItem(&achsen[2]);
    achsen[2].setPen(QPen(Qt::white));

    this->setMouseTracking(true);
    MouseKoords = new QGraphicsSimpleTextItem();
    scene->addItem(MouseKoords);
    MouseKoords->setBrush(Qt::white);



    //this->scene->setMinimumRenderSize(100);
    scene->setSceneRect(0, 0, 500,  500);


    mapBorder = new QGraphicsPolygonItem();
    QVector<QPoint> ecken;

    ecken.append(QPoint(0, 0));
    ecken.append(QPoint(0, 3000));
    ecken.append(QPoint(3000, 3000));
    ecken.append(QPoint(3000, 0));
    ecken.append(QPoint(0, 0));
    ecken.append(QPoint(-1000, -1000));
    ecken.append(QPoint(-1000, 4000));
    ecken.append(QPoint(4000, 4000));
    ecken.append(QPoint(4000, -1000));
    ecken.append(QPoint(-1000, -1000));




    mapBorder->setPolygon(QPolygon(ecken));
    mapBorder->setBrush(Qt::black);
    scene->addItem(mapBorder);

}

#include <QMouseEvent>
#include <iostream>
#include <QGraphicsWidget>

void Game::mouseMoveEvent(QMouseEvent *event)
{
    if(event)
        MouseKoords->setText("Mouse-Pos: x" + QString::number(event->x()) + " y=" + QString::number(event->y())
                         + "\n -> MaptoScene: x=" + QString::number(mapToScene(event->pos()).x()) + " + y=" + QString::number(mapToScene(event->pos()).y()) );

    MouseKoords->setPos(mapToScene(this->pos()).x() + 600, mapToScene(this->pos()).y() + 10);


    //this->setBackgroundBrush(QBrush(QColor(Qt::gray)));
   setBackgroundBrush(QImage(":/wormio_game/Ressources/slitherio_background_543a4c3cc8.jpg"));
   setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);


    if(player->Worm.size() < 0 || !event)
       return;

    QLineF ln( player->Worm.at(0)->boundingRect().center(), mapToScene( event->pos() ));

    achsen[2].setLine(ln);

   player->Worm.at(0)->setRotation(-1* ln.angle() + 90);

}



void Game::sceneRectChanged(const QRectF &rect)
{
    Koords->setText("SceneRect: x" + QString::number(scene->sceneRect().x()) + " y=" + QString::number(scene->sceneRect().y())
                         + " w=" + QString::number(scene->sceneRect().x()) + " h=" + QString::number(scene->sceneRect().x()));

    Koords->setPos(mapToScene(this->pos()).x() + 600, mapToScene(this->pos()).y() - 10);

    mouseMoveEvent(nullptr);

}
