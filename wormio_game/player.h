#ifndef PLAYER_H
#define PLAYER_H

#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QObject>
#include <QTimer>
#include <QVector>
#include <QApplication>
#include "wormio_game/wormpart.h"
#include <QtMath>



//setting:


constexpr static double initSpeed  = 3;
constexpr static double boostSpeed = 8;

constexpr static double initRadius = 25;
constexpr static double maxRadius  = 100;

constexpr static double initLength = 20;


// use can set up it in a ranche as Graphicsqualtiy settings
constexpr static double wormPartDistance = 15;
//constexpr static double minPartDistance
//constexpr static double maxPartDistance

constexpr static double initScale  = 1;
constexpr static double scaleFaktorAtPoints = -1;

///ingame setting:
//after x points gets the worm oine bigger:
constexpr static double increaseWormThiknessSequenceByPoints_add   = 0.05;
constexpr static int increaseWormThiknessSequenceByPoints_every = 10;

//after x points gets the worm one longer
constexpr static int increaseWormLongnessSequenceByPoints_veryOnePoint_add = 5;

//only every x rounds poit will droped at boost
constexpr static double droppointEveryXRounds = 6;



constexpr static double defaultRotation = 90;
constexpr static double turnSpeed = 5;




class Player : public QObject
{
    Q_OBJECT
public:

    Player(QGraphicsScene * scene, QVector< QPair<QBrush, QPen> > muster, QGraphicsView * mainView, QRectF map);
    ~Player();

    //input ( from game-Object )
    void boost( bool boost );

    void start( QPoint at );
    void stop();

    int getLength() const;
    size_t getPoints() const;
    bool inGame() const;


public slots:
    //resize Text Items
    void sceneRectChanged( QPointF min, QPointF max  );

signals:
    void lose();
    //void scoreChanged();
    void movedTo( QPointF newPos, double rotation);

    void sendDataToPeers(QString msg);

private:

    QGraphicsScene * scene;
    QGraphicsView * mainView;
    QRectF map;

    QVector <WormPart *> Worm;

    QGraphicsTextItem * scoreInfos;

    QTimer * moveTimer;
    QTimer * rotateTimer;

    //-
    void resetData();
    void initWormAt( QPoint startPos );

    void setSpeed(double value);
    void setRadius( double r);

    void dropPoint();

    void addPoint();
    void removePoint();
    void increaseWorm();
    void decreaseWorm();

    QPair < QBrush, QPen > getMuster();

//    void setScale( double scale );

    double speed, moveLastTimerSeqence, radius, scale;
    int moveLastTimer, length, dropPointCounter, musterPos;
    size_t points;
    bool doBoost, isInGame;
    QVector < QPair < QBrush, QPen > > muster;


private slots:
    void move();
    void rotateHead();

};

#endif // PLAYER_H
