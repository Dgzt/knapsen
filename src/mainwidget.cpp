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

#include "mainwidget.h"
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

MainWindow::MainWindow( QWidget* parent ) : 
    KXmlGuiWindow( parent ),
    mGameStatus( Knapsen::None )
{
    mServer = 0;
    client = 0;
    endRoundDialog = 0;
    endGameDialog = 0;
    
    //Initialize the data path (example pictures, icons, etc)
    initializePaths();
    
    //Setup the central widget
    cWidget = new CentralWidget( this );
    setCentralWidget( cWidget );

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
    KStandardGameAction::gameNew(                       this, SLOT( newGameSlot()  ), actionCollection() );
    mCloseGameAction = KStandardGameAction::end(        this, SLOT(closeGameSlot() ), actionCollection() );
    KStandardGameAction::quit(                          this, SLOT( close()        ), actionCollection() );
    
    mCloseGameAction->setEnabled( false );
    
    //Settings menu
    KStandardAction::preferences(this, SLOT(optionsPreferences()), actionCollection());
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

void MainWindow::setGameSignals()
{
    connect( client, SIGNAL( error( QAbstractSocket::SocketError ) ),    this,   SLOT( slotSocketError( QAbstractSocket::SocketError ) ) );
    connect( client, SIGNAL( signalGameError( Client::GameErrorType ) ), this,   SLOT( slotGameError( Client::GameErrorType ) ) );
    connect( client, SIGNAL( signalEndRound( QString, int ) ),           this,   SLOT( slotEndRound( QString, int ) ) );
    connect( client, SIGNAL( signalEndGame( QString ) ),                 this,   SLOT( slotEndGame( QString ) ) );
}

void MainWindow::newGameSlot()
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
    
    NewGameDialog newGameDialog( this );
    
    int ret = newGameDialog.exec();
    
    if( ret ){
        kDebug() << "KDialog button: Ok";
        
        client = new Client( Settings::playerName() );
        //
        cWidget->setClient( client );
        //
        
        setGameSignals();
        
        if( newGameDialog.getGameMode() == Knapsen::LocalMode ){
            kDebug() << "Game mode: local";
            
            mServer = createServer();
            
            connect( mServer, SIGNAL( signalServerFull() ), this, SLOT( slotStartGame() ) );
            connect( mServer, SIGNAL( signalServerEmpty() ), this, SLOT( slotServerEmpty() ) );
            
            if( mServer->listen( QHostAddress::LocalHost ) ){
                kDebug() << "Server port:" << mServer->serverPort();
                
                client->connectToHost( "127.0.0.1", mServer->serverPort() );
                
                mServer->addBot( Settings::botName(), newGameDialog.getGameDifficulty() );
                
                mGameStatus = Knapsen::LocalMode;
            }else{
                kDebug() << "Server listen error!";
                
                delete client;
                client = 0;
            }
            
        }else if( newGameDialog.getGameMode() == Knapsen::ClientMode ){
            kDebug() << "Game mode: client";
            
            WaitForServerDialog waitForServerDialog;
            
            connect( client, SIGNAL( error( QAbstractSocket::SocketError ) ), &waitForServerDialog, SLOT( reject() ) );
            connect( client, SIGNAL( signalGameError( Client::GameErrorType ) ), &waitForServerDialog, SLOT( reject() ) );
            connect( client, SIGNAL( signalNewGame() ), &waitForServerDialog, SLOT( accept() ) );
            
            client->connectToHost( newGameDialog.getClient_ServerAddress(), newGameDialog.getClient_ServerPort() );
            
            if( waitForServerDialog.exec() ){
                mCloseGameAction->setEnabled( true );
                mGameStatus = Knapsen::ClientMode;
            }else{
                kDebug() << "WaitForServerDialog - Cancel button.";
                client->disconnectFromHost();
                delete client;
                client = 0;
                
            }
            
        }else{ //newGameDialog.getGameMode() == Knapsen::ServerMode
            kDebug() << "Game mode: server";
            
            mServer = createServer();
            
            connect( mServer, SIGNAL( signalServerEmpty() ), this, SLOT( slotServerEmpty() ) );
            
            if( mServer->listen( QHostAddress::Any, newGameDialog.getServer_ServerPort() ) ){
            
                client->connectToHost( QHostAddress::LocalHost , mServer->serverPort() );
                
                WaitForClientDialog waitForClientDialog;
                waitForClientDialog.setAdminName( client->getName() );
                
                connect( mServer, SIGNAL( signalPlayerConnected( QString ) ), &waitForClientDialog, SLOT( slotPlayerConnected( QString ) ) );
                connect( mServer, SIGNAL( signalPlayerDisconnected( QString ) ), &waitForClientDialog, SLOT( slotPlayerDisconnectedSlot( QString ) ) );
                connect( mServer, SIGNAL( signalServerFull() ), &waitForClientDialog, SLOT( slotServerFull() ) );
                
                if( waitForClientDialog.exec() ){
                    
                    slotStartGame();
                    
                    mGameStatus = Knapsen::ServerMode;
                }else{
                    kDebug() << "WaitForClientDialog - Cancel button";
                    
                    client->disconnectFromHost();
                    delete client;
                    client = 0;
                }
                
            }else{
                kDebug() << "The port is busy!";
                
                delete client;
                client = 0;
            }
            
        }
    }else{
        kDebug() << "KDialog button: Cancel";
    }
}

