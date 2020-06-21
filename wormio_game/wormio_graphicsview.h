#ifndef WORMIO_GRAPHICSVIEW_H
#define WORMIO_GRAPHICSVIEW_H

#include <QGraphicsView>
#include "wormio_scene.h"
#include <QMouseEvent>

class WormIO_GraphicsView : public QGraphicsView
{

public:
    WormIO_GraphicsView(QWidget * parent);
    ~WormIO_GraphicsView();

    WormIO_Scene * scene;

    bool eventFilter(QObject *watched, QEvent *event) override;

private:


};

#endif // WORMIO_GRAPHICSVIEW_H
