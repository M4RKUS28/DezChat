#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QThread>
#include <QGraphicsRectItem>
#include <QGraphicsWidget>
#include <QScrollBar>

#include "player.h"
#include "enemymanager.h"
#include "minimap.h"

class Game : public QGraphicsView
{
    Q_OBJECT
public:
    Game( QSize MainWindowSize, QWidget * parant);
    ~Game();



    //tests
    QGraphicsSimpleTextItem * Koords;
    QGraphicsSimpleTextItem * MouseKoords;


public slots:
    void stopGame();
    void showGameLobby();

private slots:
    void sceneRectChanged(const QRectF &rect);


private:

    //New
    //Objects

    //Game
    QGraphicsScene * scene;
    Player * player;
    EnemyManager * enemyManager;
    QGraphicsRectItem * mapBorder;
    MiniMap * miniMap;

    //Lobby
    QGraphicsSimpleTextItem * lobyMessage;
    QGraphicsSimpleTextItem * statsInLobby;
    unsigned long highscore;

    //Funktions:


    void startGame();
    void hideGameLobby();



    //Override Funktions:

    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;


signals:
    void sceneRectChanged(QPointF min, QPointF max);
    void wantLeaveGame();

};

#endif // GAME_H
