#include <QtCore/QTimer>
#include <KDE/KDebug>
//#include "gamesequence.h"
#include "deck.h"
#include "centralcards.h"
#include "trump.h"
#include "bot.h"
#include "player.h"
#include "server.h"

//Only 2 players game, 4 players maybe in the future...
const int MAX_PLAYERS = 2;

Server::Server( QObject* parent ) :
    QTcpServer( parent ),
    mSizeOfDeck( 20 ),
    mNumberOfCardsInHand( 5 ),
    mTypeOfCards( Knapsen::GermanSuits ),
    mCurrentPlayerId(0),
    mGameStarterPlayerId(0),
    mRoundStarterPlayerId(0),
    mTwentyButtonClickedThisTurn( false ),
    mFortyButtonClickedThisTurn( false ),
    mClickedToCloseButtonThisTurn( false ),
    mOpponentHaveNotTricksBeforePlayerClickedToCloseButton( false )
{
    kDebug() << "Initialize.";

    mBot = 0;
    mCentralCards = 0;
    mTrump = 0;
    mDeck = 0;
    mPlayerWhoClickedToCloseButtonThisRound = 0;
    mWaitingMarriage = 0;
    mPlayerListWhoWantNewRound = 0;
    mPlayerListWhoWantNewGame = 0;
}

Server::~Server()
{
	kDebug() << "Server is deleting.";
	if( mBot ){
		delete mBot;
	}
	/*if( mGameSequence ){
		delete mGameSequence;
	}*/
	if( mDeck ){
		delete mDeck;
	}
	if( mCentralCards ){
		delete mCentralCards;
	}
	if( mTrump ){
		delete mTrump;
	}
}

void Server::newGame()
{
	kDebug() << "Start new game.";
	
	for( int i = 0; i < mPlayerList.size(); ++i ){
		mPlayerList.at( i )->sendNewGame();
	}
	
	newRound();
	
}

void Server::newRound()
{
	kDebug() << "Start new round.";
	
	//Build deck
	mDeck->buildDeck();
	
	//Clear central cards
	mCentralCards->clear();
	
	//Clear trump card
	mTrump->clearTrumpCard( true );
	
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
			addNewCard( mPlayerList.at( j ), mDeck->getCard() );
		}
		
		//Get trump card
		if( i == 2 ){
			//mTrumpCard = mDeck->getCard();
			mTrump->addNewCard( mDeck->getCard() );
			
			for( int j = 0; j < mPlayerList.size(); ++j ){
				mPlayerList.at( j )->sendNewTrumpCard( mTrump );
			}
			
		}
		
	}
	
	//Player* currentPlayer = mGameSequence->getCurrentPlayer();
	Player* currentPlayer = getCurrentPlayer();
	
	//if( currentPlayer->haveRegularMarriages() ){
	if( currentPlayer->haveRegularMarriages( mTrump ) ){
		kDebug() << currentPlayer->getName() << "have regular marriages.";
		currentPlayer->setTwentyButtonVisible( true );
		currentPlayer->sendTwentyButtonVisible();
	}
	
	//if( currentPlayer->haveTrumpMarriages() ){
	if( currentPlayer->haveTrumpMarriages( mTrump ) ){
		kDebug() << currentPlayer->getName() << "have trump marriages.";
		currentPlayer->setFortyButtonVisible( true );
		currentPlayer->sendFortyButtonVisible();
	}
	
	//if( currentPlayer->canChangeTrumpCard() ){
	if( currentPlayer->canChangeTrumpCard( mTrump ) ){
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
    
    //Player *currentPlayer = mGameSequence->getCurrentPlayer();
    Player* currentPlayer = getCurrentPlayer();
    //Player *nextPlayer = mGameSequence->getNextPlayer();
    Player* nextPlayer = getNextPlayer();
    
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
    addScores( winnerPlayer, scores );
    
    //if( mGameSequence->isGameOver() ){
    if( isGameOver() ){
        mPlayerListWhoWantNewGame = new QList< Player* >;
        
        for( int i = 0; i < mPlayerList.size(); ++i ){
            mPlayerList.at( i )->sendEndGame( winnerPlayer->getName() );
        }

    }else{
        mPlayerListWhoWantNewRound = new QList< Player* >;
            
        for( int i = 0; i < mPlayerList.size(); ++i ){
            mPlayerList.at( i )->sendEndRound( winnerPlayer->getName(), scores );
        }
    }

    /*for( int i = 0; i < mPlayerList.size(); ++i ){
        mPlayerList.at( i )->sendCommandsEnd();
    }*/
}

