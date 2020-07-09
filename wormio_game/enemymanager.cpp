#include "enemymanager.h"


EnemyManager::EnemyManager(QGraphicsScene *scene)
    : scene(scene)
{
//    recvedGameMsg("MOVEDTO=456.01,45.90", nullptr);
}

EnemyManager::~EnemyManager()
{
    std::cout << "~EnemyManager()" << std::endl;
}

void EnemyManager::recvedGameMsg(QString msg, Peer *who)
{
    QString what = msg.left( msg.indexOf( '=' ) );
    QString value = msg.mid( msg.indexOf( '=' ) + 1 );

    if( what == "INIT" ) {

        //check if msg is ok....
        int pos1, pos2;
        bool ok = true;

        if( (pos1 = value.indexOf(",")) == -1) {
            std::cerr << " Couldn't find ',' => Invalid INIT Message!!" << std::endl;
            return;
        }

        QPointF startPos = QPointF( value.left(pos1).toDouble(&ok), value.mid( pos1 + 1 ).toDouble(&ok) );//INIT=x,y

        if( !ok ) {
            std::cerr << " Some cast failed => Invalid INIT Message!!" << std::endl;
            return;
        }


        if( enemyByPeerMap.contains( who ) ) {
            enemyByPeerMap[who]->destrsoySelf();
            delete enemyByPeerMap[who];
            if( enemyByPeerMap.remove(who) != 0) {
                std::cerr << ("Remove old CLient failed!!") << std::endl;
                return;
            }
        }

        Enemy * newEnemy = new Enemy( startPos, scene);
        enemyByPeerMap.insert(who, newEnemy);


    }
    else if ( what == "M.H.TO" || what == "M.L.TO" )
    {
        if( ! enemyByPeerMap.contains( who ) )
        {
            std::cerr << ( "MOVEDTO: Enemy does't exist in Map!!" ) << std::endl;
            return;
        }
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
        if( ! enemyByPeerMap.contains( who ) ) {
            std::cerr << ( "NEW_LENGTH || NEW_RADIUS: Enemy does't exist in Map!!" ) << std::endl;
            return;
        }
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

        if( ! enemyByPeerMap.contains( who ) ) {
            std::cerr << ( "DIED: Enemy does't exist in Map!!" ) << std::endl;
            return;
        }
        enemyByPeerMap[who]->destrsoySelf();
        delete enemyByPeerMap[who];
        enemyByPeerMap.remove(who);

    }


}
