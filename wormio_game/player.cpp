#include "player.h"

Player::Player(QGraphicsScene *scene, QVector< QPair<QBrush, QPen> > muster, QGraphicsView *mainView, QRectF map)
    : scene(scene), mainView(mainView), map(map), moveLastTimer(0), isInGame(false), muster(muster)
{
    // init objects
    scoreInfos = new QGraphicsTextItem();
    scoreInfos->setDefaultTextColor(Qt::white);
    scene->addItem(scoreInfos);


    moveTimer = new QTimer(this);
    moveTimer->setInterval(20);
    connect(moveTimer, SIGNAL(timeout()), this, SLOT(move()));

    rotateTimer = new QTimer(this);
    rotateTimer->setInterval(20);
    connect(rotateTimer, SIGNAL(timeout()), this, SLOT(rotateHead()));

    //setup setting
    setSpeed(initSpeed);
    resetData();

}

Player::~Player()
{
    std::cout << "~Player()" << std::endl;

    moveTimer->stop();
    rotateTimer->stop();

    delete moveTimer;
    moveTimer = nullptr;

    delete rotateTimer;
    rotateTimer = nullptr;

    //scene deletes items
}


bool Player::inGame() const
{
    return isInGame;
}

QPair<QBrush, QPen> Player::getMuster()
{
    QPair < QBrush, QPen > ret;

    if(muster.size() > 0 && musterPos < muster.size()) {
        ret = muster.at( musterPos  );
        if ( (++musterPos) >= muster.size() )
            musterPos = 0;
    }
    return ret;
}

int Player::getLength() const
{
    return length;
}


void Player::resetData()
{

    moveTimer->stop();
    rotateTimer->stop();

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
    dropPointCounter = 0;
    length = initLength;
    doBoost = false;
    points = 0;
    musterPos = 0;



}

void Player::initWormAt(QPoint startPos)
{
    //init worm
    for(int i = 0; i < length; i++) {
        WormPart * wp = new WormPart(radius, getMuster(), scale);
        Worm.prepend( wp );
        wp->setPos( startPos );
        scene->addItem( wp );
    }
}



void Player::start(QPoint at)
{
    if(inGame())
        return;

    resetData();
    initWormAt( at );

    moveTimer->start();
    rotateTimer->start();
    isInGame = true;

    emit sendDataToPeers("GM=INIT=" + QString::number( at.x() ) + "," + QString::number( at.y() )  );

    //...
    for( int i = 0; i < 200; i++)
        addPoint();

}

void Player::stop()
{
    if(!inGame())
        return; // do not drop points if was not ingame bevore

    moveTimer->stop();
    rotateTimer->stop();



    //drop points....
    emit sendDataToPeers("GM=DIED");

    //rm old Worm: && ---> drop points
    for( auto e : Worm) {
        scene->removeItem(e);
        delete e;
    }
    Worm.clear();

    isInGame = false;

}




void Player::boost(bool boost)
{
    if( getLength() <= initLength )
        return;

    doBoost = boost;
    if(boost)
        setSpeed( boostSpeed );
    else
        setSpeed( initSpeed );
}

void Player::move()
{
    if( (++dropPointCounter) >= droppointEveryXRounds  && ! (dropPointCounter = 0) ) {
        if( doBoost )
            dropPoint();
    }

    if(Worm.length() <= 2) {
        std::cout << " Error: Worm legth <= 2" << std::endl;
        QApplication::exit(-1);
    }

    if((moveLastTimer++) && moveLastTimer >= moveLastTimerSeqence && !(moveLastTimer = 0)) {

        //move last behind vect(0) && set pos from vec(0) && move vect 0
        Worm.move(Worm.size() - 1, 1);
        Worm.at(1)->setPos(Worm.at(0)->pos().x() , Worm.at(0)->pos().y()  );
        Worm.at(1)->setRotation(Worm.at(0)->rotation() );
        emit sendDataToPeers("GM=M.L.TO=" + QString::number( Worm.at(0)->pos().x() ) + "," + QString::number( Worm.at(0)->pos().y() ) );

    }

    //move player
    double theta = Worm.at(0)->rotation() - 90;
    Worm.at(0)->moveBy( speed * qCos(qDegreesToRadians(theta)), speed * qSin(qDegreesToRadians(theta)) );

    //move player to mid of qGView
    scene->setSceneRect(Worm.at(0)->sceneBoundingRect());

    //update miniMap
    emit movedTo(Worm.at(0)->pos(), Worm.at(0)->rotation());
    emit sendDataToPeers("GM=M.H.TO=" + QString::number( Worm.at(0)->pos().x() ) + "," + QString::number( Worm.at(0)->pos().y() ) );



    //Check for losing.....
    if( !map.contains(Worm.at(0)->pos())) {
        emit lose();
    }

   // auto e = scene->items( Worm.at(0)->pos() );



}

void Player::rotateHead()
{
    if(this->Worm.size() < 1)
        return;

    QLineF ln( this->Worm.at(0)->pos(), mainView->mapToScene(  mainView->mapFromGlobal( QCursor::pos() ) ) );

    double rot = Worm.at(0)->rotation() - defaultRotation;
    double x = rot - 360 + ln.angle() ;

    x = rot - //derzeitige rotation MINUS
            ( ( x > turnSpeed ) ? turnSpeed : ( ( x < (-turnSpeed) ) ? (-turnSpeed) : x ) ) * // +- Geschwindigkeit, oder weniger, wenn kleiner als Geschwindigkeit MAL
            ( ( /*qAbs*/abs(x) > 180 ) ? -1 : 1 ); // +-1, um kürzeren Wegn zu nehmen

    if(x > 360)
        x -= 360;
    else if(x < 0)
        x += 360;

    Worm.at(0)->setRotation( x + defaultRotation  );

}

void Player::setSpeed(double value)
{
    speed = value;
    if(speed)
        moveLastTimerSeqence = wormPartDistance / speed;
    if(moveLastTimerSeqence < 1)
        std::cout << "WARNING: moveLastTimerSeqence < 1 !: " << moveLastTimerSeqence << std::endl;
}

void Player::setRadius(double r)
{
    //set new radius
    radius = r;
    //update existing items
    for( auto &e : Worm)
        e->updateRadius( radius );

    emit sendDataToPeers("GM=NEW_RADIUS=" + QString::number( radius ) );
}

void Player::dropPoint()
{
    removePoint();

    //add item
}



void Player::increaseWorm()
{
    length++;

    WormPart * wp = new WormPart(radius, getMuster(), scale);
    Worm.append( wp);
    wp->setPos(-2000, -2000); // so that you can't see them
    scene->addItem( wp );

    emit sendDataToPeers("GM=NEW_LENGTH=" + QString::number( length ));
}

void Player::decreaseWorm()
{
    length--;

    auto wp = Worm.back();
    Worm.pop_back();

    scene->removeItem(wp);
    delete wp;

    emit sendDataToPeers("GM=NEW_LENGTH=" + QString::number( length ));
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


//void Player::setScale(double scale)
//{
//    this->scale = scale;
//}


size_t Player::getPoints() const
{
    return points;
}

void Player::sceneRectChanged(QPointF min , QPointF max)
{
    scoreInfos->setPos(min.x() + 5, max.y() - 65);
    scoreInfos->setPlainText( "Score: " + QString::number( points ) + "\nLength: " + QString::number( length ));
}