void Server::nextPlayerStartRound()
{
    if( mRoundStarterPlayerId == 0 ){
        mRoundStarterPlayerId = 1;
    }else{
        mRoundStarterPlayerId = 0;
    }
        
    mCurrentPlayerId = mRoundStarterPlayerId;
}

void Server::nextPlayerStartGame()
{
    if( mGameStarterPlayerId == 0 ){
        mGameStarterPlayerId = 1;
    }else{
        mGameStarterPlayerId = 0;
    }
    
    mRoundStarterPlayerId = mGameStarterPlayerId;
    mCurrentPlayerId = mRoundStarterPlayerId;
}

void Server::setCurrentPlayer( Player* player )
{
    for( int i = 0; i < mPlayerList.size(); ++i ){
        if( player == mPlayerList.at( i ) ){
            mCurrentPlayerId = i;
        }
    }
}

Player* Server::getNextPlayer()
{
    if( mCurrentPlayerId == 0 ){
        return mPlayerList.at( 1 );
    }//else
    
    return mPlayerList.at( 0 );
}

bool Server::isGameOver()
{
    for( int i = 0; i < mPlayerList.size(); ++i ){
        if( mPlayerList.at( i )->getScores() >= 7 ){ // mPlayers->at( i )->getScores() >= 7
            return true;
        }
    }
        
    return false;
}

bool Server::isRoundOver()
{
    //If a player have equal or more then 66 tricks, will have win the round
    for( int i = 0; i < mPlayerList.size(); ++i ){
        if( mPlayerList.at( i )->getTricks() >= 66 ){
            return true;
        }
    }
        
    //If a player have not card in hand, then win the round who win the last turn
    if( mPlayerList.at( mCurrentPlayerId )->getNumberOfCardsInHandNow()  == 0 ){
        return true;
    }
        
    return false;
}

void Server::addNewCard( Player* player, Card* card )
{
	int cardId = player->sendNewCard( card );
	
	//Send player's card id to other players
	for( int i = 0; i < mPlayerList.size(); ++i ){
		if( mPlayerList.at( i ) != player ){
			mPlayerList.at( i )->sendNewOpponentCard( cardId );
		}
	}
}

void Server::addTricks( Player* player, int newTricks )
{
	player->addTricks( newTricks );
	
	//Send player's tricks to other players
	for( int i = 0; i < mPlayerList.size(); ++i ){
		if( mPlayerList.at( i ) != player ){
			mPlayerList.at( i )->sendOpponentTricksChanged( player->getTricks() );
		}
	}
}

