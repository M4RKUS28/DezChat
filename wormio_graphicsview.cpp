#include "wormio_graphicsview.h"


WormIO_GraphicsView::WormIO_GraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{

    scene = new WormIO_Scene();
    this->setScene(scene);


    //setMouseTracking(true);
    //this->setFocus();

    this->centerOn( this->scene->rect );

    //this->viewport()->setMouseTracking(true);

}

WormIO_GraphicsView::~WormIO_GraphicsView()
{
    delete scene;
    scene = nullptr;




}


#include <iostream>

void WormIO_GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    std::cout << " Mouse: " << event->x() << " " << event->y() << " in graphicsview" << std::endl;

}

