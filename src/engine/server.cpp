#include <QtCore/QTimer>
#include <KDE/KDebug>
#include "gamesequence.h"
#include "deck.h"
#include "player.h"
#include "server.h"

//Only 2 players game, 4 players maybe in the future...
const int MAX_PLAYERS = 2;

const int MAX_CENTRAL_CARDS_SIZE = 2;

Server::Server( QObject* parent ) :
	QTcpServer( parent ),
	mAdminName( "" ),
	mTwentyButtonClickedThisTurn( false ),
	mFortyButtonClickedThisTurn( false )
{
	kDebug() << "Initialize.";

	mGameSequence = new GameSequence;
	mGameSequence->setPlayerList( &mPlayerList );
	
	//Defaults
	//mWhoStartGame = Knapsen::AdminPlayer;
	//mSizeOfDeck = Server::Number20;
	mSizeOfDeck = 20;
	//mCardsInHand = Server::Number5;
	mNumberOfCardsInHand = 5;
	mTypeOfCards = Knapsen::GermanSuits;
	
	mDeck = 0;
}

Server::~Server()
{
	kDebug() << "Server is deleting.";
	if( mGameSequence ){
		delete mGameSequence;
	}
	if( mDeck ){
		delete mDeck;
	}
}

void Server::slotNewPlayer( Player* player )
{
	kDebug() << player->getName();
	
	if( mPlayerList.size() < MAX_PLAYERS ){
		
		for( int i = 0; i < mPlayerList.size(); ++i ){
			if( mPlayerList.at(i)->getName() == player->getName() ){
				kDebug() << "The \"" << player->getName() << "\" name is busy.";
				return;
			}
		}
		
		mPlayerList.append( player );
		connect( player, SIGNAL( signalPlayerDisconnected( Player* ) ),	this, SLOT( slotPlayerDisconnected( Player* ) ) );
		connect( player, SIGNAL( signalSelectedCard( Card ) ),			this, SLOT( slotPlayerSelectedCard( Card ) ) ); 
		connect( player, SIGNAL( signalTwentyButtonClicked() ),			this, SLOT( slotPlayerTwentyButtonClicked() ) );
		connect( player, SIGNAL( signalFortyButtonClicked() ),			this, SLOT( slotPlayerFortyButtonClicked() ) );
		emit signalPlayerConnected( player->getName() );
		
		if( mPlayerList.size() == MAX_PLAYERS ){
			kDebug() << "The server is full.";
			
			emit signalServerFull();
		}
		
	}else{
		kDebug() << "The server is full, no free slot.";
	}
	
}

void Server::slotPlayerDisconnected( Player* player )
{
	kDebug() << player->getName() << "disconnected.";
	
	int i = 0;
	while( mPlayerList.at(i)->getName() != player->getName() ){
		i++;
	}
	
	mPlayerList.removeAt(i);
	
	//
	player->disconnectFromHost();
	player->deleteLater();
	//
	
	emit signalPlayerDisconnected( player->getName() );
	
	if( mPlayerList.isEmpty() ){
		kDebug() << "Server is empty!";
		emit signalServerEmpty();
	}else{
		
		for( i = 0; i < mPlayerList.size(); ++i ){
			mPlayerList.at( i )->sendOpponentDisconnected();
		}
		
	}
}

void Server::slotPlayerSelectedCard( Card selectedCard )
{
	kDebug() << "Selected card:" << selectedCard.getCardText( mTypeOfCards );
	
	mCentralCards.append( selectedCard );
	
	if( mCentralCards.size() != MAX_CENTRAL_CARDS_SIZE ){
		kDebug() << "Next player step.";
		
		Player* nextPlayer = mGameSequence->getNextPlayer();
		mGameSequence->setCurrentPlayer( nextPlayer );
		
		//nextPlayer->sendSelectableAllCards();
		if( mDeck->getDeckSize() > 0 ){
			nextPlayer->sendSelectableAllCards();
		}else{ // mDeck->getDeckSize() == 0
			nextPlayer->sendSelectableCertainCards();
		}
		
	}else{
		QTimer::singleShot( 1000, this, SLOT( slotCheckCentralCards() ) );
	}
}

void Server::slotPlayerTwentyButtonClicked()
{
	kDebug() << mGameSequence->getCurrentPlayer()->getName() << "Clicked to twenty button.";
	mTwentyButtonClickedThisTurn = true;
}

void Server::slotPlayerFortyButtonClicked()
{
	kDebug() << mGameSequence->getCurrentPlayer()->getName() << "Clicked to forty button.";
	mFortyButtonClickedThisTurn = true;
}

