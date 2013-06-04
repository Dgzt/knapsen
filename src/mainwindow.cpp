#include <QtCore/QTimer>
#include <KDE/KConfigDialog>
#include <KDE/KStatusBar>
#include <KDE/KAction>
#include <KDE/KActionCollection>
#include <KDE/KStandardAction>
#include <KDE/KStandardGameAction>
#include <KDE/KGlobal>
#include <KDE/KStandardDirs>
#include <KDE/KMessageBox>
#include <KDE/KDebug>
//#include <KDE/KPageDialog>
#include <KDE/KAction>

#include "mainwindow.h"
#include "table/centralwidget.h"
#include "settings.h"
#include "config-knapsen.h"

#include "engine/client.h"
#include "engine/server.h"

#include "dialogs/newgamedialog.h"
#include "dialogs/waitforserverdialog.h"
#include "dialogs/waitforclientdialog.h"
#include "dialogs/endrounddialog.h"
#include "dialogs/endgamedialog.h"
#include "dialogs/waitingforopponent.h"

#include "ui_playersettingswidget.h"
#include "ui_gamesettingswidget.h"

//
#include "table/svgimage.h"
#include "table/array.h"
//

MainWindow::MainWindow( QWidget* parent ) : 
    KXmlGuiWindow( parent ),
    mGameStatus( Knapsen::None )
{
    mServer = 0;
    mClient = 0;
    endRoundDialog = 0;
    endGameDialog = 0;
    
    //Initialize the data path (example pictures, icons, etc)
    initializePaths();
    
    //
    qmlRegisterType<SvgImage>("CustomComponents", 1, 0, "SvgImage");
    qmlRegisterType<Array>("CustomComponents", 1, 0, "Array");
    //
    
    //Setup the central widget
    mCWidget = new CentralWidget( this );
    setCentralWidget( mCWidget );

    setupActions();

    setupGUI();
}

MainWindow::~MainWindow()
{}

void MainWindow::initializePaths()
{
    KGlobal::dirs()->addPrefix( KNAPSEN_PREFIX );
    
    QString path;
    path = KGlobal::dirs()->findResource( "appdata", "pics/background.png" );
    
    if( path.isNull() ){
        path = KNAPSEN_PREFIX "/share/apps/knapsen/pics/background.png";
        
        KMessageBox::error( this, i18n( "Knapsen not found the \"%1\"!", path ) );
        
        exit(0);
    }
    
    path = KGlobal::dirs()->findResource( "appdata", "pics/william-tell.svgz" );
    
    if( path.isNull() ){
        path = KNAPSEN_PREFIX "/share/apps/knapsen/pics/william-tell.svgz";
        
        KMessageBox::error( this, i18n( "Knapsen not found the \"%1\"!", path ) );
        
        exit(0);
    }
    
    path = KGlobal::dirs()->findResource( "appdata", "pics/tigullio-bridge.svg" );
    
    if( path.isNull() ){
        path = KNAPSEN_PREFIX "/share/apps/knapsen/pics/tigullio-bridge.svg";
        
        KMessageBox::error( this, i18n( "Knapsen not found the \"%1\"!", path ) );
        
        exit(0);
    }
    
    //
    path = KGlobal::dirs()->findResource( "appdata", "pics/arrow.png" );
    
    if( path.isNull() ){
        path = KNAPSEN_PREFIX "/share/apps/knapsen/pics/arrow.png";
        
        KMessageBox::error( this, i18n( "Knapsen not found the \"%1\"!", path ) );
        
        exit(0);
    }
}

void MainWindow::setupActions()
{
    //Game menu
    KStandardGameAction::gameNew( this, SLOT( slotNewGame() ), actionCollection() );
    mCloseGameAction = KStandardGameAction::end( this, SLOT( closeGameSlot() ), actionCollection() );
    KStandardGameAction::quit( this, SLOT( close() ), actionCollection() );
    
    mCloseGameAction->setEnabled( false );
    
    //Settings menu
    KStandardAction::preferences(this, SLOT(optionsPreferences()), actionCollection());
}

