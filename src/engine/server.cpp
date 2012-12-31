#include <QtCore/QTimer>
#include <KDE/KDebug>
#include "gamesequence.h"
#include "deck.h"
//
#include "centralcards.h"
//
//
#include "trumpcard.h"
//
//
#include "bot.h"
//
#include "player.h"
#include "server.h"

//Only 2 players game, 4 players maybe in the future...
const int MAX_PLAYERS = 2;

//const int MAX_CENTRAL_CARDS_SIZE = 2;

Server::Server( QObject* parent ) :
	QTcpServer( parent ),
	//mAdminName( "" ),
	mTwentyButtonClickedThisTurn( false ),
	mFortyButtonClickedThisTurn( false ),
	mClickedToCloseButtonThisTurn( false ),
	mOpponentHaveNotTricksBeforePlayerClickedToCloseButton( false )
{
	kDebug() << "Initialize.";

	mGameSequence = new GameSequence;
	mGameSequence->setPlayerList( &mPlayerList );
	
	//Defaults
	mSizeOfDeck = 20;
	mNumberOfCardsInHand = 5;
	mTypeOfCards = Knapsen::GermanSuits;
	
	//
	mBot = 0;
	//
	
	//
	//mCentralCards = new CentralCards;
	mCentralCards = 0;
	//
	
	//
	mTrumpCard = 0;
	//
	
	mDeck = 0;
	mPlayerWhoClickedToCloseButtonThisRound = 0;
	mWaitingMarriage = 0;
}

Server::~Server()
{
	kDebug() << "Server is deleting.";
	if( mBot ){
		delete mBot;
	}
	if( mGameSequence ){
		delete mGameSequence;
	}
	if( mDeck ){
		delete mDeck;
	}
	if( mCentralCards ){
		delete mCentralCards;
	}
	if( mTrumpCard ){
		delete mTrumpCard;
	}
}

void Server::newGame()
{
	kDebug() << "Start new game.";
	
	
	for( int i = 0; i < mPlayerList.size(); ++i ){
		mPlayerList.at( i )->sendNewGame();
	}
	
	mCentralCards = new CentralCards;
	
	newRound();
	
}

void Server::newRound()
{
	kDebug() << "Start new round.";
	
	//Build deck
	mDeck->buildDeck();
	
	//Clear central cards
	mCentralCards->clear();
	
	//Clear variables
	mOpponentHaveNotTricksBeforePlayerClickedToCloseButton = false;
	mPlayerWhoClickedToCloseButtonThisRound = 0;
	mTwentyButtonClickedThisTurn = false;
	mFortyButtonClickedThisTurn = false;
	mClickedToCloseButtonThisTurn = false;
	
	//Clear cards, tricks
	for( int i = 0; i < mPlayerList.size(); ++i ){
		mPlayerList.at( i )->sendNewRound();
	}
	
	//Send cards to players
	for( int i = 0; i < mNumberOfCardsInHand; ++i ){
		
		for( int j = 0; j < mPlayerList.size(); ++j ){
			//mPlayerList.at( j )->sendNewCard( mDeck->getCard() );
			int cardId = mPlayerList.at( j )->sendNewCard( mDeck->getCard() );
			
			for( int k = 0; k < mPlayerList.size(); ++k ){
				if( mPlayerList.at( k ) != mPlayerList.at( j ) ){
					mPlayerList.at( k )->sendNewOpponentCard( cardId );
				}
			}
		}
		
		//Get trump card
		if( i == 2 ){
			//mTrumpCard = mDeck->getCard();
			mTrumpCard->addNewCard( mDeck->getCard() );
			
			for( int j = 0; j < mPlayerList.size(); ++j ){
				mPlayerList.at( j )->sendNewTrumpCard( mTrumpCard );
			}
			
		}
		
	}
	
	Player* currentPlayer = mGameSequence->getCurrentPlayer();
	
	//if( currentPlayer->haveRegularMarriages() ){
	if( currentPlayer->haveRegularMarriages( mTrumpCard ) ){
		kDebug() << currentPlayer->getName() << "have regular marriages.";
		currentPlayer->setTwentyButtonVisible( true );
		currentPlayer->sendTwentyButtonVisible();
	}
	
	//if( currentPlayer->haveTrumpMarriages() ){
	if( currentPlayer->haveTrumpMarriages( mTrumpCard ) ){
		kDebug() << currentPlayer->getName() << "have trump marriages.";
		currentPlayer->setFortyButtonVisible( true );
		currentPlayer->sendFortyButtonVisible();
	}
	
	//if( currentPlayer->canChangeTrumpCard() ){
	if( currentPlayer->canChangeTrumpCard( mTrumpCard ) ){
		kDebug() << currentPlayer->getName() << "can change trump card.";
		currentPlayer->sendSelectableTrumpCard();
	}
	
	currentPlayer->sendCloseButtonVisible();
	
	currentPlayer->sendSelectableAllCards();
	for( int i = 0; i < mPlayerList.size(); ++i ){
		if( mPlayerList.at( i ) != currentPlayer ){
			mPlayerList.at( i )->sendOpponentInAction();
		}
	}
	
}

