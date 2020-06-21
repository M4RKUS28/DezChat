#include "game.h"

Game::Game(QWidget *parant)
    : QGraphicsView(parant)
{
    //create scene
    scene = new QGraphicsScene(this);
    //set scene
    this->setScene(scene);

    //Create Player
    player = new Player(scene);

    //create enemyManager with Acces on Scene
    enemyManager = new EnemyManager(scene);

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}