void MainWindow::setupConnects()
{
    connect( mClient, SIGNAL( signalInitialize( QString, QString, Knapsen::TypeOfCards ) ), mCWidget, SLOT( slotInitialize( QString, QString, Knapsen::TypeOfCards ) ) );
    connect( mClient, SIGNAL( signalNewGame() ), mCWidget, SLOT( slotStartGame() ) );
    connect( mClient, SIGNAL( signalNewRound() ), mCWidget, SLOT( slotNewRound() ) );
    connect( mClient, SIGNAL( signalNewPlayerCard( bool, Card* ) ), mCWidget, SLOT( slotNewPlayerCard( bool, Card* ) ) );
    connect( mClient, SIGNAL( signalNewPlayerCardTrumpCard() ), mCWidget, SLOT( slotNewPlayerCardTrumpCard() ) );
    connect( mClient, SIGNAL( signalNewOpponentCard( bool ) ), mCWidget, SLOT( slotNewOpponentCard( bool ) ) );
    connect( mClient, SIGNAL( signalNewOpponentCardTrumpCard()), mCWidget, SLOT( slotNewOpponentCardTrumpCard() ) );
    connect( mClient, SIGNAL( signalNewTrumpCard( Card* ) ), mCWidget, SLOT( slotNewTrumpCard( Card* ) ) );
    connect( mClient, SIGNAL( signalPlayerCardSelectableChanged( int, bool ) ), mCWidget, SLOT( slotPlayerCardSelectableChanged( int, bool ) ) );
    connect( mClient, SIGNAL( signalOpponentSelectedCard( int, Card* ) ), mCWidget, SLOT( slotOpponentSelectedCard( int, Card* ) ) );
    connect( mClient, SIGNAL( signalOpponentGetCentralCards() ), mCWidget, SLOT( slotOpponentGetCentralCards() ) );
    connect( mClient, SIGNAL( signalPlayerGetCentralCards() ), mCWidget, SLOT( slotPlayerGetCentralCards() ) );
    connect( mClient, SIGNAL( signalCloseButtonVisible( bool ) ), mCWidget, SLOT( slotCloseButtonVisibleChanged( bool ) ) );
    connect( mClient, SIGNAL( signalCloseDeck() ), mCWidget, SLOT( slotCloseDeck() ) );
    connect( mClient, SIGNAL( signalFortyButtonVisible( bool ) ), mCWidget, SLOT( slotFortyButtonVisibleChanged( bool ) ) );
    connect( mClient, SIGNAL( signalTwentyButtonVisible( bool ) ), mCWidget, SLOT( slotTwentyButtonVisibleChanged( bool ) ) );
    connect( mClient, SIGNAL( signalSchnapsenButtonVisible( bool ) ), mCWidget, SLOT( slotSchnapsenButtonVisibleChanged( bool ) ) );
    connect( mClient, SIGNAL( signalOpponentTricksChanged( int ) ), mCWidget, SLOT( slotOpponentTricksChanged( int ) ) );
    connect( mClient, SIGNAL( signalOpponentScoresChanged( int ) ), mCWidget, SLOT( slotOpponentScoresChanged( int ) ) );
    connect( mClient, SIGNAL( signalPlayerTricksChanged( int ) ), mCWidget, SLOT( slotPlayerTricksChanged( int ) ) );
    connect( mClient, SIGNAL( signalPlayerScoresChanged( int ) ), mCWidget, SLOT( slotPlayerScoresChanged( int ) ) );
    connect( mClient, SIGNAL( signalTrumpCardSelectableChanged( bool ) ), mCWidget, SLOT( slotTrumpCardSelectableChanged( bool ) ) );
    connect( mClient, SIGNAL( signalPlayerChangeTrumpCard( int ) ), mCWidget, SLOT( slotPlayerChangeTrumpCard( int ) ) );
    connect( mClient, SIGNAL( signalOpponentChangeTrumpCard( int, Card* ) ), mCWidget, SLOT( slotOpponentChangeTrumpCard( int, Card* ) ) );
    connect( mClient, SIGNAL( signalShowOpponentCards( int, Card, int, Card ) ), mCWidget, SLOT( slotShowOpponentCards( int, Card, int, Card ) ) );
    
    connect( mCWidget, SIGNAL( signalAnimationEnd() ), mClient, SLOT( slotProcessCommands() ) );
    connect( mCWidget, SIGNAL( signalSelectedCard( int, int ) ), mClient, SLOT(slotSelectCardId( int, int ) ) );
    connect( mCWidget, SIGNAL( signalCloseButtonClicked() ), mClient, SLOT( slotCloseButtonClicked() ) );
    connect( mCWidget, SIGNAL( signalFortyButtonClicked() ), mClient, SLOT( slotFortyButtonClicked() ) );
    connect( mCWidget, SIGNAL( signalTwentyButtonClicked() ), mClient, SLOT( slotTwentyButtonClicked() ) );
    connect( mCWidget, SIGNAL( signalSchnapsenButtonClicked() ), mClient, SLOT( slotSchnapsenButtonClicked() ) );
    connect( mCWidget, SIGNAL( signalTrumpCardClicked() ), mClient, SLOT( slotSelectTrumpCard() ) );
}