void MainWindow::closeGameSlot()
{
    kDebug() << "Close game.";
    
    cWidget->clearWidget();
    
    client->disconnectFromHost();
    
    //delete client;
    //client = 0;
    client->deleteLater();
    client = 0;
    
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
    
    settingsDialog->addPage( playerSettingsWidget, i18n( "Player" ), "playersettings.png", "player_settings" );
    
    QWidget *gameSettingsWidget = new QWidget;
    Ui::GameSettingsUi gsu;
    gsu.setupUi( gameSettingsWidget );
    
    settingsDialog->addPage( gameSettingsWidget, i18n( "Game" ), "gamesettings.png", "game_settings" );
    
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
             KMessageBox::error( this, i18n( "The remote host closed the connection." ) ); 
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
            KMessageBox::information( this, i18n( "The opponent disconnected!" ) );
            closeGameSlot();
        }
    }

    if( gameErrorType == Client::ServerIsFull ){
        kDebug() << "Name is busy!";
        KMessageBox::error( this, i18n( "The server is full!" ) );
    }
}

void MainWindow::slotEndRound( QString roundWinnerName, int scores )
{
    endRoundDialog = new EndRoundDialog( this, roundWinnerName, scores );
    QTimer::singleShot( 0, this, SLOT( slotEndRoundExec() ) );
}

void MainWindow::slotEndRoundExec()
{
    connect( client, SIGNAL( signalGameError( Client::GameErrorType ) ), endRoundDialog, SLOT( reject() ) );
    connect( client, SIGNAL( error( QAbstractSocket::SocketError ) ), endRoundDialog, SLOT( reject() ) );
    
    int ret = endRoundDialog->exec();
    
    delete endRoundDialog;
    endRoundDialog = 0;
    
    if( ret ){
        WaitingForOpponentDialog waitingForOpponentDialog( this );
        
        connect( client, SIGNAL( signalGameError( Client::GameErrorType ) ), &waitingForOpponentDialog, SLOT( accept() ) );
        connect( client, SIGNAL( error( QAbstractSocket::SocketError ) ), &waitingForOpponentDialog, SLOT( accept() ) );
        
        connect( client, SIGNAL( signalNewRound() ), &waitingForOpponentDialog, SLOT( accept() ) );
        
        client->startNextRound();
        
        waitingForOpponentDialog.exec();
    }else{
        
        if( mCloseGameAction->isEnabled() ){
            closeGameSlot();
        }
        
    }
}

void MainWindow::slotEndGame( QString gameWinnerName )
{
    endGameDialog = new EndGameDialog( this, gameWinnerName );
    QTimer::singleShot( 0, this, SLOT( slotEndGameExec() ) );
}

void MainWindow::slotEndGameExec()
{
    connect( client, SIGNAL( signalGameError( Client::GameErrorType ) ), endGameDialog, SLOT( reject() ) );
    connect( client, SIGNAL( error( QAbstractSocket::SocketError ) ), endGameDialog, SLOT( reject() ) );
    
    int ret = endGameDialog->exec();
    
    if( ret ){
            
        kDebug() << "Start next game.";
        
        WaitingForOpponentDialog waitingForOpponentDialog( this );
        
        connect( client, SIGNAL( signalGameError( Client::GameErrorType ) ), &waitingForOpponentDialog, SLOT( accept() ) );
        connect( client, SIGNAL( error( QAbstractSocket::SocketError ) ), &waitingForOpponentDialog, SLOT( accept() ) );
        
        connect( client, SIGNAL( signalStartGame() ), &waitingForOpponentDialog, SLOT( accept() ) );
        
        client->startNextGame();
        
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
