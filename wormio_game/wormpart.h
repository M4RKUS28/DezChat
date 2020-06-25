#ifndef WORMPART_H
#define WORMPART_H

#include <QObject>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>

#include <QGraphicsScene>



class WormPart : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    WormPart(int radius, QGraphicsItem * parant = nullptr);
    void timerEvent(QTimerEvent * ev);
    QPointF getCenter();
    int radius = 60;

};

#endif // WORMPART_H
