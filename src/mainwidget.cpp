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
#include <KDE/KPageDialog>
#include <KDE/KAction>

#include <KDE/KGame/KMessageServer>

#include "mainwidget.h"
#include "table/centralwidget.h"
#include "settings.h"
#include "config-knapsen.h"

#include "engine/bot.h"
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

MainWindow::MainWindow( QWidget* parent ) : KXmlGuiWindow( parent )
{
	server = 0;
	client = 0;
	bot = 0;
	//
	endRoundDialog = 0;
	endGameDialog = 0;
	//
	
	//Initialize the data path (example pictures, icons, etc)
	initializePaths();
	
	//Setup the central widget
    cWidget = new CentralWidget( this );
	setCentralWidget( cWidget );
	
    setupActions();
	
    setupGUI();
}

MainWindow::~MainWindow()
{
}

void MainWindow::initializePaths()
{
	KGlobal::dirs()->addPrefix( KNAPSEN_PREFIX );
	
	QString path;
	path = KGlobal::dirs()->findResource( "appdata", "pics/background.png" );
	
	if( path.isNull() ){
		path = KNAPSEN_PREFIX "/share/apps/knapsen/pics/background.png";
		
		KMessageBox::error( this, i18n( "Knapsen not found the \"%1\"!", path ) );

		close();
	}
	
	path = KGlobal::dirs()->findResource( "appdata", "pics/william-tell.svgz" );
	
	if( path.isNull() ){
		path = KNAPSEN_PREFIX "/share/apps/knapsen/pics/william-tell.svgz";
		
		KMessageBox::error( this, i18n( "Knapsen not found the \"%1\"!", path ) );
		
		close();
	}
	
	path = KGlobal::dirs()->findResource( "appdata", "pics/tigullio-bridge.svg" );
	
	if( path.isNull() ){
		path = KNAPSEN_PREFIX "/share/apps/knapsen/pics/tigullio-bridge.svg";
		
		KMessageBox::error( this, i18n( "Knapsen not found the \"%1\"!", path ) );
		
		close();
	}
}

void MainWindow::setupActions()
{
	//Game menu
	KStandardGameAction::gameNew(					this, SLOT( newGameSlot()  ), actionCollection() );
	mCloseGameAction = KStandardGameAction::end(	this, SLOT(closeGameSlot() ), actionCollection() );
	KStandardGameAction::quit(						this, SLOT( close()        ), actionCollection() );
	
	mCloseGameAction->setEnabled( false );
	
	//Settings menu
	KStandardAction::preferences(this, SLOT(optionsPreferences()), actionCollection());
}

void MainWindow::setServerConfig()
{
	//Set who start the game
	if( Settings::localPlayer() ){
		server->setWhoStartGame( Knapsen::AdminPlayer );
	}else if( Settings::opponentPlayer() ){
		server->setWhoStartGame( Knapsen::OpponentPlayer );
	}else{ // if( Settings::randomPlayer() )
		server->setWhoStartGame( Knapsen::RandomPlayer );
	}

	//Set the size of deck
	if( Settings::sizeOfDeck20() ){
		//server->setSizeOfDeck( Server::Number20 );
		server->setSizeOfDeck( 20 );
	}else{ // if( Settings::sizeOfDeck24 )
		//server->setSizeOfDeck( Server::Number24 );
		server->setSizeOfDeck( 24 );
	}

	//Set cards in hand
	if( Settings::cardsInHand5() ){
		//server->setCardsInHand( Server::Number5 );
		server->setNumberOfCardsInHand( 5 );
	}else{ // if( Settings::cardsInHand6 )
		//server->setCardsInHand( Server::Number6 );
		server->setNumberOfCardsInHand( 6 );
	}

	//Set type of cards
	if( Settings::germanSuits() ){
		server->setTypeOfCards( Knapsen::GermanSuits );
	}else{ // if( Settings::frenchSuits() )
		server->setTypeOfCards( Knapsen::FrenchSuits );
	}
}

