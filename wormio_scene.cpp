#include "wormio_scene.h"

WormIO_Scene::WormIO_Scene()
{
    rect = new QGraphicsEllipseItem(200, 200, 100, 100 );

    QPolygonF Triangle;
    Triangle.append(QPointF(-10.,0));
    Triangle.append(QPointF(0.,-10));
    Triangle.append(QPointF(10.,0));
    Triangle.append(QPointF(-10.,0));

    trangle = new QGraphicsPolygonItem(Triangle);


    this->addItem( rect );

    this->addItem( trangle );
    //this->setFocus();

    x = trangle->x();
    y = trangle->y();

}

#include <iostream>
#include <qmath.h>

void WormIO_Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    rect->setPos(event->pos());
    event->accept();
    std::cout << "Mousemove..." << std::endl;
}

void WormIO_Scene::updateTest(int x, int y)
{
    // x = R( r^2 / 1 + m^2 )
    // m = Dy / Dx = .. / x2-x1

    double m = (y - this->y) / (x - this->x);
    double radius = 1.5;
    double x_add = sqrt( (pow( radius, 2 )) / (( 1 + pow( m, 2)))) ;
    double y_add = m * x_add;




    trangle->moveBy(x_add, y_add);
}
