#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include <QGraphicsScene>
#include <QObject>
#include <iostream>

class EnemyManager : public QObject
{
    Q_OBJECT
public:

    EnemyManager( QGraphicsScene * scene);
    ~EnemyManager();
    QGraphicsScene *scene;



};

#endif // ENEMYMANAGER_H