void Server::roundOver()
{
	kDebug() << "---- Round Over ----";
	
	Player *currentPlayer = mGameSequence->getCurrentPlayer();
	Player *nextPlayer = mGameSequence->getNextPlayer();
	
	Player *winnerPlayer = 0; //Initialize with 0, becouse the compiler write warning.
	Player *looserPlayer = 0; // --- || ---
	
	//Who win the round
	if( mPlayerWhoClickedToCloseButtonThisRound ){
		
		if( mPlayerWhoClickedToCloseButtonThisRound == currentPlayer && currentPlayer->getTricks() >= 66 ){
			winnerPlayer = currentPlayer;
			looserPlayer = nextPlayer;
		}else if( mPlayerWhoClickedToCloseButtonThisRound == nextPlayer && nextPlayer->getTricks() >= 66 ){
			winnerPlayer = nextPlayer;
			looserPlayer = currentPlayer;
		}else if( mPlayerWhoClickedToCloseButtonThisRound == currentPlayer ){
			winnerPlayer = nextPlayer;
			looserPlayer = currentPlayer;
		}else{
			winnerPlayer = currentPlayer;
			looserPlayer = nextPlayer;
		}
		
	}else{
		
		if( currentPlayer->getTricks() >= 66 ){
			winnerPlayer = currentPlayer;
			looserPlayer = nextPlayer;
		}else if( nextPlayer->getTricks() >= 66 ){
			winnerPlayer = nextPlayer;
			looserPlayer = currentPlayer;
		}else if( currentPlayer->getNumberOfCardsInHandNow() == 0 ){
			winnerPlayer = currentPlayer;
			looserPlayer = nextPlayer;
		}
		
	}
	
	//Winning scores
	int scores = 0;
	
	if( mPlayerWhoClickedToCloseButtonThisRound == winnerPlayer ){
		
		if( looserPlayer->getTricks() == 0 ){
			scores = 3;
		}else if( looserPlayer->getTricks() < 33 ){
			scores = 2;
		}else{ //  33 <= looserPlayer->getTricks() && looserPlayer->getTricks() < 66
			scores = 1;
		}
		
	}else if( mPlayerWhoClickedToCloseButtonThisRound == looserPlayer ){
		
		//scores = 2;
		if( mOpponentHaveNotTricksBeforePlayerClickedToCloseButton ){
			scores = 3;
		}else{
			scores = 2;
		}
		
		
	}else{ // !mPlayerWhoClickedToCloseButtonThisRound

		if( looserPlayer->getTricks() == 0 ){
			scores = 3;
		}else if( looserPlayer->getTricks() < 33 ){
			scores = 2;
		}else{ //  33 <= looserPlayer->getTricks() && looserPlayer->getTricks() < 66
			scores = 1;
		}

	}
	
	kDebug() << winnerPlayer->getName() << "win the round with" << scores << "scores.";
	winnerPlayer->addScores( scores );
	
	if( mGameSequence->isGameOver() ){
		
		for( int i = 0; i < mPlayerList.size(); ++i ){
			mPlayerList.at( i )->sendEndGame( winnerPlayer->getName() );
		}
		
	}else{
		
		for( int i = 0; i < mPlayerList.size(); ++i ){
			mPlayerList.at( i )->sendEndRound( winnerPlayer->getName(), scores );
		}
		
	}
	
	for( int i = 0; i < mPlayerList.size(); ++i ){
		mPlayerList.at( i )->sendCommandsEnd();
	}
	
}

