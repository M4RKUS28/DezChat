#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QThread>
#include <QGraphicsRectItem>
#include <QGraphicsWidget>
#include <QScrollBar>

#include <QPushButton>
#include <QPalette>


#include "player.h"
#include "enemymanager.h"
#include "minimap.h"


class Game : public QGraphicsView
{
    Q_OBJECT
public:
    Game( QSize MainWindowSize, QWidget * parant);
    ~Game();

    void showGameLobby( bool fromLobby = false);
    EnemyManager * enemyManager;
    Player * player;

private:
    void hideGameLobby();
    void startGame();
    void stopGame();


private slots:
    void playerLose();

    void sceneRectChanged(const QRectF &rect);
    void onStartButtonClicked();
    void onBackToChatButtonClicked();
    void onShowOverLaysButtonClicked();



private:

    //New
    //Objects

    //Game
    QGraphicsScene * scene;
    QGraphicsRectItem * mapBorder;
    MiniMap * miniMap;

    //Lobby
    unsigned long highscore;
    bool showOverLays;

    QGraphicsRectItem * lobbyBackGround;
    QPoint picturePos;

    QGraphicsTextItem * lobbyTitle;

    QPushButton * startSpielButton;
    QPushButton * leaveGameLobbyButton;
    QPushButton * showOverLaysButton;

    QGraphicsRectItem * messageBox;
    QGraphicsRectItem * statsOverLay;
    QGraphicsRectItem * playerListOverLay;


    QGraphicsTextItem * listOfPlayingPlayer;
    QGraphicsTextItem * listOfYourStats;
    QGraphicsTextItem * tipps;



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
