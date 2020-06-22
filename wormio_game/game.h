#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QThread>


#include "player.h"
#include "enemymanager.h"
#include <QScrollBar>


class Game : public QGraphicsView
{
    Q_OBJECT
public:
    Game( QWidget * parant);


    QGraphicsScene * scene;

    Player * player;
    EnemyManager * enemyManager;

    QGraphicsSimpleTextItem * Koords;
    QGraphicsSimpleTextItem * MouseKoords;

    QGraphicsLineItem achsen[3];

    QGraphicsPolygonItem * mapBorder;




    void mouseMoveEvent(QMouseEvent *event) override;

public slots:

    void sceneRectChanged(const QRectF &rect);

};

#endif // GAME_H
