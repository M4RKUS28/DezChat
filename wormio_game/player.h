#ifndef PLAYER_H
#define PLAYER_H

#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>

#include <QGraphicsScene>
#include <QObject>
#include <QVector>


class Player : public QObject
{
    Q_OBJECT
public:

    class WormPart : public QObject, public QGraphicsPixmapItem {
    public:
        WormPart();
        void timerEvent(QTimerEvent * ev);
        QPointF center;
    };


    Player(QGraphicsScene * scene);

    QGraphicsScene * scene;

    QVector <WormPart *> Worm;

};

#endif // PLAYER_H
