#ifndef PLAYER_H
#define PLAYER_H

#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>

#include <QGraphicsScene>
#include <QObject>



class Player : public QObject
{
    Q_OBJECT
public:
    Player(QGraphicsScene * scene);

    QGraphicsScene * scene;
};

#endif // PLAYER_H
