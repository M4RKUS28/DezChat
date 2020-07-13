#include "wormpart.h"




WormPart::WormPart(bool isEnemy, double radius, QPair<QBrush, QPen> muster, double scale, QGraphicsItem *parant)
    : QGraphicsEllipseItem /*QGraphicsRectItem */ (parant), isEnemy( isEnemy )
{
    //Rect
    this->setRect( -1 * radius, -1 * radius, 2* radius , 2 * radius); // size
    this->setPos(600, 200);

    //Form
    this->setScale(scale);
    this->setBrush(muster.first);
    this->setPen(muster.second); // Rand transparenter

}

WormPart::~WormPart()
{

}



void WormPart::updateRadius(double rNew)
{
    this->setRect( -1 * rNew, -1 * rNew, 2* rNew , 2 * rNew);
}

void WormPart::updateScale(double scale)
{
    this->setScale( scale );
}

