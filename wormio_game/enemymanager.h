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

    EnemyManager( Player * player, QGraphicsScene * scene);
    ~EnemyManager();

    QGraphicsScene *scene;
    Player * player;
    QMap< const Peer *, Enemy * > enemyByPeerMap;

    void startManagingEnemys();
    void stopManagingEnemys();


private:
    bool isManagingEnemys;


public slots:
    void recvedGameMsg(QString msg, Peer * who );

signals:
    void wantSendMsgTO(QString msg, Peer * who );





};

#endif // ENEMYMANAGER_H
