#ifndef WORMIO_SCENE_H
#define WORMIO_SCENE_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsEllipseItem>
#include <QGraphicsPolygonItem>


class WormIO_Scene : public QGraphicsScene
{

public:
    WormIO_Scene();

    QGraphicsEllipseItem * rect;
    QGraphicsPolygonItem * trangle;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void updateTest(int x, int y);

    double x, y;

};

#endif // WORMIO_SCENE_H
