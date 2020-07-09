#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include <QGraphicsScene>
#include <QObject>
#include <iostream>

#include <QMap>
#include "../ConnectionsStuff/peer.h"
#include "enemy.h"

class EnemyManager : public QObject
{
    Q_OBJECT
public:

    EnemyManager( QGraphicsScene * scene);
    ~EnemyManager();
    QGraphicsScene *scene;

    QMap< const Peer *, Enemy * > enemyByPeerMap;



public slots:
    void recvedGameMsg(QString msg, Peer * who );


};

#endif // ENEMYMANAGER_H
