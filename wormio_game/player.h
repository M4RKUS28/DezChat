#ifndef PLAYER_H
#define PLAYER_H

#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QObject>
#include <QTimer>
#include <QVector>

#include "wormio_game/wormpart.h"
#include <QtMath>

//setting:
#define initSpeed 2.5

#define initRadius 25
#define maxRadius 100

#define initLength 20


// use can set up it in a ranche as Graphicsqualtiy settings
#define wormPartDistance 15
//#define minPartDistance
//#define maxPartDistance

#define initScale 1


///ingame setting:
//after x points gets the worm oine bigger:
#define increaseWormThiknessSequenceByPoints_add 1
#define increaseWormThiknessSequenceByPoints_every 10

//after x points gets the worm one longer
#define increaseWormLongnessSequenceByPoints_veryOnePoint_add 1




class Player : public QObject
{
    Q_OBJECT
public:

    Player(QGraphicsScene * scene);

    QGraphicsScene * scene;
    QVector <WormPart *> Worm;

    QTimer * moveTimer;

    //-
    void resetData();
    void start();
    void stop();

    //score
    void addPoint();
    void setScale( double scale );

    //input ( from game-Object )
    void rotateHead(QPointF mousePos, QGraphicsLineItem *debugLine);



    int getLength() const;
    size_t getPoints() const;



public slots:
    void move();

signals:
    void lose();
    void scoreChanged();


private:
    double speed, moveLastTimerSeqence, radius, scale;
    int moveLastTimer, length;
    size_t points;

    void increaseWorm();
    void setSpeed(double value);
    void setRadius( double r);

};

#endif // PLAYER_H
