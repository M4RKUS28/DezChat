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

#define initRadius 25
#define maxRadius 100

#define initLength 20


// use can set up it in a ranche as Graphicsqualtiy settings
#define wormPartDistance 15
//#define minPartDistance
//#define maxPartDistance

#define initScale 1
#define scaleFaktorAtPoints ka

///ingame setting:
//after x points gets the worm oine bigger:
#define increaseWormThiknessSequenceByPoints_add 0.05
#define increaseWormThiknessSequenceByPoints_every 10

//after x points gets the worm one longer
#define increaseWormLongnessSequenceByPoints_veryOnePoint_add 5

//only every x rounds poit will droped at boost
#define droppointEveryXRounds 6



#define defaultRotation 90
#define turnSpeed 5




class Player : public QObject
{
    Q_OBJECT
public:

    Player(QGraphicsScene * scene, QVector< QPair<QBrush, QPen> > muster, QGraphicsView * mainView, QRectF map);
    ~Player();

    //input ( from game-Object )
    void mousePosChanged(QPointF mousePos);
    void boost( bool boost );

    void start();
    void stop();

    int getLength() const;
    size_t getPoints() const;

    void setMusterVec(const QVector < QPair < QBrush, QPen > > &value);

    bool inGame() const;

public slots:
    //resize Text Items
    void sceneRectChanged( QPointF min, QPointF max  );

signals:
    void lose();
    //void scoreChanged();
    void movedTo( QPointF newPos, double rotation);



private:

    QGraphicsScene * scene;
    QGraphicsView * mainView;
    QRectF map;

    QGraphicsLineItem * debugDirectionLine;


    QVector <WormPart *> Worm;

    QGraphicsSimpleTextItem * scoreText;
    QGraphicsSimpleTextItem * lengthText;
    QGraphicsSimpleTextItem * thinknessText;


    QTimer * moveTimer;
    QTimer * rotateTimer;


    //-
    void resetData();

    //score
    void addPoint();
    void removePoint();

    void setScale( double scale );






    QPair < QBrush, QPen > getMuster();


    double speed, moveLastTimerSeqence, radius, scale;
    int moveLastTimer, length, dropPointCounter;
    size_t points;
    bool doBoost, isInGame;

    void increaseWorm();
    void decreaseWorm();

    void setSpeed(double value);
    void setRadius( double r);
    void dropPoint();

    QVector < QPair < QBrush, QPen > > muster;
    int musterPos;


private slots:
    void move();
    void rotateHead();

};

#endif // PLAYER_H
