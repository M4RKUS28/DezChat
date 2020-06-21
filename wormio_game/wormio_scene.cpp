#include "wormio_scene.h"

WormIO_Scene::WormIO_Scene()
{


   /* QPolygonF Triangle;
    Triangle.append(QPointF(-10.,0));
    Triangle.append(QPointF(0.,-10));
    Triangle.append(QPointF(10.,0));
    Triangle.append(QPointF(-10.,0));

    trangle = new QGraphicsPolygonItem(Triangle);
*/


    this->addItem( trangle );
    //this->setFocus();



    //r1.setRect(-100, -100, 100, 100);
   // r1.setRect(-300, -300, 300, 300);
   // this->addItem(&r1);
   // this->addItem(&r2);

   // QPointF scenePoint = mapToScene(event->Pos());




    /*line = new QGraphicsLineItem( (this->width() / 2) - 1 ,( this->height() / 2) -1, (this->width() / 2) + 1 ,( this->height() / 2) + 1);
    this->addItem(line);
    line2 = new QGraphicsLineItem( (this->width() / 2) -1 ,( this->height() / 2) + 1 , (this->width() / 2) + 1 ,( this->height() / 2) - 1);
    this->addItem(line2);
*/
    line3 = new QGraphicsLineItem;
    this->addItem(line3);

    //line4 = new QGraphicsLineItem;
    //this->addItem(line4);


    x = 10 ;
    y = 10;

    circel = new QGraphicsEllipseItem(x -50, y -50, 100, 100 );
    this->addItem( circel );


    this->setSceneRect(-250, -250, 500, 500);
    this->setBackgroundBrush( QBrush(Qt::green) );

    this->startTimer(1000);

}

#include <iostream>
#include <qmath.h>

void WormIO_Scene::timerEvent(QTimerEvent *)
{
    this->sceneRect().setRect(this->sceneRect().x() + 100, this->sceneRect().y(), this->sceneRect().width(), this->sceneRect().height());
    std::cout << "t.." << std::endl;
}


void WormIO_Scene::updateTest(int x_P2, int y_P2)
{
    // x = R( r^2 / 1 + m^2 )
    double r = 100;

    double x_P1 = this->x,
           y_P1 = this->y;

    double delta_y = ( y_P2 - y_P1 ),
           delta_x = ( x_P2 - x_P1 );

    if(delta_x == 0) {
        delta_x = 1;
        delta_y = MAXFLOAT * ((delta_y < 0) ? -1 : 1);
    }
    double m =  ( delta_y / delta_x );

    double dx = (( delta_x > 0 ) ? 1 : -1) *  sqrt( (r * r) / ( 1 + (m * m) )  );
    double dy = m * dx;

    double x = x_P1 + dx;
    double y = y_P1 + dy;


    line3->setLine( x_P1 , y_P1, x, y);

    //this->setSceneRect(this->x + dx, this->y + dy, 500, 500);

    //trangle->moveBy(x_add, y_add);
}

