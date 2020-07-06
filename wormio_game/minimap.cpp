#include "minimap.h"

MiniMap::MiniMap(QSize miniMapSize, double playerSize, QGraphicsScene *scene, QGraphicsItem *parent)
    : QGraphicsRectItem(parent), miniMapSize(miniMapSize), scene(scene), masstabX(0), masstabY(0), playerSize(playerSize)
{
    this->setRect(0, 0, miniMapSize.width(), miniMapSize.height());
    this->setPos(10, 10);
    this->setPen(QPen(Qt::white, 3));


    player = new QGraphicsPolygonItem( this );


    QPolygonF threeangle;
    player->setPolygon( (threeangle << QPointF(0, 0) << QPointF(playerSize, 0.0) << QPointF(playerSize / 2.0, (playerSize / -2.0) * sqrt( 3 )  )) );
    player->setTransformOriginPoint( playerSize / 2.0,  playerSize / - 3.0);

    player->setBrush(Qt::white);


}

MiniMap::~MiniMap()
{
    std::cout << "~MiniMap()" << std::endl;

    scene->removeItem(player);
    delete player;
    player = nullptr;
}

void MiniMap::playerPosChanched(QPointF pos, double rotation)
{
//    std::cout << "Setpos: " <<  pos.x()  << "*" << masstabX << " " << pos.y() << "*" << masstabY << std::endl;
    player->setPos( pos.x() * masstabX - (playerSize / 2) , pos.y() * masstabY + (playerSize / 2));
    player->setRotation( rotation );
}

void MiniMap::sceneRectChanged(QPointF min, QPointF max)
{
    this->setPos(max.x() - this->rect().width() - 33,
                 min.y() + 10);
}


void MiniMap::setMapSize(const QSizeF &value)
{
    mapSize = value;

    if(mapSize.width())
        masstabX = miniMapSize.width() / mapSize.width();
    if(mapSize.height())
        masstabY = miniMapSize.height() / mapSize.height();

}
