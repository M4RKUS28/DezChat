#include "enemymanager.h"


EnemyManager::EnemyManager(QGraphicsScene *scene)
    : scene(scene)
{

}

EnemyManager::~EnemyManager()
{
    std::cout << "~EnemyManager()" << std::endl;
}
