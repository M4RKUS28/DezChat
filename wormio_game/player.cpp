#include "player.h"

Player::Player(QGraphicsScene *scene)
    : scene(scene)
{
    WormPart * wp = new WormPart;
    Worm.append( wp );
    scene->addItem( Worm.front() );

    this->startTimer(100);

}

#include <qmath.h>


Player::WormPart::WormPart()
{
    setPixmap(QPixmap(":/wormio_game/Ressources/test.png"));
    setScale(0.5);
    setRotation(90);
    //this->setBrush(Qt::blue);

}

#include <iostream>

void Player::WormPart::timerEvent(QTimerEvent *ev)
{
    //std::cout << "Moved" << std::endl;
    double STEP_SIZE = 4;
    double theta = rotation() - 90;

    double dy = STEP_SIZE * qSin(qDegreesToRadians(theta));
    double dx = STEP_SIZE * qCos(qDegreesToRadians(theta));

    this->moveBy(dx, dy);

    //scene()->setSceneRect(scene()->sceneRect().x() - 1, 100, 100, 100);
    scene()->setSceneRect(this->sceneBoundingRect());

}
