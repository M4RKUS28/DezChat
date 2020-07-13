#include "enemy.h"


Enemy::Enemy(QString initWOrm , QGraphicsScene *scene)
    : scene(scene), length(initLength), radius( initRadius )
{
    std::cout << "INIT: " << initWOrm.toStdString() << std::endl;

    enemyAsGroup = new QGraphicsItemGroup();
    scene->addItem( enemyAsGroup );

    //i,p1,p2,p3 => lW=3,lS=4
    //init Worm
    QStringList initMsgParts = initWOrm.split( "%" );
    for ( int i = 0; i < initMsgParts.length(); ++i ) {

        bool ok1 = true, ok2 = true;
        QStringList subParts = initMsgParts.at(i).split(",");

        if(subParts.length() != 2) {
            std::cerr << "ERROR invalid subPart in WormINIt!!!" << std::endl;
            continue;
        }

        double part1 = subParts.at( 0 ).toDouble( &ok1 );
        double part2 = subParts.at( 1 ).toDouble( &ok2 );

        if( !ok1 || !ok2) {
            std::cerr << " ERROR: CONVERT TO DOUBLE FAILED: IN init new Enemy" << std::endl;
        }


        if( i == 0 ) {
            if( initMsgParts.length() - 1 != static_cast<int>(part1) ) {
                std::cerr << " ERROR: InitMsgLength != WormLength!!" << std::endl;
            } else {
                length = part1;
                radius = part2;
            }
        } else {

            WormPart * nwp = new WormPart( true, radius, QPair<QBrush, QPen>(QBrush(QImage(":/wormio_game/Ressources/border.png")), QPen()), 1);
            Worm.push_back( nwp );

            nwp->setPos(part1, part2);
            enemyAsGroup->addToGroup( nwp );
            //scene->addItem(nwp);

        }
    }

}

void Enemy::destrsoySelf()
{
    std::cout << "I LOOOOOSED" << std::endl;

    for( int i = 0; i < Worm.length(); i++ ) {
        scene->removeItem( Worm.at(i) );
        delete Worm.at(i);
        if( i % 5 == 0);
//            QGraphicsEllipseItem * newPoint = new QGraphicsEllipseItem(10, 10, 20, 20);
    }
    Worm.clear();
}



void Enemy::moveHeadTo(QPointF newPos)
{
    if(Worm.size())
        Worm.at(0)->setPos( newPos );
    else
        std::cerr << "Can't move Head... doesn't exist!" << std::endl;
}



void Enemy::moveLastTo(QPointF newPos)
{
    if(Worm.length() <= 2) {
        std::cout << " Error: Worm legth <= 2" << std::endl;
        return;
    }

    Worm.move(Worm.size() - 1, 1);
    Worm.at(1)->setPos( newPos.x() , newPos.y()  );
///    Worm.at(1)->setRotation(Worm.at(0)->rotation() );

}



void Enemy::updateLength(unsigned newLength)
{
    while ( newLength != length ) {

        if( newLength > length ) {
            length++;

            WormPart * nwp = new WormPart( true, radius, QPair<QBrush, QPen>(QBrush(QImage(":/wormio_game/Ressources/border.png")), QPen()), 1);
            Worm.push_back( nwp );
            nwp->setPos(-200, -2000);

            //enemyAsGroup->addToGroup( nwp );
            scene->addItem(nwp);

        } else if (newLength < length ) {
            length--;

            if( length > initLength ) {
                auto wp = Worm.back();
                Worm.pop_back();

                scene->removeItem(wp);
                delete wp;
            }
        }
    }
}



void Enemy::updateRadius(double radius)
{
    for( auto &wp : Worm )
        wp->updateRadius( radius );

    this->radius = radius;
}