Server* MainWindow::createServer()
{
    Knapsen::TypeOfCards typeOfCards;
    if( Settings::germanSuits() ){
        typeOfCards = Knapsen::GermanSuits;
    }else{ // else french suits
        typeOfCards = Knapsen::FrenchSuits;
    }
    
    int sizeOfDeck;
    if( Settings::sizeOfDeck20() ){
        sizeOfDeck = 20;
    }else{ 
        sizeOfDeck = 24;
    }
    
    int numberOfCardsInHand;
    if( Settings::cardsInHand5() ){
        numberOfCardsInHand = 5;
    }else{
        numberOfCardsInHand = 6;
    }
    
    Knapsen::WhoStartGame whoStartGame;
    if( Settings::localPlayer() ){
        whoStartGame = Knapsen::AdminPlayer;
    }else if( Settings::opponentPlayer() ){
        whoStartGame = Knapsen::OpponentPlayer;
    }else{ // else random player
        whoStartGame = Knapsen::RandomPlayer;
    }
    
    bool enabledSchnapsenButton = Settings::schnapsenButton();
    
    return new Server( typeOfCards, sizeOfDeck, numberOfCardsInHand, whoStartGame, enabledSchnapsenButton );
}

void MainWindow::slotNewGame()
{
    kDebug() << "New game.";
    
    if( mCloseGameAction->isEnabled() ){
        
        int ret = KMessageBox::questionYesNo( this, i18n( "Really want to start a new game?" ) );
        
        if( ret == KMessageBox::Yes ){ //Close the current game and start new game
            closeGameSlot();
        }else{
            return;
        }
        
    }
    
    NewGameDialog newGameDialog;
    
    int ret = newGameDialog.exec();
    
    if( ret ){
        kDebug() << "KDialog button: Ok";
        
        mClient = new Client( Settings::playerName() );
        
        setupConnects();
        
        connect( mClient, SIGNAL( error( QAbstractSocket::SocketError ) ),    this,   SLOT( slotSocketError( QAbstractSocket::SocketError ) ) );
        connect( mClient, SIGNAL( signalGameError( Client::GameErrorType ) ), this,   SLOT( slotGameError( Client::GameErrorType ) ) );
        connect( mClient, SIGNAL( signalEndRound( QString, int ) ),           this,   SLOT( slotEndRound( QString, int ) ) );
        connect( mClient, SIGNAL( signalEndGame( QString ) ),                 this,   SLOT( slotEndGame( QString ) ) );
        
        if( newGameDialog.getGameMode() == Knapsen::LocalMode ){
            kDebug() << "Game mode: local";
            
            mServer = createServer();
            
            connect( mServer, SIGNAL( signalServerFull() ), this, SLOT( slotStartGame() ) );
            connect( mServer, SIGNAL( signalServerEmpty() ), this, SLOT( slotServerEmpty() ) );
            
            if( mServer->listen( QHostAddress::LocalHost ) ){
                kDebug() << "Server port:" << mServer->serverPort();
                
                mClient->connectToHost( QHostAddress::LocalHost, mServer->serverPort() );
                
                mServer->addBot( Settings::botName(), newGameDialog.getGameDifficulty() );
                
                mGameStatus = Knapsen::LocalMode;
            }else{
                kDebug() << "Server listen error!";
                
                delete mClient;
                mClient = 0;
            }
            
        }else if( newGameDialog.getGameMode() == Knapsen::ClientMode ){
            kDebug() << "Game mode: client";
            
            WaitForServerDialog waitForServerDialog;
            
            connect( mClient, SIGNAL( error( QAbstractSocket::SocketError ) ), &waitForServerDialog, SLOT( reject() ) );
            connect( mClient, SIGNAL( signalGameError( Client::GameErrorType ) ), &waitForServerDialog, SLOT( reject() ) );
            connect( mClient, SIGNAL( signalNewGame() ), &waitForServerDialog, SLOT( accept() ) );
            
            mClient->connectToHost( newGameDialog.getClient_ServerAddress(), newGameDialog.getClient_ServerPort() );
            
            if( waitForServerDialog.exec() ){
                mCloseGameAction->setEnabled( true );
                mGameStatus = Knapsen::ClientMode;
            }else{
                kDebug() << "WaitForServerDialog - Cancel button.";
                mClient->disconnectFromHost();
                delete mClient;
                mClient = 0;
                
            }
            
        }else{ //newGameDialog.getGameMode() == Knapsen::ServerMode
            kDebug() << "Game mode: server";
            
            mServer = createServer();
            
            connect( mServer, SIGNAL( signalServerEmpty() ), this, SLOT( slotServerEmpty() ) );
            
            if( mServer->listen( QHostAddress::Any, newGameDialog.getServer_ServerPort() ) ){
            
                mClient->connectToHost( QHostAddress::LocalHost , mServer->serverPort() );
                
                WaitForClientDialog waitForClientDialog;
                waitForClientDialog.setAdminName( mClient->getName() );
                
                connect( mServer, SIGNAL( signalPlayerConnected( QString ) ), &waitForClientDialog, SLOT( slotPlayerConnected( QString ) ) );
                connect( mServer, SIGNAL( signalPlayerDisconnected( QString ) ), &waitForClientDialog, SLOT( slotPlayerDisconnectedSlot( QString ) ) );
                connect( mServer, SIGNAL( signalServerFull() ), &waitForClientDialog, SLOT( slotServerFull() ) );
                
                if( waitForClientDialog.exec() ){
                    
                    slotStartGame();
                    
                    mGameStatus = Knapsen::ServerMode;
                }else{
                    kDebug() << "WaitForClientDialog - Cancel button";
                    
                    mClient->disconnectFromHost();
                    delete mClient;
                    mClient = 0;
                }
                
            }else{
                kDebug() << "The port is busy!";
                
                delete mClient;
                mClient = 0;
            }
            
        }
    }else{
        kDebug() << "KDialog button: Cancel";
    }
}

