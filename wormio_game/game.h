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
#include <QTimer>

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

    //debug MSG
    QGraphicsTextItem * netWorkTransfereMsg;
    QGraphicsTextItem * fpsTextItem;
    QTimer * fpsTimer;
    double frameRate;

public slots:

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
    void fpsTimerTimeout();



private:

    //New
    //Objects

    //Game
    QGraphicsScene * scene;
    QGraphicsRectItem * mapBorder;
    MiniMap * miniMap;
    QGraphicsRectItem * startRect;




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
    void drawForeground(QPainter *painter, const QRectF &rect) override;


signals:
    void sceneRectChanged(QPointF min, QPointF max);
    void wantLeaveGame();

};

#endif // GAME_H
