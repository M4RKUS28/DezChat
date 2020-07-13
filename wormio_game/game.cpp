#include "game.h"


Game::Game(QSize MainWindowSize, QWidget *parant)
    : QGraphicsView(parant), highscore(0), showOverLays( true )
{


    this->hide();

    //Init Objects:
    this->setGeometry( 10, 10, MainWindowSize.width() - 20, MainWindowSize.height() - 20 );
    this->setBackgroundBrush(QImage("://wormio_game/Ressources/'background.png'"));

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    //scene
    scene = new QGraphicsScene(this );
    scene->setSceneRect( 0, 0, this->width(), this->height() );
    this->setScene(scene);


    //mapBoarder
    mapBorder = new QGraphicsRectItem( -500, -500, 4000, 4000 );
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

    player = new Player(scene, muster, this, QRectF(0, 0, 3000, 3000));
    enemyManager = new EnemyManager(player, scene);
    frameRate = (50);
//debug msgs
    netWorkTransfereMsg = new QGraphicsTextItem();
    netWorkTransfereMsg->setDefaultTextColor(Qt::white);
    scene->addItem(netWorkTransfereMsg);


    fpsTextItem = new QGraphicsTextItem("-1 fps");
    fpsTextItem->setDefaultTextColor(Qt::white);
    scene->addItem( fpsTextItem );
    fpsTimer = new QTimer( this );
    connect(fpsTimer, SIGNAL(timeout()), this, SLOT(fpsTimerTimeout()));
    fpsTimer->start(1000);



    //dawei:


    startRect = new QGraphicsRectItem( -150, -150, 300, 300 );
    startRect->setPos(1500, 1500);
    startRect->setPen(QPen(Qt::white, 3));
    scene->addItem(startRect);






    ///LOBBY------------------------------------------------------------>


    QSize buttonSize(500, 50);
    startSpielButton = new QPushButton( "Neues Spiel", this);
    startSpielButton->setGeometry(      (this->width() - buttonSize.width()) / 2 + 150, 550, buttonSize.width(), buttonSize.height());
    startSpielButton->setFont(QFont("times", 20));
    startSpielButton->setStyleSheet( "background-color: #3cbaa2; border: 1px solid black; border-radius: 5px;" );
    //button 2
    leaveGameLobbyButton = new QPushButton( "Zurück zum Chat", this);
    leaveGameLobbyButton->setGeometry(  (this->width() - buttonSize.width()) / 2 + 150, 610, buttonSize.width(), buttonSize.height());
    leaveGameLobbyButton->setFont(QFont("times", 20));
    leaveGameLobbyButton->setStyleSheet( "background-color: #3cbaa2; border: 1px solid black; border-radius: 5px;" );

    showOverLaysButton = new QPushButton( "OL An/Aus" , this );
    showOverLaysButton->setGeometry( 60 , 650, 100, 30);
    showOverLaysButton->setStyleSheet( "background-color: #3cbaa2; border: 1px solid black; border-radius: 5px;" );


    //background picture && container für lobby items
    picturePos = QPoint(540, 200); // move background whitout moving its item

    lobbyBackGround = new QGraphicsRectItem(0, 0, 2120, 1192);
    lobbyBackGround->setBrush(QBrush(QImage(":/wormio_game/Ressources/slitherIo.png")));
    lobbyBackGround->setPos( -1 * picturePos );



    //--->Overlays
    //der ramen und "vater" aller items darinnen
    messageBox = new QGraphicsRectItem(picturePos.x(), picturePos.y(), 1100, 650, lobbyBackGround);
    messageBox->setPos( (this->width() - messageBox->rect().width()) / 2, 50);

    statsOverLay = new QGraphicsRectItem( picturePos.x() + 150, picturePos.y() + 20, 500, 200,  messageBox );
    playerListOverLay = new QGraphicsRectItem( picturePos.x() + 780, picturePos.y() + 20, 300, 450,  messageBox );

    //setUp Brush & Pen from all overlaysd:
    this->showOverLays = false;
    onShowOverLaysButtonClicked(); // setUp all overlays
    //<--

    listOfYourStats = new QGraphicsTextItem(messageBox);
    listOfYourStats->setDefaultTextColor(Qt::cyan);
    listOfYourStats->setFont(QFont("times", 30));
    listOfYourStats->setPos( picturePos.x() + 200, picturePos.y() + 50 );

    listOfPlayingPlayer = new QGraphicsTextItem(messageBox);
    listOfPlayingPlayer->setDefaultTextColor(Qt::gray);
    listOfPlayingPlayer->setFont(QFont("times", 14));
    listOfPlayingPlayer->setPos( picturePos.x() + 800, picturePos.y() + 50 );


    //unten rechts die infos
    tipps = new QGraphicsTextItem(lobbyBackGround);
    tipps->setPlainText("Mit der Taste 'e' kommst du aus dem Spiel zurück ins Menü!\nMit der Taste 'q' kommst du sofort in den Chat zurück!\nBei der Taste '?' passiert gar nichts. (:");
    tipps->setPos(750 + picturePos.x(), 710 + picturePos.y());
    tipps->setDefaultTextColor(Qt::gray);


    //add whole lobby-parentItems to scene
    hideGameLobby();
    scene->addItem(lobbyBackGround);


    //Connect LobbyButons:
    connect(startSpielButton, SIGNAL(clicked()), this, SLOT(onStartButtonClicked()));
    connect(leaveGameLobbyButton, SIGNAL(clicked()), this, SLOT(onBackToChatButtonClicked()));
    connect(showOverLaysButton, SIGNAL(clicked()), this, SLOT(onShowOverLaysButtonClicked()));

///    ------------------------------------------------------_>


    //Connect Signals and Slots
    //sceneRect Changes => RePos items:
    connect(this->scene, SIGNAL(sceneRectChanged(QRectF)), this, SLOT(sceneRectChanged(QRectF)));
    connect(this, SIGNAL( sceneRectChanged(QPointF, QPointF) ), miniMap, SLOT( sceneRectChanged(QPointF, QPointF) ));
    connect(this, SIGNAL( sceneRectChanged(QPointF, QPointF) ), player, SLOT( sceneRectChanged(QPointF, QPointF)) );
    //rePosPlayer in miniMap
    connect(player, SIGNAL(movedTo( QPointF, double)), miniMap, SLOT(playerPosChanched(QPointF , double)));
    //connect lose funktion to stopgame
    connect(player, SIGNAL(lose()), this, SLOT(playerLose()));

    //make gameStuff invisible
    stopGame();

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

    delete scene;
    scene = nullptr;

}