void Server::addScores( Player* player, int newScores )
{
	player->addScores( newScores );
	
	//Send player's scores to other players
	for( int i = 0; i < mPlayerList.size(); ++i ){
		if( mPlayerList.at( i ) != player ){
			mPlayerList.at( i )->sendOpponentScoresChanged( player->getScores() );
		}
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

void Server::slotPlayerSelectedCard( Card* selectedCard, int cardPosition )
{
    kDebug() << "Selected card:" << selectedCard->getCardText();
    
    mCentralCards->add( selectedCard );

    //Player* nextPlayer = getNextPlayer();
        
    if( !mCentralCards->isFull() ){
        kDebug() << "Next player step.";
        
        //Player* currentPlayer = getCurrentPlayer();

        //If the previous player clicked to twenty/forty button, then show that card to current player
        if( mTwentyButtonClickedThisTurn ){
            
            //int posOfPairOfCard = currentPlayer->getPositionOfPairOfCard( selectedCard );
            int posOfPairOfCard = getCurrentPlayer()->getPositionOfPairOfCard( selectedCard );
            
            //kDebug() << "Pair this card position:" << currentPlayer->getPositionOfPairOfCard( selectedCard );
            kDebug() << "Pair this card position:" << posOfPairOfCard;
            
            //nextPlayer->sendVisibleOpponentCards( cardPosition, selectedCard, posOfPairOfCard, currentPlayer->getCard( posOfPairOfCard ) );
            getNextPlayer()->sendVisibleOpponentCards( cardPosition, selectedCard, posOfPairOfCard, getCurrentPlayer()->getCard( posOfPairOfCard ) );

            //
            //if( currentPlayer->getTricks() > 0 ){
            if( getCurrentPlayer()->getTricks() > 0 ){
                //addTricks( currentPlayer, 20 );
                addTricks( getCurrentPlayer(), 20 );
                
                //if( mGameSequence->isRoundOver() ){
                if( isRoundOver() ){
                    roundOver();
                    
                    for( int i = 0; i < mPlayerList.size(); ++i ){
                        mPlayerList.at( i )->sendCommandsEnd();
                    }
                
                    return;
                }
            
            }else{ //getCurrentPlayer()->getTricks() > 0
                mWaitingMarriage = new QPair< Player*, int >;
                //mWaitingMarriage->first = currentPlayer;
                mWaitingMarriage->first = getCurrentPlayer();
                mWaitingMarriage->second = 20;
            }
            
            mTwentyButtonClickedThisTurn = false;
        }
        
        if( mFortyButtonClickedThisTurn ){
            int posOfPairOfCard = getCurrentPlayer()->getPositionOfPairOfCard( selectedCard );
            kDebug() << "Pair this card position:" << posOfPairOfCard;
            getNextPlayer()->sendVisibleOpponentCards( cardPosition, selectedCard, posOfPairOfCard, getCurrentPlayer()->getCard( posOfPairOfCard ) );

            //
            if( getCurrentPlayer()->getTricks() > 0 ){
                addTricks( getCurrentPlayer(), 40 );

                //if( mGameSequence->isRoundOver() ){
                if( isRoundOver() ){
                    roundOver();
                    
                    for( int i = 0; i < mPlayerList.size(); ++i ){
                        mPlayerList.at( i )->sendCommandsEnd();
                    }
                    
                    return;
                }
                
            }else{ //currentPlayer->getTricks() >= 0
                mWaitingMarriage = new QPair< Player*, int >;
                mWaitingMarriage->first = getCurrentPlayer();
                mWaitingMarriage->second = 40;
            }
            
            mFortyButtonClickedThisTurn = false;
        }
        
        if( mClickedToCloseButtonThisTurn ){
            getNextPlayer()->sendOpponentClickedToCloseButton();
        }
        
        getNextPlayer()->sendOpponentSelectedCardId( cardPosition );
        getNextPlayer()->sendOpponentAddNewCentralCard( selectedCard );
        
        //mGameSequence->setCurrentPlayer( nextPlayer );
        setCurrentPlayer( getNextPlayer() );
        //
        //currentPlayer = mGameSequence->getCurrentPlayer();
        //
        
        //if( !mClickedToCloseButtonThisRound && mDeck->getDeckSize() > 0 ){
        if( !mPlayerWhoClickedToCloseButtonThisRound && mDeck->getDeckSize() > 0 ){
            //currentPlayer->sendSelectableAllCards();
            getCurrentPlayer()->sendSelectableAllCards();
            /*for( int i = 0; i < mPlayerList.size(); ++i ){
                if( mPlayerList.at( i ) != getCurrentPlayer() ){
                    mPlayerList.at( i )->sendOpponentInAction();
                }
            }*/
            getNextPlayer()->sendOpponentInAction();
        }else{ // mDeck->getDeckSize() == 0
            //currentPlayer->sendSelectableCertainCards( mCentralCards, mTrump );
            getCurrentPlayer()->sendSelectableCertainCards( mCentralCards, mTrump );
            /*for( int i = 0; i < mPlayerList.size(); ++i ){
                if( mPlayerList.at( i ) != getCurrentPlayer() ){
                    mPlayerList.at( i )->sendOpponentInAction();
                }
            }*/
            getNextPlayer()->sendOpponentInAction();
        }
        
        /*
        //Show opponent's arrow and send commands end
        for( int i = 0; i < mPlayerList.size(); ++i ){
            mPlayerList.at( i )->sendCommandsEnd();
        }
        */
        
    }else{ // !mCentralCards->isFull()
        //nextPlayer->sendOpponentSelectedCardId( cardPosition );
        getNextPlayer()->sendOpponentSelectedCardId( cardPosition );
        //nextPlayer->sendOpponentAddNewCentralCard( selectedCard );
        getNextPlayer()->sendOpponentAddNewCentralCard( selectedCard );
        QTimer::singleShot( 1000, this, SLOT( slotCheckCentralCards() ) );
    }
    
    //nextPlayer->sendCommandsEnd();

    //Show opponent's arrow and send commands end
    for( int i = 0; i < mPlayerList.size(); ++i ){
        mPlayerList.at( i )->sendCommandsEnd();
    }
}

void Server::slotPlayerTwentyButtonClicked()
{
    kDebug() << getCurrentPlayer()->getName() << "Clicked to twenty button.";
    getCurrentPlayer()->setSelectableRegularMarriagesCards( mTrump );
    
    mTwentyButtonClickedThisTurn = true;
}

void Server::slotPlayerFortyButtonClicked()
{
    kDebug() << getCurrentPlayer()->getName() << "Clicked to forty button.";
    getCurrentPlayer()->setSelectableTrumpMarriagesCards( mTrump );
    
    mFortyButtonClickedThisTurn = true;
}

void Server::slotPlayerClickedToCloseButton()
{
    kDebug() << getCurrentPlayer()->getName() << "Clicked to close button.";
    mClickedToCloseButtonThisTurn = true;
    
    //
    for( int i = 0; i < mPlayerList.size(); ++i ){
        if( mPlayerList.at( i ) != getCurrentPlayer() ){
            
            if( mPlayerList.at( i )->getTricks() == 0 ){
                mOpponentHaveNotTricksBeforePlayerClickedToCloseButton = true;
            }else{
                mOpponentHaveNotTricksBeforePlayerClickedToCloseButton = false;
            }
            
        }
    }
    //
    
    mPlayerWhoClickedToCloseButtonThisRound = getCurrentPlayer();
}

void Server::slotPlayerChangeTrumpCard( Player *player )
{
    kDebug() << "Begin.";
    
    //Check
    if( getCurrentPlayer() != player ){
        kDebug() << "ERROR! Not the current player want change trump card!";
        return;
    }
    
    if( !player->canChangeTrumpCard( mTrump ) ){
        kDebug() << "ERROR! The current player can't change trump card, but he/she want!";
        return;
    }
    
    int id = player->changeTrumpCard( mTrump );
    
    mTrump->getCard()->setSelectable( false );
    player->getCard( id )->setSelectable( true );
        
    if( player->haveRegularMarriages( mTrump ) ){
        player->setTwentyButtonVisible( true );
    }
    
    if( player->haveTrumpMarriages( mTrump ) ){
        player->setFortyButtonVisible( true );
    }
    
    for( int i = 0; i < mPlayerList.size(); ++i ){
        
        if( mPlayerList.at(i) != player ){
            mPlayerList.at(i)->sendNewTrumpCard( mTrump );
        }
        
    }
    
    kDebug() << "End.";
}

void Server::slotCheckCentralCards()
{
    kDebug() << "Check the central cards.";
    
    const int centralCard1Point = mCentralCards->getCard( 0 )->getCardPoint();
    const int centralCard2Point = mCentralCards->getCard( 1 )->getCardPoint();
    
    kDebug() << "centralCard1Point" << centralCard1Point;
    kDebug() << "centralCard2Point" << centralCard2Point;
    
    const Card::Suit centralCard0Suit = mCentralCards->getCard( 0 )->getSuit();
    const Card::Suit centralCard1Suit = mCentralCards->getCard( 1 )->getSuit();
    
    bool currentPlayerStartNextTurn;
    
    if( mTrump->getCardSuit() == centralCard0Suit && mTrump->getCardSuit() != centralCard1Suit ){
        currentPlayerStartNextTurn = false;
    }else if( mTrump->getCardSuit() != centralCard0Suit && mTrump->getCardSuit() == centralCard1Suit ){
        currentPlayerStartNextTurn = true;
    }else if( centralCard0Suit == centralCard1Suit ){
    
        if( centralCard1Point < centralCard2Point ){
            currentPlayerStartNextTurn = true;
        }else{
            currentPlayerStartNextTurn = false;
        }
        
    }else{ // centralCard0Suit == centralCard1Suit
        currentPlayerStartNextTurn = false;
    }
    
    if( !currentPlayerStartNextTurn ){
        setCurrentPlayer( getNextPlayer() );
    }
    
    //Player* currentPlayer = mGameSequence->getCurrentPlayer();
    
    kDebug() << getCurrentPlayer()->getName() << "get" << centralCard1Point+centralCard2Point << "points.";
    addTricks( getCurrentPlayer(), centralCard1Point + centralCard2Point );
    
    if( mWaitingMarriage && mWaitingMarriage->first == getCurrentPlayer() ){
        addTricks( getCurrentPlayer(), mWaitingMarriage->second );
        delete mWaitingMarriage;
        mWaitingMarriage = 0;
    }
    
    if( mClickedToCloseButtonThisTurn ){
        mClickedToCloseButtonThisTurn = false;
    }
    
    if( isRoundOver() ){
        roundOver();
        //return;
    }else{
        kDebug() << "Start next turn";
        
        //Clear central cards
        mCentralCards->clear();
        
        for( int i = 0; i < mPlayerList.size(); ++i ){
            mPlayerList.at( i )->sendClearCentralCards();
        }
        
        //If player closed the deck
        if( !mPlayerWhoClickedToCloseButtonThisRound ){
            //If the dech have card yet, then add new cards to players, first who won the last turn
            if( mDeck->getDeckSize() > 0 ){
                addNewCard( getCurrentPlayer(), mDeck->getCard() );
                
                if( mDeck->getDeckSize() > 0 ){
                    addNewCard( getNextPlayer(), mDeck->getCard() );
                    
                }else{
                    addNewCard( getNextPlayer(), mTrump->getCard() );
                    
                    mTrump->clearTrumpCard( false );
                    
                    for( int i = 0; i < mPlayerList.size(); ++i ){
                        mPlayerList.at( i )->sendClearTrumpCard();
                    }
                    
                }
                
            }
        }
        
        getCurrentPlayer()->sendSelectableAllCards();
        for( int i = 0; i < mPlayerList.size(); ++i ){
            if( mPlayerList.at( i ) != getCurrentPlayer() ){
                mPlayerList.at( i )->sendOpponentInAction();
            }
        }
        
        if( getCurrentPlayer()->haveRegularMarriages( mTrump ) ){
            kDebug() << getCurrentPlayer()->getName() << "have regular marriages.";
            getCurrentPlayer()->setTwentyButtonVisible( true );
            getCurrentPlayer()->sendTwentyButtonVisible();
        }
        
        if( getCurrentPlayer()->haveTrumpMarriages( mTrump ) ){
            kDebug() << getCurrentPlayer()->getName() << "have trump marriages.";
            getCurrentPlayer()->setFortyButtonVisible( true );
            getCurrentPlayer()->sendFortyButtonVisible();
        }
        
        if( getCurrentPlayer()->canChangeTrumpCard( mTrump ) ){
            kDebug() << getCurrentPlayer()->getName() << "can change trump card.";
            getCurrentPlayer()->sendSelectableTrumpCard();
        }
        
        if( !mPlayerWhoClickedToCloseButtonThisRound && mDeck->getDeckSize() > 3 ){
            getCurrentPlayer()->sendCloseButtonVisible();
        }
        
    }
    
    for( int i = 0; i < mPlayerList.size(); ++i ){
        mPlayerList.at( i )->sendCommandsEnd();
    }
    
}

void Server::slotPlayerWantStartNextRound( Player *player )
{
    if( mPlayerListWhoWantNewRound != 0 ){
        
        for( int i = 0; i < mPlayerListWhoWantNewRound->size(); ++i ){
            if( player == mPlayerListWhoWantNewRound->at( i ) ){
                kDebug() << "ERROR!" << player->getName() << "want again start next round!";
                return;
            }
        }
        
        mPlayerListWhoWantNewRound->append( player );
        
        if( mPlayerListWhoWantNewRound->size() == mPlayerList.size() ){
            mPlayerListWhoWantNewRound->clear();
            delete mPlayerListWhoWantNewRound;
            mPlayerListWhoWantNewRound = 0;
            
            kDebug() << "Start next round.";
            
            nextPlayerStartRound();
            
            newRound();
            
            for( int i = 0; i < mPlayerList.size(); ++i ){
                mPlayerList.at( i )->sendCommandsEnd();
            }
        }
        
    }else{ //mPlayerListWhoWantNewRound != 0
        kDebug() << "ERROR!" << player->getName() << "want start next round, but this isn't possible!";
    }
}

void Server::slotPlayerWantStartNextGame( Player *player )
{
    if( mPlayerListWhoWantNewGame != 0 ){
        
        for( int i = 0; i < mPlayerListWhoWantNewGame->size(); ++i ){
            if( player == mPlayerListWhoWantNewGame->at( i ) ){
                kDebug() << "ERROR!" << player->getName() << "want again start new game!";
                return;
            }
        }
        
        mPlayerListWhoWantNewGame->append( player );
        
        if( mPlayerListWhoWantNewGame->size() == mPlayerList.size() ){
            mPlayerListWhoWantNewGame->clear();
            delete mPlayerListWhoWantNewGame;
            mPlayerListWhoWantNewGame = 0;
            
            kDebug() << "Start new game.";
            
            nextPlayerStartGame();
            
            newGame();
            
            for( int i = 0; i < mPlayerList.size(); ++i ){
                mPlayerList.at( i )->sendCommandsEnd();
            }
        }
        
    }else{ //mPlayerListWhoWantNewGame != 0
        kDebug() << "ERROR!" << player->getName() << "want start new game, but this isn't possible!";
    }
}


void Server::incomingConnection( int socketDescriptor )
{
	kDebug() << "socketDescriptor:" << socketDescriptor;
	
	Player* player = new Player("");
	player->setSocketDescriptor( socketDescriptor );
	
	//If the player have name, then will be a player
	connect( player, SIGNAL( signalNewPlayer( Player* ) ),          this, SLOT( slotNewPlayer( Player* ) ) );
	connect( player, SIGNAL( signalPlayerDisconnected( Player* ) ), this, SLOT( slotPlayerDisconnected( Player* ) ) );
}

void Server::setWhoStartGame( Knapsen::WhoStartGame whoStartGame )
{
    //mGameSequence->setWhoStartGame( whoStartGame );
    if( whoStartGame == Knapsen::RandomPlayer ){
        
        if( qrand()%2 == 0 ){
            whoStartGame = Knapsen::AdminPlayer;
        }else{
            whoStartGame = Knapsen::OpponentPlayer;
        }
        
    }
    
    if( whoStartGame == Knapsen::AdminPlayer ){
        //The first player always the admin
        mGameStarterPlayerId = 0;
    }else{ //whoStartGame == Knapsen::AdminPlayer
        //The second player always the opponent
        mGameStarterPlayerId = 1;
    }
    
    mRoundStarterPlayerId = mGameStarterPlayerId;
    mCurrentPlayerId = mRoundStarterPlayerId;
}

void Server::addBot( QString botName, Knapsen::GameDifficulty difficulty )
{
	mBot = new Bot( botName, difficulty );
	
	mBot->connectToHost( "127.0.0.1", serverPort() );
}

void Server::startGame()
{
	kDebug() << "Start game.";
	
	//Initialize deck
	mDeck = new Deck( mSizeOfDeck );
	
	//Initialize central card
	mCentralCards = new CentralCards;
	
	//Initialize trump
	mTrump = new Trump;
	
	for( int i = 0; i < mPlayerList.size(); ++i ){
		for( int j = 0; j < mPlayerList.size(); ++j ){
			if( mPlayerList.at( j ) != mPlayerList.at( i ) ){
				mPlayerList.at( i )->sendInitializeTable( mPlayerList.at( j )->getName(), mTypeOfCards, mSizeOfDeck, mNumberOfCardsInHand );
			}
		}
	}
	
	newGame();
	
	for( int i = 0; i < mPlayerList.size(); ++i ){
		mPlayerList.at( i )->sendCommandsEnd();
	}
}
