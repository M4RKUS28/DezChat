#include "game.h"

Game::Game(QSize MainWindowSize, QWidget *parant)
    : QGraphicsView(parant), highscore(0)
{
    //Init Objects:
    //this:
    this->hide();
    this->setMouseTracking(true);
    this->setGeometry( 10, 10, MainWindowSize.width() - 20, MainWindowSize.height() - 20 );
    //this->setFixedSize( MainWindowSize.width() - 20, MainWindowSize.height() - 20 );
    this->setBackgroundBrush(QImage("://wormio_game/Ressources/'background.png'"));

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);



    //scene
    scene = new QGraphicsScene(this);
    this->setScene(scene);
    scene->setSceneRect(150, 150, 500,  500);

    //mapBoarder
    mapBorder = new QGraphicsRectItem();
    mapBorder->setRect(-500, -500, 4000, 4000);
    mapBorder->setPos(0, 0);
    mapBorder->setPen(QPen(/*QBrush(QImage("://wormio_game/Ressources/border.png"))*/Qt::red, 1000));
    scene->addItem(mapBorder);

    //mini Map
    miniMap = new MiniMap(QSize(200, 200), 12, scene);
    miniMap->setMapSize(QSizeF(3000, 3000));
    scene->addItem(miniMap);

    QVector < QPair < QBrush, QPen > > muster;
    muster.push_back( QPair< QBrush, QPen >( QBrush(Qt::blue), QPen(QColor::fromRgb(0, 0, 0, 40), 2) ) );
    muster.push_back( QPair< QBrush, QPen >( QBrush(QImage( ":/wormio_game/Ressources/border.png" )), QPen(QColor::fromRgb(0, 0, 0, 40), 2) ) );
    muster.push_back( QPair< QBrush, QPen >( QBrush(Qt::gray), QPen(QColor::fromRgb(0, 0, 0, 40), 2) ) );

    player = new Player(scene, muster, this, QRectF(0, 0, 3000, 3000));
    enemyManager = new EnemyManager(scene);

    //Mouse Koord debug
    MouseKoords = new QGraphicsSimpleTextItem();
    MouseKoords->setBrush(Qt::white);
    scene->addItem(MouseKoords);

    lobyMessage = new QGraphicsSimpleTextItem();
    statsInLobby = new QGraphicsSimpleTextItem();
    scene->addItem(lobyMessage);
    scene->addItem(statsInLobby);



    //Connect Signals and Slots
    connect(this->scene, SIGNAL(sceneRectChanged(QRectF)), this, SLOT(sceneRectChanged(QRectF)));

    //connect miniMap
    //rePosMap
    connect(this, SIGNAL( sceneRectChanged(QPointF, QPointF) ), miniMap, SLOT( sceneRectChanged(QPointF, QPointF) ));
    //rePosPlayer
    connect(player, SIGNAL(movedTo( QPointF, double)), miniMap, SLOT(playerPosChanched(QPointF , double)));

    //repos text in player
    connect(this, SIGNAL( sceneRectChanged(QPointF, QPointF) ), player, SLOT( sceneRectChanged(QPointF, QPointF)) );

    connect(player, SIGNAL(lose()), this, SLOT(stopGame()));

    Koords = new QGraphicsSimpleTextItem();
    Koords->setBrush(Qt::white);
    scene->addItem(Koords);


    //make it default invisible
    stopGame();
    hideGameLobby();

    //Lobby:

    lobyMessage->setText("     Drücke die Taste <j> um das Spiel zu starten!\nDrücke die Taste <e> um zum Chat zurückzukehren!");
    lobyMessage->setFont(QFont("opensans", 25, 100));
    lobyMessage->setPen(QPen(QBrush(Qt::blue), 1));

    statsInLobby->setPen(QPen(QBrush(Qt::cyan), 1));
    statsInLobby->setFont(QFont("times", 25, 100));;

}

Game::~Game()
{
    std::cout << "~Game()" << std::endl;

    delete player;
    player = nullptr;

    delete enemyManager;
    enemyManager = nullptr;

    delete miniMap;
    miniMap = nullptr;

    delete mapBorder;
    mapBorder = nullptr;

    //debug
    delete Koords;
    Koords = nullptr;

    delete MouseKoords;
    MouseKoords = nullptr;
    //...|

    delete scene;
    scene = nullptr;

}

void Game::mouseMoveEvent(QMouseEvent *event)
{
    player->mousePosChanged( mapToScene( event->pos() ));
    MouseKoords->setText("Mouse-Pos: x" + QString::number(event->x()) + " y=" + QString::number(event->y())
                         + "\n -> MaptoScene: x=" + QString::number(mapToScene(event->pos()).x()) + " + y=" + QString::number(mapToScene(event->pos()).y()) );
}

void Game::mousePressEvent(QMouseEvent *)
{
    player->boost( true );
}

void Game::mouseReleaseEvent(QMouseEvent *)
{
    player->boost( false );
}

void Game::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_E) {
        stopGame();
        emit wantLeaveGame();
    } else if (event->key() == Qt::Key_J) {
        startGame();
    }

}


void Game::sceneRectChanged(const QRectF &)
{
    QPointF min = QPointF( mapToScene(this->pos()) ),
            max = QPointF( mapToScene( QPoint( this->width() + this->x(), this->height() + this->y() ) ) );
    emit sceneRectChanged(min, max);


    Koords->setText("SceneRect: x=" + QString::number(scene->sceneRect().x()) + " y=" + QString::number(scene->sceneRect().y())  );
    Koords->setPos( min.x() + 900, max.y() - 80 );
    MouseKoords->setPos( min.x() + 900, max.y() - 60);
}

void Game::startGame()
{
    hideGameLobby();

    mapBorder->show();
    miniMap->show();
    Koords->show();
    MouseKoords->show();

    player->start();
}

void Game::stopGame() // connected mit lose()
{
    player->stop();
    if(player->getPoints() > highscore)
        highscore = player->getPoints();

    mapBorder->hide();
    miniMap->hide();
    Koords->hide();
    MouseKoords->hide();


    showGameLobby();

}


void Game::showGameLobby()
{
    if(player->getPoints())
        this->statsInLobby->setText("   Bester Score: " + QString::number(highscore) + "\nDein letzter Score: " +QString::number( player->getPoints() ) + "\n      Noch ein Spiel?");
    else
        this->statsInLobby->setText("");

    auto pos = QPointF( mapToScene(this->pos()) );

    statsInLobby->setPos( pos + QPointF(400, 250) );
    lobyMessage->setPos( pos + QPointF(150, 400) );

    statsInLobby->show();
    lobyMessage->show();

}

void Game::hideGameLobby()
{
    this->lobyMessage->hide();
    this->statsInLobby->hide();
}



