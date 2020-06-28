#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QThread>
#include <QGraphicsRectItem>
#include <QGraphicsWidget>

#include "player.h"
#include "enemymanager.h"
#include <QScrollBar>


class Game : public QGraphicsView
{
    Q_OBJECT
public:
    Game( QSize MainWindowSize, QWidget * parant);

    //tests
    QGraphicsSimpleTextItem * Koords;
    QGraphicsSimpleTextItem * MouseKoords;

    QGraphicsLineItem achsen[3];










public slots:

    void sceneRectChanged(const QRectF &rect);

public:

    //New
    //Objects

    QGraphicsScene * scene;

    Player * player;
    EnemyManager * enemyManager;

    QGraphicsRectItem * mapBorder;
    QGraphicsRectItem * miniMap;

    //Funktions:


    void joinGame();
       /* void leaveGame();

    void showGameLobby();
    void hideGameLobby();


    */

    //Override Funktions:

    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;


};

#endif // GAME_H
