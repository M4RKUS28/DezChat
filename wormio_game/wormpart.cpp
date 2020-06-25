#include "wormpart.h"

#include <iostream>


WormPart::WormPart(int radius, QGraphicsItem *parant)
    : QGraphicsEllipseItem(parant), radius(radius)
{
    //setPixmap(QPixmap(":/wormio_game/Ressources/test.png"));
    //setScale(1);
    setRotation(90);
    //this->setBrush(Qt::blue);

    //this->setPos(500, 300);

    this->setRect(0, 0, 120, 120); // size
    this->setPos(1500, 100);

    this->setBrush(Qt::green);
    std::cout << "new wormpart" << std::endl;


    this->setPen(QPen(QColor::fromRgb(0, 0, 0, 20))); // Rand transparenter
}

#include <qmath.h>

void WormPart::timerEvent(QTimerEvent *ev)
{


}

QPointF WormPart::getCenter()
{
    return QPointF(this->pos().x() + radius, this->pos().y() + radius);

    return QPointF(this->rect().x() + radius, this->rect().y() + radius);
}
