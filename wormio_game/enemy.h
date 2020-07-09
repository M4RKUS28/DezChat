#ifndef ENEMY_H
#define ENEMY_H

#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QObject>
#include <QPair>
#include <QGraphicsItemGroup>

#include "wormpart.h"
#include "player.h" // initValues
#include <iostream>

class Enemy : public QObject
{
    Q_OBJECT
public:
    Enemy(QPointF startPos, QGraphicsScene * scene);

    void destrsoySelf();
    void moveHeadTo(QPointF newPos);
    void moveLastTo(QPointF newPos);
    void updateLength( unsigned newLength );
    void updateRadius( double radius );

    QGraphicsScene * scene;


    QGraphicsItemGroup * enemyAsGroup;



private:
    unsigned length;
    double radius;
    QVector< WormPart * > Worm;


};

#endif // ENEMY_H
