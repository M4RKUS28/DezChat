#include "wormio_graphicsview.h"


WormIO_GraphicsView::WormIO_GraphicsView(QWidget *parent)
    : QGraphicsView()
{

    scene = new WormIO_Scene();
    this->setScene(scene);



   // setMouseTracking(true);
    //this->setFocus();

    //this->centerOn( this->scene->trangle );

    //this->viewport()->setMouseTracking(true);

    scene->installEventFilter(this);

    setMouseTracking(true);


}

WormIO_GraphicsView::~WormIO_GraphicsView()
{
    delete scene;
    scene = nullptr;




}


#include <iostream>
#include <QGraphicsSceneMouseEvent>
#include <qdebug.h>


bool WormIO_GraphicsView::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == scene){
            // press event
            QGraphicsSceneMouseEvent *mouseSceneEvent;
            if(event->type() == QEvent::GraphicsSceneMousePress){
                mouseSceneEvent = static_cast<QGraphicsSceneMouseEvent *>(event);
                qDebug() << mouseSceneEvent->scenePos()<<mouseSceneEvent->lastScenePos();
               // your logic here
            }
            // move event
            else if (event->type() == QEvent::GraphicsSceneMouseMove) {
                mouseSceneEvent = static_cast<QGraphicsSceneMouseEvent *>(event);
                qDebug() << mouseSceneEvent->scenePos()<<mouseSceneEvent->lastScenePos();
                // your logic here

                scene->updateTest(mouseSceneEvent->scenePos().x(), mouseSceneEvent->scenePos().y());

            }
            // release event
            else if (event->type() == QEvent::GraphicsSceneMouseRelease) {
                mouseSceneEvent = static_cast<QGraphicsSceneMouseEvent *>(event);
                qDebug() << mouseSceneEvent->scenePos()<<mouseSceneEvent->lastScenePos();
                // your logic here
            }
        }
        return QGraphicsView::eventFilter(watched, event);
}
