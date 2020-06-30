#include "player.h"

Player::Player(QGraphicsScene *scene)
    : scene(scene), moveLastTimer(0)
{
    // init objects

    scoreText = new QGraphicsSimpleTextItem();
    scoreText->setBrush(QColor(Qt::white));
    scene->addItem(scoreText);

    lengthText = new QGraphicsSimpleTextItem();
    lengthText->setBrush(QColor(Qt::white));
    scene->addItem(lengthText);

    thinknessText = new QGraphicsSimpleTextItem();
    thinknessText->setBrush(QColor(Qt::white));
    scene->addItem(thinknessText);


    moveTimer = new QTimer(this);
    moveTimer->setInterval(20);
    connect(moveTimer, SIGNAL(timeout()), this, SLOT(move()));

    //setup setting
    setSpeed(initSpeed);
    resetData();

}

void Player::resetData()
{

    moveTimer->stop();

    //rm old Worm:
    for( auto e : Worm) {
        scene->removeItem(e);
        delete e;
    }
    Worm.clear();


    //reset settings:
    setSpeed(initSpeed); // speed & moveLastTimerSeqence
    radius = initRadius;
    scale = initScale;
    moveLastTimer = 0;
    length = initLength;
    doBoost = false;

    points = 0;

    //init worm
    for(int i = 0; i < length; i++) {
        WormPart * wp = new WormPart(radius, QBrush(Qt::green), scale);
        Worm.prepend( wp );
        scene->addItem( wp );
    }


}

void Player::start()
{
    resetData();
    moveTimer->start();

    //...
    for( int i = 0; i < 100; i++)
        addPoint();

}

void Player::stop()
{

    moveTimer->stop();

    //rm old Worm:
    for( auto e : Worm) {
        scene->removeItem(e);
        delete e;
    }
    Worm.clear();


}


void Player::rotateHead(QPointF mousePos, QGraphicsLineItem * debugLine)
{

    QLineF ln( QPointF(this->Worm.at(0)->x(), this->Worm.at(0)->y()), mousePos );
    if(debugLine)
        debugLine->setLine(ln);


    //Rotate only slownly to the mouse, not imideatly


    this->Worm.at(0)->setRotation(-1* ln.angle() + 90  );

    //this->Worm.at(0)->setTransformOriginPoint( 0 , 0 ); // head->rect().x() + radius, head->rect().y() + radius
}

void Player::boost(bool boost)
{
    doBoost = boost;
    if(boost)
        setSpeed( boostSpeed );
    else
        setSpeed( initSpeed );
}

void Player::move()
{
    if( doBoost )
        dropPoint();

    if(Worm.length() <= 2) {
        std::cout << " Error: Worm legth <= 0" << std::endl;
        QApplication::exit(-1);
    }

    if((moveLastTimer++) && moveLastTimer >= moveLastTimerSeqence && !(moveLastTimer = 0)) {

        //move last behind vect(0) && set pos from vec(0) && move vect 0
        Worm.move(Worm.size() - 1, 1);
        Worm.at(1)->setPos(Worm.at(0)->pos().x() , Worm.at(0)->pos().y()  );
        Worm.at(1)->setRotation(Worm.at(0)->rotation() );

    }

    //move player
    double theta = Worm.at(0)->rotation() - 90;
    double dy = speed * qSin(qDegreesToRadians(theta));
    double dx = speed * qCos(qDegreesToRadians(theta));

    //move player
    Worm.at(0)->moveBy( dx, dy );

    //move player to mid of qGView
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
    //set new radius
    radius = r;
    //update existing items
    for( auto &e : Worm)
        e->updateRadius( radius );
}

void Player::dropPoint()
{
    removePoint();

    //add item
}

void Player::increaseWorm()
{
    length++;

    WormPart * wp = new WormPart(radius, QBrush(Qt::darkGreen), scale);
    Worm.append( wp);
    wp->setPos(-2000, -2000); // so that you can't see them
    scene->addItem( wp );

}

void Player::decreaseWorm()
{
    length--;

    auto wp = Worm.back();
    Worm.pop_back();

    scene->removeItem(wp);
    delete wp;
}


void Player::addPoint()
{
    points++;

    if( (points % increaseWormLongnessSequenceByPoints_veryOnePoint_add ) == 0 )
        increaseWorm();

    if( (points % increaseWormThiknessSequenceByPoints_every ) == 0 )
        setRadius( radius + increaseWormThiknessSequenceByPoints_add );
}

void Player::removePoint()
{
    //...
    if(points > 0)
        points--;


    if(length > initLength) {

        if(  (points % increaseWormLongnessSequenceByPoints_veryOnePoint_add ) == 0 )
            decreaseWorm();

    } else {

        if( doBoost ) {
            doBoost = false;
            setSpeed( initSpeed );
        }
    }

    if( /*radius > initRadius*/ true ) {
        if( (points % increaseWormThiknessSequenceByPoints_every ) == 0 )
            setRadius( radius - increaseWormThiknessSequenceByPoints_add );
    } else {

        if( doBoost ) {
            doBoost = false;
            setSpeed( initSpeed );
        }

    }

}


void Player::setScale(double scale)
{
    this->scale = scale;
}


size_t Player::getPoints() const
{
    return points;
}

void Player::sceneRectChanged(QPointF min , QPointF max)
{
    scoreText->setPos( min.x() + 10, max.y() - 35 );
    scoreText->setText( "Score: " + QString::number( points ) );

    lengthText->setPos( min.x() + 10, max.y() - 55 );
    lengthText->setText( "Length: " + QString::number( length ) );

    thinknessText->setPos( min.x() + 10, max.y() - 75 );
    thinknessText->setText( "Thikness: " + QString::number( points ) + " - Scale: " + QString::number(scale) );
}

int Player::getLength() const
{
    return length;
}