void Server::slotCheckCentralCards()
{
	
	kDebug() << "Check the central cards.";
		
	int centralCard1Point = mCentralCards.at(0).getCardPoint();
	int centralCard2Point = mCentralCards.at(1).getCardPoint();
		
	kDebug() << "centralCard1Point" << centralCard1Point;
	kDebug() << "centralCard2Point" << centralCard2Point;
		
	bool currentPlayerStartNextTurn;
		
	if( mTrumpCard.getCardSuit() == mCentralCards.at(0).getCardSuit() && mTrumpCard.getCardSuit() != mCentralCards.at(1).getCardSuit() ){
		currentPlayerStartNextTurn = false;
	}else if( mTrumpCard.getCardSuit() != mCentralCards.at(0).getCardSuit() && mTrumpCard.getCardSuit() == mCentralCards.at(1).getCardSuit() ){
		currentPlayerStartNextTurn = true;
	}else if( mCentralCards.at(0).getCardSuit() == mCentralCards.at(1).getCardSuit() ){
		
		if( centralCard1Point < centralCard2Point ){
			currentPlayerStartNextTurn = true;
		}else{
			currentPlayerStartNextTurn = false;
		}
		
	}else{ // mCentralCards.at(0).getCardType != mCentralCards.at(1).getCardType
		currentPlayerStartNextTurn = false;
	}
		
		
	if( !currentPlayerStartNextTurn ){
		mGameSequence->setCurrentPlayer( mGameSequence->getNextPlayer() );
	}
		
	kDebug() << mGameSequence->getCurrentPlayer()->getName() << "get" << centralCard1Point+centralCard2Point << "points.";
	mGameSequence->getCurrentPlayer()->addTricks( centralCard1Point + centralCard2Point );
		
	if( mGameSequence->isRoundOver() ){
		kDebug() << "--- Round end! ---";
	}else{
		kDebug() << "Start next turn";
		
		//Clear central cards
		mCentralCards.clear();
		for( int i = 0; i < mPlayerList.size(); ++i ){
			mPlayerList.at( i )->sendClearCentralCards();
		}
		
		//If the dech have card yet, then add new cards to players, frist who won the last turn
		if( mDeck->getDeckSize() > 0 ){
			mGameSequence->getCurrentPlayer()->sendNewCard( mDeck->getCard() );
			
			if( mDeck->getDeckSize() > 0 ){
				mGameSequence->getNextPlayer()->sendNewCard( mDeck->getCard() );
			}else{
				mGameSequence->getNextPlayer()->sendNewCard( mTrumpCard );
				mTrumpCard = Card();
				
				for( int i = 0; i < mPlayerList.size(); ++i ){
					mPlayerList.at( i )->sendClearTrumpCard();
				}
				
			}
			
		}
		
		mGameSequence->getCurrentPlayer()->sendSelectableAllCards();
		
	}
	
}

void Server::incomingConnection( int socketDescriptor )
{
	kDebug() << "socketDescriptor:" << socketDescriptor;
	
	Player* player = new Player( this );
	player->setSocketDescriptor( socketDescriptor );
	
	//If the player have name, then will be a player
	connect( player, SIGNAL( signalNewPlayer( Player* ) ), this, SLOT( slotNewPlayer( Player* ) ) );
}

void Server::setWhoStartGame( Knapsen::WhoStartGame whoStartGame )
{
	mGameSequence->setWhoStartGame( whoStartGame );
}

void Server::startGame()
{
	kDebug() << "Start game.";
	
	//Initialize deck
	mDeck = new Deck( mSizeOfDeck, mTypeOfCards );
	mDeck->buildDeck();
	
	//To all players
	for( int i = 0; i < mPlayerList.size(); ++i ){
		
		//Set opponent
		for( int j = 0; j < mPlayerList.size(); ++j ){
			if( j != i ){
				mPlayerList.at( i )->setOpponent( mPlayerList.at( j ) );
			}
		}
		
		//Send the opponent player's name
		mPlayerList.at( i )->sendOpponentName();
		
		//Send the type of cards
		mPlayerList.at( i )->sendTypeOfCard( mTypeOfCards );
		//Send the size of deck
		mPlayerList.at( i )->sendSizeOfDeck( mSizeOfDeck );
		//Send number of cards in hand
		mPlayerList.at( i )->sendNumberOfCardsInHand( mNumberOfCardsInHand );
		
		//Initialize table
		mPlayerList.at( i )->sendInitializeTable();
	}
	
	//Send cards to players
	for( int i = 0; i < mNumberOfCardsInHand; ++i ){
		
		for( int j = 0; j < mPlayerList.size(); ++j ){
			mPlayerList.at( j )->sendNewCard( mDeck->getCard() );
		}
		
		//Get trump card
		if( i == 2 ){
			mTrumpCard = mDeck->getCard();
			
			for( int j = 0; j < mPlayerList.size(); ++j ){
				mPlayerList.at( j )->sendNewTrumpCard( mTrumpCard );
			}
			
		}
		
	}
	
	if( mGameSequence->getCurrentPlayer()->haveRegularMarriages() ){
		kDebug() << mGameSequence->getCurrentPlayer()->getName() << "have regular marriages.";
		mGameSequence->getCurrentPlayer()->sendTwentyButtonVisible();
	}
	
	if( mGameSequence->getCurrentPlayer()->haveTrumpMarriages() ){
		kDebug() << mGameSequence->getCurrentPlayer()->getName() << "have trump marriages.";
		mGameSequence->getCurrentPlayer()->sendFortyButtonVisible();
	}
	
	mGameSequence->getCurrentPlayer()->sendSelectableAllCards();
	
	//And finally send start the game
	for( int i = 0; i < mPlayerList.size(); ++i ){
		mPlayerList.at( i )->sendStartGame();
	}
}