void Game::stopGame() // connected mit lose()
{
    player->stop();
    if(player->getPoints() > highscore)
        highscore = player->getPoints();
    enemyManager->stopManagingEnemys();


    mapBorder->hide();
    miniMap->hide();
    netWorkTransfereMsg->hide();
    fpsTextItem->hide();

}

void Game::playerLose()
{
    stopGame();
    showGameLobby();
}


void Game::startGame()
{
    mapBorder->show();
    miniMap->show();
    netWorkTransfereMsg->show();
    fpsTextItem->show();

    enemyManager->startManagingEnemys();
    player->start_( QPoint(1500, 1500 ) );
}

void Game::hideGameLobby()
{
    this->startSpielButton->hide();
    this->leaveGameLobbyButton->hide();
    this->lobbyBackGround->hide();
    this->showOverLaysButton->hide();

}

void Game::showGameLobby(bool fromLobby)
{
    this->lobbyBackGround->setPos( mapToScene( -1 * picturePos ) ) ;
/*//*/    listOfPlayingPlayer->setPlainText("1. Platz: Markus: 10 Punkte\n2. <Du>: 0\n....: ...");


    listOfYourStats->setPlainText("Dein letzter Score: " + QString::number( player->getPoints() ) +
                                "\nDein Highscore: " + QString::number( highscore ) + "\n....: ...");

    if( fromLobby )
        this->startSpielButton->setText("Starte ein Spiel");
    else
        this->startSpielButton->setText("Nochmal spielen");



    this->startSpielButton->show();
    this->leaveGameLobbyButton->show();
    this->showOverLaysButton->show();
    this->lobbyBackGround->show();

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
        showGameLobby();
    } else if (event->key() == Qt::Key_Q) {
        stopGame();
        emit wantLeaveGame();
    }

}

void Game::fpsTimerTimeout()
{
    fpsTextItem->setPlainText(QString( QString::number( frameRate, 'f', 2 ) + " fps"));
    frameRate = 0;
}

void Game::drawForeground(QPainter *, const QRectF &)
{
    frameRate++;
}

void Game::sceneRectChanged(const QRectF &)
{
    emit sceneRectChanged(mapToScene(this->pos()), mapToScene( QPoint( this->width() + this->x(), this->height() + this->y() )) );

    netWorkTransfereMsg->setPos( mapToScene(this->pos()) + QPointF(10, 5) );
    fpsTextItem->setPos( mapToScene(this->pos()) + QPointF(10, 60) );
}


void Game::onStartButtonClicked()
{
    this->hideGameLobby();
    this->startGame();
}

void Game::onBackToChatButtonClicked()
{
    hideGameLobby();
    emit wantLeaveGame();
}

void Game::onShowOverLaysButtonClicked()
{
    if( (showOverLays = !showOverLays) ) {

        messageBox->setPen(QPen(Qt::gray, 2));
        messageBox->setBrush(QBrush(QColor::fromRgb(255, 255, 255, 30)));

        statsOverLay->setPen(QPen(QBrush(Qt::gray), 2));
        statsOverLay->setBrush(QBrush(QColor::fromRgb(255, 255, 255, 30)));

        playerListOverLay->setPen(QPen(QBrush(Qt::gray), 2));
        playerListOverLay->setBrush(QBrush(QColor::fromRgb(255, 255, 255, 30)));


    } else {

        messageBox->setPen(Qt::NoPen);
        messageBox->setBrush(QBrush());

        statsOverLay->setPen(Qt::NoPen);
        statsOverLay->setBrush(QBrush());

        playerListOverLay->setPen(Qt::NoPen);
        playerListOverLay->setBrush(QBrush());


    }
}


void Game::mouseMoveEvent(QMouseEvent *)
{
//    std::cout << " QGraphicsView( " << event->x() << " | " << event->y() << " ) --> Scene( " << mapToScene(event->pos()).x() << " | " << mapToScene( event->pos() ).y() << " )" << std::endl;
}