void Server::slotNewPlayer( Player* player )
{
	kDebug() << player->getName();
	
	if( mPlayerList.size() < MAX_PLAYERS ){
		
		for( int i = 0; i < mPlayerList.size(); ++i ){
			if( mPlayerList.at(i)->getName() == player->getName() ){
				kDebug() << "The \"" << player->getName() << "\" name is busy.";
				player->sendNameIsBusy();
				return;
			}
		}
		
		mPlayerList.append( player );
		//connect( player, SIGNAL( signalPlayerDisconnected( Player* ) ),	this, SLOT( slotPlayerDisconnected( Player* ) ) );
		connect( player, SIGNAL( signalSelectedCard( Card*, int ) ),		this, SLOT( slotPlayerSelectedCard( Card*, int ) ) ); 
		connect( player, SIGNAL( signalTwentyButtonClicked() ),				this, SLOT( slotPlayerTwentyButtonClicked() ) );
		connect( player, SIGNAL( signalFortyButtonClicked() ),				this, SLOT( slotPlayerFortyButtonClicked() ) );
		connect( player, SIGNAL( signalCloseButtonClicked() ),				this, SLOT( slotPlayerClickedToCloseButton() ) );
		//connect( player, SIGNAL( signalChangedTrumpCard( Card ) ),		this, SLOT( slotPlayerChangedTrumpCard( Card ) ) );
		connect( player, SIGNAL( signalChangeTrumpCard( Player* ) ),		this, SLOT( slotPlayerChangeTrumpCard( Player* ) ) );
		connect( player, SIGNAL( signalStartNextRound( Player* ) ),			this, SLOT( slotPlayerWantStartNextRound( Player* ) ) );
		connect( player, SIGNAL( signalStartNextGame( Player* ) ),			this, SLOT( slotPlayerWantStartNextGame( Player* ) ) );
		
		emit signalPlayerConnected( player->getName() );
		
		if( mPlayerList.size() == MAX_PLAYERS ){
			kDebug() << "The server is full.";
			
			emit signalServerFull();
		}
		
	}else{
		kDebug() << "The server is full, no free slot.";
		player->sendServerIsFull();
		//player->deleteLater();
	}
	
}

void Server::slotPlayerDisconnected( Player* player )
{
	/*kDebug() << player->getName() << "disconnected.";
	
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
	
	//
	if( mBot ){
		mBot->disconnectFromHost();
		mBot->deleteLater();
		mBot = 0;
	}
	//
	
	if( mPlayerList.isEmpty() ){
		kDebug() << "Server is empty!";
		emit signalServerEmpty();
	}else{
		
		for( i = 0; i < mPlayerList.size(); ++i ){
			mPlayerList.at( i )->sendOpponentDisconnected();
		}
		
	}*/
	
	kDebug() << player->getName() << "disconnected.";
	
	const int INVALID_ID = -1;
	
	int id = INVALID_ID;
	
	for( int i = 0; i < mPlayerList.size(); ++i ){
		if( player == mPlayerList.at( i ) ){
			id = i;
		}
	}
	
	if( id != INVALID_ID ){
		
		mPlayerList.removeAt( id );
	
		//Delete player
		player->disconnectFromHost();
		player->deleteLater();
		
		emit signalPlayerDisconnected( player->getName() );
		
		//If the opponent is bot, then delete bot
		if( mBot ){
			mBot->disconnectFromHost();
			mBot->deleteLater();
			mBot = 0;
		}
		
		if( mPlayerList.isEmpty() ){
			kDebug() << "Server is empty!";
			emit signalServerEmpty();
		}else{
			
			for( int i = 0; i < mPlayerList.size(); ++i ){
				mPlayerList.at( i )->sendOpponentDisconnected();
			}
			
		}
		
	}else{ // id == INVALID_ID
		player->disconnectFromHost();
		player->deleteLater();
	}
	
}