void MainWindow::closeGameSlot()
{
    kDebug() << "Close game.";
    
    //mCWidget->clearWidget();
    mCWidget->clear();
    
    mClient->disconnectFromHost();
    
    //delete client;
    //client = 0;
    mClient->deleteLater();
    mClient = 0;
    
    mCloseGameAction->setEnabled( false );
        
    mGameStatus = Knapsen::None;
}


void MainWindow::optionsPreferences()
{
    if( KConfigDialog::showDialog( "settings" ) ){
        return;
    }
    
    KConfigDialog *settingsDialog = new KConfigDialog( this, "settings", Settings::self() );
    
    QWidget *playerSettingsWidget = new QWidget;
    Ui::PlayerSettingsWidgetUi pswu;
    pswu.setupUi( playerSettingsWidget );
    
    settingsDialog->addPage( playerSettingsWidget, i18n( "Player" ), "playersettings.png", i18n( "Player settings" ) );
    
    QWidget *gameSettingsWidget = new QWidget;
    Ui::GameSettingsUi gsu;
    gsu.setupUi( gameSettingsWidget );
    
    settingsDialog->addPage( gameSettingsWidget, i18n( "Game" ), "gamesettings.png", i18n( "Game settings" ) );
    
    settingsDialog->setAttribute( Qt::WA_DeleteOnClose );
    
    settingsDialog->show();
}

void MainWindow::slotStartGame()
{
    kDebug() << "Start game.";
    
    mServer->startGame();
    
    mCloseGameAction->setEnabled( true );
}

void MainWindow::slotServerEmpty()
{
    kDebug() << "Server is empty!";
    
    mServer->close();
    mServer->deleteLater();
    mServer = 0;
}

