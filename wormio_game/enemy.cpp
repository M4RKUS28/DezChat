#include "enemy.h"



Enemy::Enemy(QPointF startPos , QGraphicsScene *scene)
    : scene(scene), length(initLength), radius( initRadius )
{
    std::cout << "Start: " << startPos.x() << " - " << startPos.y() << " with: Length=" << initLength << std::endl;

    enemyAsGroup = new QGraphicsItemGroup();
    scene->addItem( enemyAsGroup );

}

void Enemy::destrsoySelf()
{
    std::cout << "I LOOOOOSED" << std::endl;


}

void Enemy::moveHeadTo(QPointF newPos)
{
//    std::cout << " moveTo: " << newPos.x() << " " << newPos.y() << std::endl;
    if(Worm.size())
        Worm.at(0)->setPos( newPos );


}

void Enemy::moveLastTo(QPointF newPos)
{
    if(Worm.length() <= 2) {
        std::cout << " Error: Worm legth <= 2" << std::endl;
        return;
    }

    //move last behind vect(0) && set pos from vec(0) && move vect 0
    Worm.move(Worm.size() - 1, 1);
    Worm.at(1)->setPos( newPos.x() , newPos.y()  );
///    Worm.at(1)->setRotation(Worm.at(0)->rotation() );

}



void Enemy::updateLength(unsigned newLength)
{
    std::cout << "New Length: " << newLength << std::endl;


    if( newLength > length || true ) {
        //create new Part --> Radius == Worm[0].radius

        WormPart * nwp = new WormPart( radius, QPair<QBrush, QPen>(Qt::green, QPen()), 1);
        Worm.push_back( nwp );

        nwp->setPos(1500, 1500);
        //enemyAsGroup->addToGroup( nwp );
        scene->addItem(nwp);
        std::cout << "IN SCENE? " << scene->items().contains( nwp ) << std::endl;



    } else if (newLength < length ) {



    }

    length = newLength;
}

void Enemy::updateRadius(double radius)
{
    std::cout << "New Radius: " << radius << std::endl;

    for( auto &wp : Worm ) {
        wp->updateRadius( radius );

        //...
    }

    this->radius = radius;
}