/*void Server::slotPlayerSelectedCard( Card selectedCard, int cardPosition )
{
	//kDebug() << "Selected card:" << selectedCard.getCardText( mTypeOfCards );
	kDebug() << "Selected card:" << selectedCard.getCardText();
	
	//mCentralCards.append( selectedCard );
	mCentralCards->add( selectedCard );
	
	Player* nextPlayer = mGameSequence->getNextPlayer();
	
	//if( mCentralCards.size() != MAX_CENTRAL_CARDS_SIZE ){
	if( !mCentralCards->isFull() ){
		kDebug() << "Next player step.";
		
		Player* currentPlayer = mGameSequence->getCurrentPlayer();
		//Player* nextPlayer = mGameSequence->getNextPlayer();
		
		//If the previous player clicked to twenty/forty button, then show that card to current player
		if( mTwentyButtonClickedThisTurn ){
			kDebug() << "Pair this card position:" << currentPlayer->getPositionOfPairOfCard( selectedCard );
			int posOfPairOfCard = currentPlayer->getPositionOfPairOfCard( selectedCard );
			//nextPlayer->sendVisibleOpponentCards( cardPosition, selectedCard, posOfPairOfCard, currentPlayer->getCard( posOfPairOfCard ) );
			nextPlayer->sendVisibleOpponentCards( cardPosition, selectedCard, posOfPairOfCard, currentPlayer->getCard( posOfPairOfCard ) );
		
			
			//
			if( currentPlayer->getTricks() > 0 ){
				
				currentPlayer->addTricks( 20 );
				
				if( mGameSequence->isRoundOver() ){
					roundOver();
					return;
				}
				
			}else{ //currentPlayer->getTricks() == 0
				mWaitingMarriage = new QPair< Player*, int >;
				mWaitingMarriage->first = currentPlayer;
				mWaitingMarriage->second = 20;
			}
			
			mTwentyButtonClickedThisTurn = false;
		}
		
		if( mFortyButtonClickedThisTurn ){
			kDebug() << "Pair this card position:" << currentPlayer->getPositionOfPairOfCard( selectedCard );
			int posOfPairOfCard = currentPlayer->getPositionOfPairOfCard( selectedCard );
			//nextPlayer->sendVisibleOpponentCards( cardPosition, selectedCard, posOfPairOfCard, currentPlayer->getCard( posOfPairOfCard ) );
			nextPlayer->sendVisibleOpponentCards( cardPosition, selectedCard, posOfPairOfCard, currentPlayer->getCard( posOfPairOfCard ) );
			
			
			//
			if( currentPlayer->getTricks() > 0 ){
				
				currentPlayer->addTricks( 40 );
				
				if( mGameSequence->isRoundOver() ){
					roundOver();
					return;
				}
				
			}else{ //currentPlayer->getTricks() == 0
				mWaitingMarriage = new QPair< Player*, int >;
				mWaitingMarriage->first = currentPlayer;
				mWaitingMarriage->second = 40;
			}
			
			mFortyButtonClickedThisTurn = false;
		}
		
		if( mClickedToCloseButtonThisTurn ){
			nextPlayer->sendOpponentClickedToCloseButton();
		}
		
		nextPlayer->sendOpponentSelectedCardId( cardPosition );
		nextPlayer->sendOpponentAddNewCentralCard( selectedCard );
		
		mGameSequence->setCurrentPlayer( nextPlayer );
		
		//if( !mClickedToCloseButtonThisRound && mDeck->getDeckSize() > 0 ){
		if( !mPlayerWhoClickedToCloseButtonThisRound && mDeck->getDeckSize() > 0 ){
			nextPlayer->sendSelectableAllCards();
			for( int i = 0; i < mPlayerList.size(); ++i ){
				if( mPlayerList.at( i ) != currentPlayer ){
					mPlayerList.at( i )->sendOpponentInAction();
				}
			}
		}else{ // mDeck->getDeckSize() == 0
			//nextPlayer->sendSelectableCertainCards();
			nextPlayer->sendSelectableCertainCards( mCentralCards, mTrumpCard );
			for( int i = 0; i < mPlayerList.size(); ++i ){
				if( mPlayerList.at( i ) != currentPlayer ){
					mPlayerList.at( i )->sendOpponentInAction();
				}
			}
		}
		
		//Show opponent's arrow and send commands end
		currentPlayer->sendCommandsEnd();
		
	}else{ // mCentralCards->isFull()
		nextPlayer->sendOpponentSelectedCardId( cardPosition );
		nextPlayer->sendOpponentAddNewCentralCard( selectedCard );
		QTimer::singleShot( 1000, this, SLOT( slotCheckCentralCards() ) );
	}
	
	nextPlayer->sendCommandsEnd();
}*/

