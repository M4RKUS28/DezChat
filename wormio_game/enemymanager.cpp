#include "enemymanager.h"


EnemyManager::EnemyManager(Player *player, QGraphicsScene *scene)
    : scene(scene), player(player), isManagingEnemys( false )
{
//    recvedGameMsg("MOVEDTO=456.01,45.90", nullptr);
}

EnemyManager::~EnemyManager()
{
    std::cout << "~EnemyManager()" << std::endl;
}

void EnemyManager::startManagingEnemys()
{
    isManagingEnemys = true;
}

void EnemyManager::stopManagingEnemys()
{
    isManagingEnemys = false;

    for ( auto &e : enemyByPeerMap.keys() ) {
        enemyByPeerMap.value(e)->destrsoySelf();
        delete enemyByPeerMap.value(e);
        enemyByPeerMap.remove( e );
    }

}

void EnemyManager::recvedGameMsg(QString msg, Peer *who)
{
    QString what = msg.left( msg.indexOf( '=' ) );
    QString value = msg.mid( msg.indexOf( '=' ) + 1 );


    if( what == "SCORE") { // SCORE=NAME,SCORE
        //manage ScoreList


    }


    if( isManagingEnemys == false )
        return;

    if( what == "INIT_REQ" || what == "INIT_ANSWER" ) { // INIT_*=Length,Radius%Pos1x,Pos1y%....

        if( enemyByPeerMap.contains( who ) ) {
            std::cerr << "WARNING: Have to remove old Worm cause new Init!!" << std::endl;
            enemyByPeerMap[who]->destrsoySelf();
            delete enemyByPeerMap[who];
            if( enemyByPeerMap.remove(who) == 0) {
                std::cerr << ("Remove old CLient failed!!") << std::endl;
                return;
            }
        }

        Enemy * newEnemy = new Enemy( value, scene);
        enemyByPeerMap.insert(who, newEnemy);

        if( what == "INIT_REQ" ) {
            emit wantSendMsgTO( "GM=INIT_ANSWER=" + player->getWormAsString() , who );
            std::cout << "GOT INIT: " << value.toStdString() << " --> send init: " << ("GM=INIT_ANSWER=" + player->getWormAsString()).toStdString() << std::endl;


        }

    }


    if ( ! enemyByPeerMap.contains( who ) ) {
        //??..
        std::cout << " ENEMY NOT IN MAP: " << who << std::endl;
        return;
    }


    if ( what == "M.H.TO" || what == "M.L.TO" )
    {

        int pos = value.indexOf(",");
        bool ok = true;

        if( pos  == -1)
        {
            std::cerr << " Couldn't find ',' => Invalid MOVEDTO Message!!" << std::endl;
            return;
        }

        QPointF newPos = QPointF( value.left( pos ).toDouble( &ok ), value.mid( pos + 1).toDouble( &ok ) );

        if( !ok )
        {
            std::cerr << "Casting String to double failed ',' => Invalid MOVEDTO Message!!" << std::endl;
            return;
        }

        if( what == "M.H.TO" )
             enemyByPeerMap[who]->moveHeadTo( newPos );
        else if ( what == "M.L.TO" ) {
            enemyByPeerMap[who]->moveLastTo( newPos );
        }


    }
    else if ( what == "NEW_LENGTH" || what == "NEW_RADIUS")
    {
        bool ok = true;

        unsigned length = value.toUInt( &ok );
        double radius = value.toDouble( &ok );

        if( !ok ) {
            std::cerr << "Casting String to double failed => Invalid NEW_LENGTH/NEW_RADIUS Message!!: " << value.toStdString() << std::endl;
            return;
        }

        if( what == "NEW_LENGTH" )
            enemyByPeerMap[who]->updateLength( length );
        else if (  what == "NEW_RADIUS")
            enemyByPeerMap[who]->updateRadius( radius );

    } else if ( what == "DIED" ) {

        enemyByPeerMap[who]->destrsoySelf();
        delete enemyByPeerMap[who];
        enemyByPeerMap.remove(who);

    }


}
