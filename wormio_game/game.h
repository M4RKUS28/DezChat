#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>

#include "player.h"
#include "enemymanager.h"

class Game : public QGraphicsView
{
public:
    Game(QWidget * parant);


    QGraphicsScene * scene;

    Player * player;
    EnemyManager * enemyManager;
};

#endif // GAME_H