void Server::slotPlayerSelectedCard( Card* selectedCard, int cardPosition )
{
	//kDebug() << "Selected card:" << selectedCard.getCardText( mTypeOfCards );
	kDebug() << "Selected card:" << selectedCard->getCardText();
	
	//mCentralCards.append( selectedCard );
	mCentralCards->add( selectedCard );
	
	Player* nextPlayer = mGameSequence->getNextPlayer();
	
	//if( mCentralCards.size() != MAX_CENTRAL_CARDS_SIZE ){
	if( !mCentralCards->isFull() ){
		kDebug() << "Next player step.";
		
		Player* currentPlayer = mGameSequence->getCurrentPlayer();
		//Player* nextPlayer = mGameSequence->getNextPlayer();
		
		//If the previous player clicked to twenty/forty button, then show that card to current player
		if( mTwentyButtonClickedThisTurn ){
			kDebug() << "Pair this card position:" << currentPlayer->getPositionOfPairOfCard( selectedCard );
			int posOfPairOfCard = currentPlayer->getPositionOfPairOfCard( selectedCard );
			//nextPlayer->sendVisibleOpponentCards( cardPosition, selectedCard, posOfPairOfCard, currentPlayer->getCard( posOfPairOfCard ) );
			nextPlayer->sendVisibleOpponentCards( cardPosition, selectedCard, posOfPairOfCard, currentPlayer->getCard( posOfPairOfCard ) );
		
			
			//
			if( currentPlayer->getTricks() > 0 ){
				
				currentPlayer->addTricks( 20 );
				
				if( mGameSequence->isRoundOver() ){
					roundOver();
					return;
				}
				
			}else{ //currentPlayer->getTricks() == 0
				mWaitingMarriage = new QPair< Player*, int >;
				mWaitingMarriage->first = currentPlayer;
				mWaitingMarriage->second = 20;
			}
			
			mTwentyButtonClickedThisTurn = false;
		}
		
		if( mFortyButtonClickedThisTurn ){
			kDebug() << "Pair this card position:" << currentPlayer->getPositionOfPairOfCard( selectedCard );
			int posOfPairOfCard = currentPlayer->getPositionOfPairOfCard( selectedCard );
			//nextPlayer->sendVisibleOpponentCards( cardPosition, selectedCard, posOfPairOfCard, currentPlayer->getCard( posOfPairOfCard ) );
			nextPlayer->sendVisibleOpponentCards( cardPosition, selectedCard, posOfPairOfCard, currentPlayer->getCard( posOfPairOfCard ) );
			
			
			//
			if( currentPlayer->getTricks() > 0 ){
				
				currentPlayer->addTricks( 40 );
				
				if( mGameSequence->isRoundOver() ){
					roundOver();
					return;
				}
				
			}else{ //currentPlayer->getTricks() == 0
				mWaitingMarriage = new QPair< Player*, int >;
				mWaitingMarriage->first = currentPlayer;
				mWaitingMarriage->second = 40;
			}
			
			mFortyButtonClickedThisTurn = false;
		}
		
		if( mClickedToCloseButtonThisTurn ){
			nextPlayer->sendOpponentClickedToCloseButton();
		}
		
		nextPlayer->sendOpponentSelectedCardId( cardPosition );
		nextPlayer->sendOpponentAddNewCentralCard( selectedCard );
		
		mGameSequence->setCurrentPlayer( nextPlayer );
		//
		currentPlayer = mGameSequence->getCurrentPlayer();
		//
		
		//if( !mClickedToCloseButtonThisRound && mDeck->getDeckSize() > 0 ){
		if( !mPlayerWhoClickedToCloseButtonThisRound && mDeck->getDeckSize() > 0 ){
			//nextPlayer->sendSelectableAllCards();
			currentPlayer->sendSelectableAllCards();
			for( int i = 0; i < mPlayerList.size(); ++i ){
				if( mPlayerList.at( i ) != currentPlayer ){
					mPlayerList.at( i )->sendOpponentInAction();
				}
			}
		}else{ // mDeck->getDeckSize() == 0
			//nextPlayer->sendSelectableCertainCards();
			//nextPlayer->sendSelectableCertainCards( mCentralCards, mTrumpCard );
			currentPlayer->sendSelectableCertainCards( mCentralCards, mTrumpCard );
			for( int i = 0; i < mPlayerList.size(); ++i ){
				if( mPlayerList.at( i ) != currentPlayer ){
					mPlayerList.at( i )->sendOpponentInAction();
				}
			}
		}
		
		//Show opponent's arrow and send commands end
		//currentPlayer->sendCommandsEnd();
		for( int i = 0; i < mPlayerList.size(); ++i ){
			mPlayerList.at( i )->sendCommandsEnd();
		}
		
	}else{ // mCentralCards->isFull()
		nextPlayer->sendOpponentSelectedCardId( cardPosition );
		nextPlayer->sendOpponentAddNewCentralCard( selectedCard );
		QTimer::singleShot( 1000, this, SLOT( slotCheckCentralCards() ) );
	}
	
	nextPlayer->sendCommandsEnd();
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

void Server::slotPlayerClickedToCloseButton()
{
	kDebug() << mGameSequence->getCurrentPlayer()->getName() << "Clicked to close button.";
	mClickedToCloseButtonThisTurn = true;
	
	//
	for( int i = 0; i < mPlayerList.size(); ++i ){
		if( mPlayerList.at( i ) != mGameSequence->getCurrentPlayer() ){
			
			if( mPlayerList.at( i )->getTricks() == 0 ){
				mOpponentHaveNotTricksBeforePlayerClickedToCloseButton = true;
			}else{
				mOpponentHaveNotTricksBeforePlayerClickedToCloseButton = false;
			}
			
		}
	}
	//
	
	mPlayerWhoClickedToCloseButtonThisRound = mGameSequence->getCurrentPlayer();
}

/*void Server::slotPlayerChangedTrumpCard( Card newTrumpCard )
{
	mTrumpCard = newTrumpCard;
	
	for( int i = 0; i < mPlayerList.size(); ++i ){
		
		if( mPlayerList.at(i) != mGameSequence->getCurrentPlayer() ){
			mPlayerList.at(i)->sendNewTrumpCard( mTrumpCard );
		}
		
	}
}*/

void Server::slotPlayerChangeTrumpCard( Player *player )
{
	kDebug() << "Begin.";
	
	//Check
	if( mGameSequence->getCurrentPlayer() != player ){
		kDebug() << "ERROR! Not the current player want change trump card!";
		return;
	}
	
	if( !player->canChangeTrumpCard( mTrumpCard ) ){
		kDebug() << "ERROR! The current player can't change trump card, but he/she want!";
		return;
	}
	
	/*int pos = */player->changeTrumpCard( mTrumpCard );
	
	mTrumpCard->getCard()->setSelectable( false );
	
	if( player->haveRegularMarriages( mTrumpCard ) ){
		player->setTwentyButtonVisible( true );
	}
	
	if( player->haveTrumpMarriages( mTrumpCard ) ){
		player->setFortyButtonVisible( true );
	}
	
	for( int i = 0; i < mPlayerList.size(); ++i ){
		
		if( mPlayerList.at(i) != player ){
			mPlayerList.at(i)->sendNewTrumpCard( mTrumpCard );
		}
		
	}
	
	kDebug() << "End.";
}

void Server::slotCheckCentralCards()
{
	
	kDebug() << "Check the central cards.";
		
	//int centralCard1Point = mCentralCards.at(0).getCardPoint();
	//int centralCard1Point = mCentralCards->getCard( 0 ).getCardPoint();
	const int centralCard1Point = mCentralCards->getCard( 0 )->getCardPoint();
	//int centralCard2Point = mCentralCards.at(1).getCardPoint();
	//int centralCard2Point = mCentralCards->getCard(1).getCardPoint();
	const int centralCard2Point = mCentralCards->getCard(1)->getCardPoint();
	
	kDebug() << "centralCard1Point" << centralCard1Point;
	kDebug() << "centralCard2Point" << centralCard2Point;
	
	//
	//Card::CardSuit centralCard0Suit = mCentralCards->getCard( 0 ).getCardSuit();
	const Card::CardSuit centralCard0Suit = mCentralCards->getCard( 0 )->getCardSuit();
	//Card::CardSuit centralCard1Suit = mCentralCards->getCard( 1 ).getCardSuit();
	const Card::CardSuit centralCard1Suit = mCentralCards->getCard( 1 )->getCardSuit();
	//
	
	bool currentPlayerStartNextTurn;
	
	/*if( mTrumpCard.getCardSuit() == mCentralCards.at(0).getCardSuit() && mTrumpCard.getCardSuit() != mCentralCards.at(1).getCardSuit() ){
		currentPlayerStartNextTurn = false;
	}else if( mTrumpCard.getCardSuit() != mCentralCards.at(0).getCardSuit() && mTrumpCard.getCardSuit() == mCentralCards.at(1).getCardSuit() ){
		currentPlayerStartNextTurn = true;
	}else if( mCentralCards.at(0).getCardSuit() == mCentralCards.at(1).getCardSuit() ){
	*/	
	/*if( mTrumpCard.getCardSuit() == centralCard0Suit && mTrumpCard.getCardSuit() != centralCard1Suit ){
		currentPlayerStartNextTurn = false;
	}else if( mTrumpCard.getCardSuit() != centralCard0Suit && mTrumpCard.getCardSuit() == centralCard1Suit ){
		currentPlayerStartNextTurn = true;
	}else if( centralCard0Suit == centralCard1Suit ){
	*/
	if( mTrumpCard->getCard()->getCardSuit() == centralCard0Suit && mTrumpCard->getCard()->getCardSuit() != centralCard1Suit ){
		currentPlayerStartNextTurn = false;
	}else if( mTrumpCard->getCard()->getCardSuit() != centralCard0Suit && mTrumpCard->getCard()->getCardSuit() == centralCard1Suit ){
		currentPlayerStartNextTurn = true;
	}else if( centralCard0Suit == centralCard1Suit ){
	
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
	
	Player* currentPlayer = mGameSequence->getCurrentPlayer();
		
	kDebug() << currentPlayer->getName() << "get" << centralCard1Point+centralCard2Point << "points.";
	currentPlayer->addTricks( centralCard1Point + centralCard2Point );
	
	if( mWaitingMarriage && mWaitingMarriage->first == currentPlayer ){
		currentPlayer->addTricks( mWaitingMarriage->second );
		delete mWaitingMarriage;
		mWaitingMarriage = 0;
	}
		
	if( mClickedToCloseButtonThisTurn ){
		mClickedToCloseButtonThisTurn = false;
	}
		
	if( mGameSequence->isRoundOver() ){
		roundOver();
		return;
	}else{
		kDebug() << "Start next turn";
		
		//Clear central cards
		//mCentralCards.clear();
		mCentralCards->clear();
		
		for( int i = 0; i < mPlayerList.size(); ++i ){
			mPlayerList.at( i )->sendClearCentralCards();
		}
		
		//If player closed the deck
		if( !mPlayerWhoClickedToCloseButtonThisRound ){
			//If the dech have card yet, then add new cards to players, first who won the last turn
			if( mDeck->getDeckSize() > 0 ){
				//currentPlayer->sendNewCard( mDeck->getCard() );
				int cardId = currentPlayer->sendNewCard( mDeck->getCard() );
				
				for( int i = 0; i < mPlayerList.size(); ++i ){
					if( mPlayerList.at( i ) != currentPlayer ){
						mPlayerList.at( i )->sendNewOpponentCard( cardId );
					}
				}
			
				if( mDeck->getDeckSize() > 0 ){
					//mGameSequence->getNextPlayer()->sendNewCard( mDeck->getCard() );
					cardId = mGameSequence->getNextPlayer()->sendNewCard( mDeck->getCard() );
					
					for( int i = 0; i < mPlayerList.size(); ++i ){
						if( mPlayerList.at( i ) != mGameSequence->getNextPlayer() ){
							mPlayerList.at( i )->sendNewOpponentCard( cardId );
						}
					}
					
				}else{
					//mGameSequence->getNextPlayer()->sendNewCard( mTrumpCard->getCard() );
					cardId = mGameSequence->getNextPlayer()->sendNewCard( mTrumpCard->getCard() );
					
					for( int i = 0; i < mPlayerList.size(); ++i ){
						if( mPlayerList.at( i ) != mGameSequence->getNextPlayer() ){
							mPlayerList.at( i )->sendNewOpponentCard( cardId );
						}
					}
					
					mTrumpCard->clearTrumpCard();
					
					for( int i = 0; i < mPlayerList.size(); ++i ){
						mPlayerList.at( i )->sendClearTrumpCard();
					}
				
				}
			
			}
		}
		
		currentPlayer->sendSelectableAllCards();
		for( int i = 0; i < mPlayerList.size(); ++i ){
			if( mPlayerList.at( i ) != currentPlayer ){
				mPlayerList.at( i )->sendOpponentInAction();
			}
		}
		
		//if( currentPlayer->haveRegularMarriages() ){
		if( currentPlayer->haveRegularMarriages( mTrumpCard ) ){
			kDebug() << currentPlayer->getName() << "have regular marriages.";
			currentPlayer->setTwentyButtonVisible( true );
			currentPlayer->sendTwentyButtonVisible();
		}
	
		//if( currentPlayer->haveTrumpMarriages() ){
		if( currentPlayer->haveTrumpMarriages( mTrumpCard ) ){
			kDebug() << currentPlayer->getName() << "have trump marriages.";
			currentPlayer->setFortyButtonVisible( true );
			currentPlayer->sendFortyButtonVisible();
		}
		
		//if( currentPlayer->canChangeTrumpCard() ){
		if( currentPlayer->canChangeTrumpCard( mTrumpCard ) ){
			kDebug() << currentPlayer->getName() << "can change trump card.";
			currentPlayer->sendSelectableTrumpCard();
		}
		
		//if( !mClickedToCloseButtonThisRound && mDeck->getDeckSize() > 3 ){
		if( !mPlayerWhoClickedToCloseButtonThisRound && mDeck->getDeckSize() > 3 ){
			currentPlayer->sendCloseButtonVisible();
		}
		
	}
	
	for( int i = 0; i < mPlayerList.size(); ++i ){
		mPlayerList.at( i )->sendCommandsEnd();
	}
	
}

void Server::slotPlayerWantStartNextRound( Player *player )
{
	bool playerOk = true;
	
	for( int i = 0; i < mPlayerListWhoWantResumeGame.size(); ++i ){
		
		if( player->getName() == mPlayerListWhoWantResumeGame.at( i )->getName() ){
			playerOk = false;
		}
		
	}
	
	if( playerOk ){
		mPlayerListWhoWantResumeGame.append( player );
	}else{
		kDebug() << "ERROR!" << player->getName() << "more then once want start next round!";
	}
	
	if( mPlayerListWhoWantResumeGame.size() == mPlayerList.size() ){
		mPlayerListWhoWantResumeGame.clear();
		kDebug() << "Start next round.";
		
		//
		mGameSequence->nextPlayerStartRound();
		//
		
		newRound();
		
		for( int i = 0; i < mPlayerList.size(); ++i ){
			mPlayerList.at( i )->sendCommandsEnd();
		}
	}
}

void Server::slotPlayerWantStartNextGame( Player *player )
{
	bool playerOk = true;
	
	for( int i = 0; i < mPlayerListWhoWantNewGame.size(); ++i ){
		
		if( player->getName() == mPlayerListWhoWantNewGame.at( i )->getName() ){
			playerOk = false;
		}
		
	}
	
	if( playerOk ){
		mPlayerListWhoWantNewGame.append( player );
	}else{
		kDebug() << "ERROR!" << player->getName() << "more then once want start new game!";
	}
	
	if( mPlayerListWhoWantNewGame.size() == mPlayerList.size() ){
		
		mPlayerListWhoWantNewGame.clear();
		kDebug() << "Start new game.";
		
		//
		mGameSequence->nextPlayerStartGame();
		//
		
		newGame();
		
		for( int i = 0; i < mPlayerList.size(); ++i ){
			mPlayerList.at( i )->sendCommandsEnd();
		}
		
	}
	
}


void Server::incomingConnection( int socketDescriptor )
{
	kDebug() << "socketDescriptor:" << socketDescriptor;
	
	Player* player = new Player( this );
	player->setSocketDescriptor( socketDescriptor );
	
	//If the player have name, then will be a player
	connect( player, SIGNAL( signalNewPlayer( Player* ) ), this, SLOT( slotNewPlayer( Player* ) ) );
	//
	connect( player, SIGNAL( signalPlayerDisconnected( Player* ) ), this, SLOT( slotPlayerDisconnected( Player* ) ) );
}

void Server::setWhoStartGame( Knapsen::WhoStartGame whoStartGame )
{
	mGameSequence->setWhoStartGame( whoStartGame );
}

void Server::addBot( QString botName )
{
	mBot = new Bot( this );
	mBot->setName( botName );
	
	mBot->connectToHost( "127.0.0.1", serverPort() );
}

void Server::startGame()
{
	kDebug() << "Start game.";
	
	//Initialize deck
	mDeck = new Deck( mSizeOfDeck );

	mTrumpCard = new TrumpCard;

	//To all players
	for( int i = 0; i < mPlayerList.size(); ++i ){
		
		//Send the opponent player's name
		for( int j = 0; j < mPlayerList.size(); ++j ){
			if( mPlayerList.at( j ) != mPlayerList.at( i ) ){
				mPlayerList.at( j )->sendOpponentName( mPlayerList.at( i )->getName() );
			}
		}
		
		//Send the type of cards
		mPlayerList.at( i )->sendTypeOfCard( mTypeOfCards );
		//Send the size of deck
		mPlayerList.at( i )->sendSizeOfDeck( mSizeOfDeck );
		//Send number of cards in hand
		mPlayerList.at( i )->sendNumberOfCardsInHand( mNumberOfCardsInHand );
		
		//Initialize table
		mPlayerList.at( i )->sendInitializeTable();
	}
	
	newGame();
	
	for( int i = 0; i < mPlayerList.size(); ++i ){
		mPlayerList.at( i )->sendCommandsEnd();
	}

}
