#include "wormpart.h"




WormPart::WormPart(double radius, QBrush brush, double scale, QGraphicsItem *parant)
    : QGraphicsEllipseItem /*QGraphicsRectItem */ (parant)
{
    //Rect
    this->setRect( -1 * radius, -1 * radius, 2* radius , 2 * radius); // size
    this->setPos(600, 200);

    //Form
    this->setScale(scale);
    this->setBrush(brush);
    this->setPen(QPen(QColor::fromRgb(0, 0, 0, 20))); // Rand transparenter

}



void WormPart::updateRadius(double rNew)
{
    this->setRect( -1 * rNew, -1 * rNew, 2* rNew , 2 * rNew);
}

void WormPart::updateScale(double scale)
{
    this->setScale( scale );
}

