#include "player.h"

Player::Player(QGraphicsScene *scene)
    : scene(scene), isLiving(false)
{
    radius = 60;
    speed = 10;
    initLength = 100;

    //init worm
    for(int i = 0; i < initLength; i++) {
        WormPart * wp = new WormPart(radius);
        Worm.prepend( wp );
        scene->addItem( wp );
    }


    moveTimer = new QTimer(this);
    moveTimer->setInterval(20);
    connect(moveTimer, SIGNAL(timeout()), this, SLOT(move()));
    moveTimer->start();

    this->startTimer(20);
}

#include <iostream>
#include <QApplication>

void Player::timerEvent(QTimerEvent *event)
{

    /*
    //move last behind vect(0) && set pos from vec(0)
    //move vect 0
    Worm.move(Worm.size() - 1, 1);
    Worm.at(1)->setPos(Worm.at(0)->rect().x() + radius, Worm.at(0)->rect().y() + radius );

    //std::cout << "Worm: " << Worm.at(1)->rect().x()  << " " << Worm.at(1)->rect().y() << " - " << Worm.at(0)->rect().x()  << " " << Worm.at(0)->rect().y()  << std::endl;


    //move player
    auto & head = Worm.at(0);


    //std::cout << "Moved" << std::endl;
    double STEP_SIZE = speed;
    double theta = head->rotation() - 90;

    double dy = STEP_SIZE * qSin(qDegreesToRadians(theta));
    double dx = STEP_SIZE * qCos(qDegreesToRadians(theta));

    //this->moveBy(dx, dy);
    head->setRect(head->rect().x() + dx, head->rect().y() + dy, radius*2, radius*2);
    head->setTransformOriginPoint(head->rect().x() + radius, head->rect().y() + radius);


    //scene()->setSceneRect(scene()->sceneRect().x() - 1, 100, 100, 100);
    scene->setSceneRect(head->sceneBoundingRect());

    */

}

void Player::rotateHead(QPointF mousePos, QGraphicsLineItem * debugLine)
{

    QLineF ln(QPointF( this->Worm.at(0)->rect().x()  + this->radius  ,
                       this->Worm.at(0)->rect().y()  + this->radius  ),
              mousePos );

    debugLine->setLine(ln);

    this->Worm.at(0)->setRotation(-1* ln.angle()  + 90 );
    std::cout << this->Worm.at(0)->rect().x() << " " << this->Worm.at(0)->rect().y() << " - " << this->Worm.at(0)->rect().width() << " " << this->Worm.at(0)->rect().height() << std::endl;
}

void Player::move()
{
    //move last behind vect(0) && set pos from vec(0) && move vect 0
    Worm.move(Worm.size() - 1, 1);
    Worm.at(1)->setPos(Worm.at(0)->pos().x() + radius, Worm.at(0)->pos().y() + radius ); // <-- Kopf is gedreht, rest aber nicht ==> pos != pos => center == center
    ///QLineF ln(Worm.at(1)->getCenter(), Worm.at(0)->getCenter());
    ///Worm.at(1)->setPos( Worm.at(1)->getCenter().x() + ln.dx(), Worm.at(1)->getCenter().x() + ln.dy() );


    //move player

    auto & head = Worm.at(0);

    double STEP_SIZE = speed;
    double theta = head->rotation() - 90;

    double dy = STEP_SIZE * qSin(qDegreesToRadians(theta));
    double dx = STEP_SIZE * qCos(qDegreesToRadians(theta));

    //this->moveBy(dx, dy);
    head->setRect(head->pos().x() + dx, head->pos().y() + dy, radius*2, radius*2);
    //head->setPos( head->rect().x() + dx, head->rect().y() + dy );

    head->setTransformOriginPoint( head->getCenter() ); // head->rect().x() + radius, head->rect().y() + radius

    //set plkayer to mid
    scene->setSceneRect(head->rect());
}




