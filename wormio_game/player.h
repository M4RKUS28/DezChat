#ifndef PLAYER_H
#define PLAYER_H

#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QObject>
#include <QTimer>
#include <QVector>

#include "wormio_game/wormpart.h"
#include <QtMath>


class Player : public QObject
{
    Q_OBJECT
public:

    Player(QGraphicsScene * scene);
    QGraphicsScene * scene;
    QVector <WormPart *> Worm;


    void timerEvent(QTimerEvent *event) override;

    int radius, speed, length, initLength;

    QPointF save;


    QTimer * moveTimer;


    bool isLiving;
    void join();
    void rotateHead(QPointF mousePos, QGraphicsLineItem *debugLine);


public slots:
    void move();

signals:
    void died();


};

#endif // PLAYER_H
