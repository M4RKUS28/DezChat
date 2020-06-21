#ifndef WORMIO_SCENE_H
#define WORMIO_SCENE_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsEllipseItem>
#include <QGraphicsPolygonItem>

#include <QGraphicsLineItem>

class WormIO_Scene : public QGraphicsScene
{

public:
    WormIO_Scene();

    QGraphicsEllipseItem * circel;
    QGraphicsPolygonItem * trangle;
    QGraphicsLineItem * line;
    QGraphicsLineItem * line2;

    QGraphicsLineItem * line3;

    QGraphicsLineItem * line4;

    QGraphicsRectItem r1, r2;


    void timerEvent(QTimerEvent *) override;

    void updateTest(int x_P2, int y_P2);

    double x, y;





};

#endif // WORMIO_SCENE_H
