#ifndef ENEMY_H
#define ENEMY_H

#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QObject>

class Enemy : public QObject
{
    Q_OBJECT
public:
    Enemy(QGraphicsScene * scene);

    QGraphicsScene * scene;
};

#endif // ENEMY_H