void MainWindow::slotSocketError( QAbstractSocket::SocketError socketError )
{
    kDebug() << socketError;
    
    switch( socketError ){
        case QAbstractSocket::ConnectionRefusedError : 
             KMessageBox::error( this, i18n( "The connection was refused!" ) ); 
             break;
        case QAbstractSocket::HostNotFoundError : 
             KMessageBox::error( this, i18n( "The host not found!" ) ); 
             break;
        case QAbstractSocket::RemoteHostClosedError : 
             KMessageBox::error( this, i18n( "The remote host closed the connection!" ) ); 
             closeGameSlot();
             break;
        default: 
             KMessageBox::error( this, i18n( "Undefined error!" ) );
    }
    
}

void MainWindow::slotGameError( Client::GameErrorType gameErrorType )
{
    if( gameErrorType == Client::NameIsBusy ){
        kDebug() << "Name is busy!";
        KMessageBox::error( this, i18n( "The name is busy!" ) );
    }
    
    if( gameErrorType == Client::OpponentDisconnected ){
        kDebug() << "Opponent disconnected!";
        
        //if( mCloseGameAction->isEnabled() ){
        if( mGameStatus != Knapsen::None ){
            closeGameSlot();
            KMessageBox::information( this, i18n( "The opponent disconnected!" ) );
        }
    }

    if( gameErrorType == Client::ServerIsFull ){
        kDebug() << "Name is busy!";
        KMessageBox::error( this, i18n( "The server is full!" ) );
    }
}

void MainWindow::slotEndRound( QString name, int scores )
{
    endRoundDialog = new EndRoundDialog( this, name, scores );
    
    QTimer::singleShot( 0, this, SLOT( slotEndRoundExec() ) );
}

void MainWindow::slotEndRoundExec()
{
    connect( mClient, SIGNAL( signalGameError( Client::GameErrorType ) ), endRoundDialog, SLOT( reject() ) );
    connect( mClient, SIGNAL( error( QAbstractSocket::SocketError ) ), endRoundDialog, SLOT( reject() ) );
    
    int ret = endRoundDialog->exec();
    
    delete endRoundDialog;
    endRoundDialog = 0;
    
    if( ret ){
        WaitingForOpponentDialog waitingForOpponentDialog( this );
        
        connect( mClient, SIGNAL( signalGameError( Client::GameErrorType ) ), &waitingForOpponentDialog, SLOT( accept() ) );
        connect( mClient, SIGNAL( error( QAbstractSocket::SocketError ) ), &waitingForOpponentDialog, SLOT( accept() ) );
        
        connect( mClient, SIGNAL( signalNewRound() ), &waitingForOpponentDialog, SLOT( accept() ) );
        
        mClient->startNextRound();
        
        waitingForOpponentDialog.exec();
    }else{
        
        if( mCloseGameAction->isEnabled() ){
            closeGameSlot();
        }
        
    }
}

void MainWindow::slotEndGame( QString name )
{
    endGameDialog = new EndGameDialog( this, name );
    QTimer::singleShot( 0, this, SLOT( slotEndGameExec() ) );
}

void MainWindow::slotEndGameExec()
{
    connect( mClient, SIGNAL( signalGameError( Client::GameErrorType ) ), endGameDialog, SLOT( reject() ) );
    connect( mClient, SIGNAL( error( QAbstractSocket::SocketError ) ), endGameDialog, SLOT( reject() ) );
    
    int ret = endGameDialog->exec();
    
    if( ret ){
            
        kDebug() << "Start next game.";
        
        WaitingForOpponentDialog waitingForOpponentDialog( this );
        
        connect( mClient, SIGNAL( signalGameError( Client::GameErrorType ) ), &waitingForOpponentDialog, SLOT( accept() ) );
        connect( mClient, SIGNAL( error( QAbstractSocket::SocketError ) ), &waitingForOpponentDialog, SLOT( accept() ) );
        
        connect( mClient, SIGNAL( signalNewGame() ), &waitingForOpponentDialog, SLOT( accept() ) );
        
        mClient->startNextGame();
        
        waitingForOpponentDialog.exec();
        
    }else{
        
        if( mCloseGameAction->isEnabled() ){
            kDebug() << "Close game.";
            
            closeGameSlot();
        }
        
    }
    
    delete endGameDialog;
    endGameDialog = 0;
    
}
