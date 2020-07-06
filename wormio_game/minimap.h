#ifndef MINIMAP_H
#define MINIMAP_H


#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QPen>
#include <QBrush>
#include <QObject>
#include <QRect>
#include <QApplication>

#include <math.h>
#include <iostream>



class MiniMap : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:

    MiniMap(QSize miniMapSize, double playerSize, QGraphicsScene * scene, QGraphicsItem * parent = nullptr);
    ~MiniMap();

    void setMapSize(const QSizeF &value);


public slots:
    void playerPosChanched(QPointF pos , double rotation);
    void sceneRectChanged(QPointF min, QPointF max);


private:
    QSizeF mapSize, miniMapSize;
    QGraphicsScene * scene;

//    QGraphicsPixmapItem * player;
    QGraphicsPolygonItem * player;


    double masstabX, masstabY, playerSize;

};

#endif // MINIMAP_H