void MainWindow::setGameSignals()
{
	connect( client, SIGNAL( signalInitialize( QString, QString, Knapsen::TypeOfCards, int ) ),	cWidget, SLOT( slotInitialize( QString, QString, Knapsen::TypeOfCards, int ) ) );
	connect( client, SIGNAL( signalNewPlayerCard( int, QString ) ),								cWidget, SLOT( slotNewPlayerCard( int, QString ) ) );
	connect( client, SIGNAL( signalNewOpponentCardId( int ) ),									cWidget, SLOT( slotNewOpponentCardId( int ) ) );
	connect( client, SIGNAL( signalNewTrumpCard( QString ) ),									cWidget, SLOT( slotNewTrumpCard( QString ) ) );
	connect( client, SIGNAL( signalTrumpCardHide() ),											cWidget, SLOT( slotTrumpCardHide() ) );
	connect( client, SIGNAL( signalTrumpCardSelectableChanged( bool ) ),						cWidget, SLOT( slotTrumpCardSelectableChanged( bool ) ) );
	connect( client, SIGNAL( signalPlayerCardSelectableChanged( int, bool ) ),					cWidget, SLOT( slotPlayerCardSelectableChanged( int, bool ) ) );
	connect( client, SIGNAL( signalCentralCardChanged( int, QString ) ),						cWidget, SLOT( slotCentralCardChanged( int, QString ) ) );
	connect( client, SIGNAL( signalOpponentSelectedCardId( int ) ),								cWidget, SLOT( slotOpponentSelectedCardId( int ) ) );
	connect( client, SIGNAL( signalOpponentTricksChanged( int ) ),								cWidget, SLOT( slotOpponentTricksChanged( int ) ) );
	connect( client, SIGNAL( signalOpponentScoresChanged( int ) ),								cWidget, SLOT( slotOpponentScoresChanged( int ) ) );
	connect( client, SIGNAL( signalPlayerTricksChanged( int ) ),								cWidget, SLOT( slotPlayerTricksChanged( int ) ) );
	connect( client, SIGNAL( signalPlayerScoresChanged( int ) ),								cWidget, SLOT( slotPlayerScoresChanged( int ) ) );
	connect( client, SIGNAL( signalHideDeck() ),												cWidget, SLOT( slotHideDeck() ) );
	connect( client, SIGNAL( signalCloseDeck() ),												cWidget, SLOT( slotCloseDeck() ) );
	connect( client, SIGNAL( signalTwentyButtonVisible( bool ) ),								cWidget, SLOT( slotTwentyButtonVisible( bool ) ) );
	connect( client, SIGNAL( signalFortyButtonVisible( bool ) ),								cWidget, SLOT( slotFortyButtonVisible( bool ) ) );
	connect( client, SIGNAL( signalCloseButtonVisible( bool ) ),								cWidget, SLOT( slotCloseButtonVisible( bool ) ) );
	connect( client, SIGNAL( signalShowOpponentCards( int, QString, int, QString ) ),			cWidget, SLOT( slotShowOpponentCards( int, QString, int, QString ) ) );
	connect( client, SIGNAL( signalNewRound() ),												cWidget, SLOT( slotNewRound() ) );
	connect( client, SIGNAL( signalStartGame() ),												cWidget, SLOT( slotStartGame() ) );
	
	//
	connect( client, SIGNAL( error( QAbstractSocket::SocketError ) ),							this,	SLOT( slotSocketError( QAbstractSocket::SocketError ) ) );
	connect( client, SIGNAL( signalGameError( Client::GameErrorType ) ),						this,	SLOT( slotGameError( Client::GameErrorType ) ) );
	//
	connect( client, SIGNAL( signalEndRound( QString, int ) ),									this,	 SLOT( slotEndRound( QString, int ) ) );
	connect( client, SIGNAL( signalEndGame( QString ) ),										this,	 SLOT( slotEndGame( QString ) ) );
	
	connect( cWidget, SIGNAL( signalSelectedCardId( int ) ),									client, SLOT( slotSelectedCardId( int ) ) );
	connect( cWidget, SIGNAL( signalSelectedTrumpCard() ),										client, SLOT( slotSelectedTrumpCard() ) );
	connect( cWidget, SIGNAL( signalTwentyButtonClicked() ),									client, SLOT( slotTwentyButtonClicked() ) );
	connect( cWidget, SIGNAL( signalFortyButtonClicked() ),										client, SLOT( slotFortyButtonClicked() ) );
	connect( cWidget, SIGNAL( signalCloseButtonClicked() ),										client, SLOT( slotCloseButtonClicked() ) );
	connect( cWidget, SIGNAL( signalHideShowedOpponentCards() ),								client, SLOT( slotProcessCommands() ) );
	
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
		
		client = new Client;
		client->setName( Settings::playerName() );
		
		setGameSignals();
		
		if( newGameDialog.getGameMode() == NewGameDialog::LocalMode ){
			kDebug() << "Game mode: local";
			
			server = new Server/*( this )*/;
			connect( server, SIGNAL( signalServerFull() ), this, SLOT( slotStartGame() ) );
			connect( server, SIGNAL( signalServerEmpty() ), this, SLOT( slotServerEmpty() ) );
			
			if( server->listen( QHostAddress::LocalHost ) ){
			
				kDebug() << "Server port:" << server->serverPort();

				server->setAdminName( client->getName() );
				
				//
				setServerConfig();
				//
			
				client->connectToHost( "127.0.0.1", server->serverPort() );

				bot = new Bot( this );
				bot->setName( Settings::botName() );
			
				bot->connectToHost( "127.0.0.1", server->serverPort() );
			
			}else{
				kDebug() << "Server listen error!";
				
				delete client;
				client = 0;
			}
				
		}else if( newGameDialog.getGameMode() == NewGameDialog::ClientMode ){
			kDebug() << "Game mode: client";
			
			//
			client->setName( "Client" );
			//
			
			WaitForServerDialog waitForServerDialog;
			
			connect( client, SIGNAL( signalStartGame() ), &waitForServerDialog, SLOT( slotStartGame() ) );
			connect( client, SIGNAL( error( QAbstractSocket::SocketError ) ), &waitForServerDialog, SLOT( reject() ) );
			connect( client, SIGNAL( signalGameError( Client::GameErrorType ) ), &waitForServerDialog, SLOT( reject() ) );
			
			
			client->connectToHost( newGameDialog.getClient_ServerAddress(), newGameDialog.getClient_ServerPort() );
			
			if( waitForServerDialog.exec() ){
				mCloseGameAction->setEnabled( true );
			}else{
				kDebug() << "WaitForServerDialog - Cancel button.";
				client->disconnectFromHost();
				delete client;
				client = 0;
			
			}
			
			
		}else{ //newGameDialog.getGameMode() == NewGameDialog::ServerMode
			kDebug() << "Game mode: server";
			
			server = new Server/*( this )*/;
			
			connect( server, SIGNAL( signalServerEmpty() ), this, SLOT( slotServerEmpty() ) );
			
			if( server->listen( QHostAddress::Any, newGameDialog.getServer_ServerPort() ) ){

				server->setAdminName( client->getName() );
				
				//
				setServerConfig();
				//
				
				client->connectToHost( "127.0.0.1", server->serverPort() );
				
				WaitForClientDialog waitForClientDialog;
				waitForClientDialog.setAdminName( client->getName() );
				
				connect( server, SIGNAL( signalPlayerConnected( QString ) ), &waitForClientDialog, SLOT( slotPlayerConnected( QString ) ) );
				connect( server, SIGNAL( signalPlayerDisconnected( QString ) ), &waitForClientDialog, SLOT( slotPlayerDisconnectedSlot( QString ) ) );
				connect( server, SIGNAL( signalServerFull() ), &waitForClientDialog, SLOT( slotServerFull() ) );
				
				if( waitForClientDialog.exec() ){

					slotStartGame();
					
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
	
	settingsDialog->addPage( playerSettingsWidget, i18n( "Player" ), "player_settings" );
	
	QWidget *gameSettingsWidget = new QWidget;
	Ui::GameSettingsUi gsu;
	gsu.setupUi( gameSettingsWidget );
	
	settingsDialog->addPage( gameSettingsWidget, i18n( "Game" ), "game_settings" );
	
	settingsDialog->setAttribute( Qt::WA_DeleteOnClose );
	
	settingsDialog->show();
}

void MainWindow::slotStartGame()
{
	kDebug() << "Start game.";
	
	server->startGame();
	
	mCloseGameAction->setEnabled( true );
}

void MainWindow::slotServerEmpty()
{
	kDebug() << "Server is empty!";
	
	if( bot ){
		delete bot;
		bot = 0;
	}
	
	server->close();
	server->deleteLater();
	server = 0;
}

void MainWindow::slotSocketError( QAbstractSocket::SocketError socketError )
{
	kDebug() << socketError;
	
	if( socketError == QAbstractSocket::ConnectionRefusedError ){
		KMessageBox::error( this, i18n( "The connection was refused!" ) );
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
		
		if( mCloseGameAction->isEnabled() ){
			KMessageBox::information( this, i18n( "The opponent disconnected!" ) );
			closeGameSlot();
		}
		
	}
}

void MainWindow::slotEndRound( QString roundWinnerName, int scores )
{
	endRoundDialog = new EndRoundDialog( this, roundWinnerName, scores );
	QTimer::singleShot( 0, this, SLOT( slotEndRoundExec() ) );
}

void MainWindow::slotEndRoundExec()
{	
	endRoundDialog->exec();
	
	delete endRoundDialog;
	endRoundDialog = 0;
	
	WaitingForOpponentDialog waitingForOpponentDialog( this );
	
	connect( client, SIGNAL( signalNewRound() ), &waitingForOpponentDialog, SLOT( accept() ) );
	
	client->startNextRound();
	
	waitingForOpponentDialog.exec();
	
}

void MainWindow::slotEndGame( QString gameWinnerName )
{
	endGameDialog = new EndGameDialog( this, gameWinnerName );
	QTimer::singleShot( 0, this, SLOT( slotEndGameExec() ) );
}

void MainWindow::slotEndGameExec()
{
	int ret = endGameDialog->exec();
	
	if( ret ){
		
		kDebug() << "Start next game.";
		
		WaitingForOpponentDialog waitForOpponentDialog( this );
		
		connect( client, SIGNAL( signalStartGame() ), &waitForOpponentDialog, SLOT( accept() ) );
		
		client->startNextGame();
		
		waitForOpponentDialog.exec();
		
	}else{
		
		kDebug() << "Close game.";
		
		closeGameSlot();
		
	}
	
	delete endGameDialog;
	endGameDialog = 0;
	
}
