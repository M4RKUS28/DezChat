#include "player.h"

Player::Player(QGraphicsScene *scene)
    : scene(scene), moveLastTimer(0)
{
    // init objects
    moveTimer = new QTimer(this);
    moveTimer->setInterval(20);
    connect(moveTimer, SIGNAL(timeout()), this, SLOT(move()));

    //setup setting
    setSpeed(initSpeed);
    resetData();

}

void Player::resetData()
{
    //stop timer:
    moveTimer->stop();

    //reset settings:
    radius = initRadius;
    scale = initScale;

    //rm old Worm if exits:
    for( auto &e : Worm)
        delete e;
    Worm.clear();


    //init worm
    for(int i = 0; i < initLength; i++) {
        WormPart * wp = new WormPart(radius, QBrush(Qt::green), scale);
        Worm.prepend( wp );
        scene->addItem( wp );
    }


}

void Player::start()
{
    resetData();
    moveTimer->start();
}

void Player::stop()
{





}

#include <iostream>
#include <QApplication>

void Player::rotateHead(QPointF mousePos, QGraphicsLineItem * debugLine)
{

    QLineF ln( QPointF(this->Worm.at(0)->x(), this->Worm.at(0)->y()), mousePos );
    if(debugLine)
        debugLine->setLine(ln);





    this->Worm.at(0)->setRotation(-1* ln.angle() + 90  );

    //this->Worm.at(0)->setTransformOriginPoint( 0 , 0 ); // head->rect().x() + radius, head->rect().y() + radius
}

void Player::move()
{
    //std::cout << moveLastTimer << std::endl;
    moveLastTimer++;
    if(moveLastTimer >= moveLastTimerSeqence) {
        moveLastTimer = 0;

        //move last behind vect(0) && set pos from vec(0) && move vect 0
        Worm.move(Worm.size() - 1, 1);
        Worm.at(1)->setPos(Worm.at(0)->pos().x() , Worm.at(0)->pos().y()  );
        Worm.at(1)->setRotation(Worm.at(0)->rotation() );

    }

    ///QLineF ln(Worm.at(1)->getCenter(), Worm.at(0)->getCenter());
    ///Worm.at(1)->setPos( Worm.at(1)->getCenter().x() + ln.dx(), Worm.at(1)->getCenter().x() + ln.dy() );


    //move player

    double theta = Worm.at(0)->rotation() - 90;

    double dy = speed * qSin(qDegreesToRadians(theta));
    double dx = speed * qCos(qDegreesToRadians(theta));

    Worm.at(0)->setPos( Worm.at(0)->pos().x() + dx, Worm.at(0)->pos().y() + dy );

    scene->setSceneRect(Worm.at(0)->sceneBoundingRect());
}

void Player::setSpeed(double value)
{
    speed = value;
    if(speed)
        moveLastTimerSeqence = wormPartDistance / speed;
}

void Player::setRadius(double r)
{
    radius = r;
    for( auto &e : Worm)
        e->updateRadius( radius );
}

void Player::increaseWorm()
{
    length++;
    WormPart * wp = new WormPart(radius, QBrush(Qt::darkGreen), scale);
    Worm.append( wp);
    scene->addItem( wp );


}

void Player::addPoint()
{
    points++;
    setSpeed( speed + 0.1 );
    increaseWorm();
    setRadius( radius + 0.1 );
}

void Player::setScale(double scale)
{
    this->scale = scale;
}



int Player::getSpeed() const
{
    return speed;
}

size_t Player::getPoints() const
{
    return points;
}

int Player::getLength() const
{
    return length;
}



