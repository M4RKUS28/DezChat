#ifndef WORMPART_H
#define WORMPART_H

#include <QObject>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>

#include <QGraphicsScene>
#include <iostream>
#include <qmath.h>


class WormPart : public QObject, public QGraphicsEllipseItem // */ QGraphicsRectItem
{
    Q_OBJECT
public:
    WormPart(double radius, QBrush brush, double scale, QGraphicsItem * parant = nullptr);
    void updateRadius( double rNew );
    void updateScale( double scale );

};

#endif // WORMPART_H
