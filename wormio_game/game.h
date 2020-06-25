#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QThread>
#include <QGraphicsRectItem>

#include "player.h"
#include "enemymanager.h"
#include <QScrollBar>


class Game : public QGraphicsView
{
    Q_OBJECT
public:
    Game( QWidget * parant);

    //tests
    QGraphicsSimpleTextItem * Koords;
    QGraphicsSimpleTextItem * MouseKoords;

    QGraphicsLineItem achsen[3];



    //important/save
    QGraphicsScene * scene;

    Player * player;
    EnemyManager * enemyManager;

    QGraphicsRectItem * mapBorder;
    QGraphicsRectItem * miniMap;






    void mouseMoveEvent(QMouseEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

public slots:

    void sceneRectChanged(const QRectF &rect);

};

#endif // GAME_H
