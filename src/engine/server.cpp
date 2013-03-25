#include <QtCore/QTimer>
#include <KDE/KDebug>
#include "trump.h"
#include "bot.h"
#include "player.h"
#include "server.h"

//Only 2 players game, 4 players maybe in the future...
const int MAX_PLAYERS = 2;

const int HALF_WIN_POINTS = 33; //33
const int WIN_TRICKS = 66; //66

const int WIN_SCORES = 7; //7

Server::Server(Knapsen::TypeOfCards typeOfCards, 
               int sizeOfDeck, 
               int numberOfCardsInHand, 
               Knapsen::WhoStartGame whoStartGame,
               bool schnapsenButton
              ) :
    mTypeOfCards( typeOfCards ),
    mSizeOfDeck( sizeOfDeck ),
    mNumberOfCardsInHand( numberOfCardsInHand ),
    mSchnapsenButton( schnapsenButton ),
    //mCurrentPlayerId( 0 ),
    //mGameStarterPlayerId( 0 ),
    //mRoundStarterPlayerId( 0 ),
    mTwentyButtonClickedThisTurn( false ),
    mFortyButtonClickedThisTurn( false ),
    mSchnapsenButtonClickedThisRound( false ),
    mClickedToCloseButtonThisTurn( false ),
    mOpponentHaveNotTricksBeforePlayerClickedToCloseButton( false )
{
    kDebug() << "Initialize.";
    
    mBot = 0;
    mTrump = new Trump();
    
    mPlayerWhoClickedToCloseButtonThisRound = 0;
    mWaitingMarriage = 0;
    mPlayerListWhoWantNewRound = 0;
    mPlayerListWhoWantNewGame = 0;
    
    setWhoStartGame( whoStartGame );
}

Server::~Server()
{
    kDebug() << "Server is deleting.";
    if( mBot ){
        delete mBot;
    }
    /*if( mDeck ){
        delete mDeck;
    }*/
    /*if( mCentralCards ){
        delete mCentralCards;
    }*/
    //if( mTrump ){
    delete mTrump;
    //}
}

void Server::buildDeck()
{
kDebug() << "Build deck.";
    
    while( mCardDeck.size() != mSizeOfDeck ){
        Card *card;
        
        if( mSizeOfDeck == Card::CARD_NUMBERS_WITHOUT_9 ){
            card = new Card( qrand()%Card::CARD_NUMBERS_WITHOUT_9 );
        }else{ //mDeckSize == Card::CARD_NUMBERS_WITH_9
            card = new Card( qrand()%Card::CARD_NUMBERS_WITH_9 );
        }
        
        bool inList = false;
        for( int i = 0; i < mCardDeck.size(); ++i ){
            if( mCardDeck.at(i)->getValue() == card->getValue() ){
                inList = true;
            }
        }
        
        if( !inList ){
            mCardDeck.append( card );
        }else{
            delete card;
        }
        
    }
    
    //Marriages
    //mCards[ 11 ] = new Card( 17 );
    //mCards[ 18 ] = new Card( 16 );
    //
    
    //Can change trump card
    //mCards[ 19 ] = new Card( 3 );
    //mCards[ 18 ] = new Card( 3 );
    //mCards[ 13 ] = new Card( 0 );
    
    kDebug() << "Generated deck:";
    for( int i = 0; i < mCardDeck.size(); ++i ){
        kDebug() << i << ":" << mCardDeck.at( i )->getValue();
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
    buildDeck();
    
    //Clear central cards
    clearCentralCards();
    
    //Clear trump card
    if( !mTrump->isEmpty() ){
        mTrump->clearTrumpCard();
    }
    
    //Clear variables
    mOpponentHaveNotTricksBeforePlayerClickedToCloseButton = false;
    mPlayerWhoClickedToCloseButtonThisRound = 0;
    mTwentyButtonClickedThisTurn = false;
    mFortyButtonClickedThisTurn = false;
    mSchnapsenButtonClickedThisRound = false;
    mClickedToCloseButtonThisTurn = false;
    
    //Clear cards, tricks
    for( int i = 0; i < mPlayerList.size(); ++i ){
        mPlayerList.at( i )->sendNewRound();
    }
    
    //Send cards to players
    for( int i = 0; i < mNumberOfCardsInHand; ++i ){
        
        for( int j = 0; j < mPlayerList.size(); ++j ){
            //addNewCard( mPlayerList.at( j ), mDeck->takeCard() );
            addNewCard( mPlayerList.at( j ), mCardDeck.takeLast() );
        }
        
        //Get trump card
        if( i == 2 ){
            //mTrumpCard = mDeck->getCard();
            //mTrump->addNewCard( mDeck->takeCard() );
            mTrump->addNewCard( mCardDeck.takeLast() );
            
            for( int j = 0; j < mPlayerList.size(); ++j ){
                mPlayerList.at( j )->sendNewTrumpCard( mTrump );
            }
            
        }
        
    }
    
    Player* currentPlayer = getCurrentPlayer();
    
    if( currentPlayer->haveRegularMarriage( mTrump ) ){
        kDebug() << currentPlayer->getName() << "have regular marriages.";
        currentPlayer->setTwentyButtonVisible( true );
        currentPlayer->sendTwentyButtonVisible();
    }
    
    if( currentPlayer->haveTrumpMarriage( mTrump ) ){
        kDebug() << currentPlayer->getName() << "have trump marriages.";
        currentPlayer->setFortyButtonVisible( true );
        currentPlayer->sendFortyButtonVisible();
    }
    
    if( currentPlayer->canChangeTrumpCard( mTrump ) ){
        kDebug() << currentPlayer->getName() << "can change trump card.";
        currentPlayer->sendSelectableTrumpCard();
    }
    
    currentPlayer->sendCloseButtonVisible();
    
    //Schnapsen button
    if( mSchnapsenButton ){
        getCurrentPlayer()->setSchnapsenButtonVisible( true );
        getCurrentPlayer()->sendSchnapsenButtonVisible();
    }
    
    currentPlayer->sendSelectableAllCards();
    for( int i = 0; i < mPlayerList.size(); ++i ){
        if( mPlayerList.at( i ) != currentPlayer ){
            mPlayerList.at( i )->sendOpponentInAction();
        }
    }
    
}

/*void Server::roundOver()
{
    kDebug() << "---- Round Over ----";
    
    Player* currentPlayer = getCurrentPlayer();
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
        //}else if( currentPlayer->getNumberOfCardsInHandNow() == 0 ){
        }else if( currentPlayer->getNumberOfCardsInHand() == 0 ){
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

}*/

void Server::roundOver()
{
    kDebug() << "---- Round Over ----";
    
    Player* currentPlayer = getCurrentPlayer();
    Player* nextPlayer = getNextPlayer();
    
    Player* winnerPlayer = 0; //Initialize with 0, becouse the compiler write warning.
    Player* looserPlayer = 0; // --- || ---
    
    //Who win the round
    if( mSchnapsenButtonClickedThisRound ){
        
        for( int i = 0; i < mPlayerList.size(); ++i ){
            if( mPlayerList.at( mRoundStarterPlayerId ) != mPlayerList.at( i ) ){
            
                if( mPlayerList.at( i )->getTricks() > 0 ){
                    winnerPlayer = mPlayerList.at( i );
                    looserPlayer = mPlayerList.at( mRoundStarterPlayerId );
                }else{
                    winnerPlayer = mPlayerList.at( mRoundStarterPlayerId );
                    looserPlayer = mPlayerList.at( i );
                }
                
            }
        }
        
    }else{
        if( mPlayerWhoClickedToCloseButtonThisRound ){
            
            if( mPlayerWhoClickedToCloseButtonThisRound == currentPlayer && currentPlayer->getTricks() >= WIN_TRICKS ){
                winnerPlayer = currentPlayer;
                looserPlayer = nextPlayer;
            }else if( mPlayerWhoClickedToCloseButtonThisRound == nextPlayer && nextPlayer->getTricks() >= WIN_TRICKS ){
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
            
            if( currentPlayer->getTricks() >= WIN_TRICKS ){
                winnerPlayer = currentPlayer;
                looserPlayer = nextPlayer;
            }else if( nextPlayer->getTricks() >= WIN_TRICKS ){
                winnerPlayer = nextPlayer;
                looserPlayer = currentPlayer;
            //}else if( currentPlayer->getNumberOfCardsInHandNow() == 0 ){
            }else if( currentPlayer->getNumberOfCards() == 0 ){
                winnerPlayer = currentPlayer;
                looserPlayer = nextPlayer;
            }

        }
    }
        
    //Winning scores
    int scores = 0;
    
    if( mSchnapsenButtonClickedThisRound ){
        
        scores = 6;
        
    }else{
    
        if( mPlayerWhoClickedToCloseButtonThisRound == winnerPlayer ){
            
            if( looserPlayer->getTricks() == 0 ){
                scores = 3;
            }else if( looserPlayer->getTricks() < HALF_WIN_POINTS ){
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
            }else if( looserPlayer->getTricks() < HALF_WIN_POINTS ){
                scores = 2;
            }else{ //  33 <= looserPlayer->getTricks() && looserPlayer->getTricks() < 66
                scores = 1;
            }
            
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
        if( mPlayerList.at( i )->getScores() >= WIN_SCORES ){
            return true;
        }
    }
        
    return false;
}

bool Server::isRoundOver()
{
    //If a player have equal or more then 66 tricks, will have win the round
    for( int i = 0; i < mPlayerList.size(); ++i ){
        if( mPlayerList.at( i )->getTricks() >= WIN_TRICKS ){ 
            return true;
        }
    }
        
    //If a player have not card in hand, then win the round who win the last turn
    if( getCurrentPlayer()->getNumberOfCards()  == 0 ){
        return true;
    }
    
    //If someone meld schnapsen and the opponent have tricks
    if( mSchnapsenButtonClickedThisRound ){
        for( int i = 0; i < mPlayerList.size(); ++i ){
            if( mPlayerList.at( mRoundStarterPlayerId ) != mPlayerList.at( i ) && mPlayerList.at( i )->getTricks() > 0 ){
                return true;
            }
        }
    }
    
    return false;
}

void Server::addNewCard( Player* player, Card* card )
{
    player->sendNewCard( card );
    
    //Send other players
    for( int i = 0; i < mPlayerList.size(); ++i ){
        if( mPlayerList.at( i ) != player ){
            mPlayerList.at( i )->sendNewOpponentCard();
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

void Server::addTrumpCardToPlayer( Player* player )
{
    //player->sendNewCardTrumpCard();
    player->sendNewCardTrumpCard( mTrump->takeCard() );
    
    for( int i = 0; i < mPlayerList.size(); ++i ){
        if( mPlayerList.at( i ) != player ){
            mPlayerList.at( i )->sendNewOpponentCardTrumpCard();
        }
    }
    
    //mTrump->clearTrumpCard();
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
        
        connect( player, SIGNAL( signalSelectedCardId( int ) ),         this, SLOT( slotPlayerSelectedCardId( int ) ) );
        connect( player, SIGNAL( signalTwentyButtonClicked() ),         this, SLOT( slotPlayerTwentyButtonClicked() ) );
        connect( player, SIGNAL( signalFortyButtonClicked() ),          this, SLOT( slotPlayerFortyButtonClicked() ) );
        connect( player, SIGNAL( signalSchnapsenButtonClicked() ),      this, SLOT( slotPlayerSchnapsenButtonClicked() ) );
        connect( player, SIGNAL( signalCloseButtonClicked() ),          this, SLOT( slotPlayerClickedToCloseButton() ) );
        connect( player, SIGNAL( signalChangeTrumpCard( Player* ) ),    this, SLOT( slotPlayerChangeTrumpCard( Player* ) ) );
        connect( player, SIGNAL( signalStartNextRound( Player* ) ),     this, SLOT( slotPlayerWantStartNextRound( Player* ) ) );
        connect( player, SIGNAL( signalStartNextGame( Player* ) ),      this, SLOT( slotPlayerWantStartNextGame( Player* ) ) );
        
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

void Server::slotPlayerSelectedCardId( int selectedCardId )
{
    kDebug() << "Selected card:" << getCurrentPlayer()->getCard( selectedCardId )->getCardText();
    
    //If the trump card is selectable then wont selectable.
    if( !mTrump->isEmpty() && mTrump->getCard()->isSelectable() ){
        mTrump->getCard()->setSelectable( false );
    }
    
    //if( mCentralCards->isEmpty() ){
    if( mCentralCards.empty() ){
        kDebug() << "is empty";
        
        if( mTwentyButtonClickedThisTurn || mFortyButtonClickedThisTurn ){
            
            int posOfPairOfCard = getCurrentPlayer()->getPositionOfPairOfCardId( selectedCardId );
            
            kDebug() << "Position of card:" << selectedCardId << "position of pair of card:" << posOfPairOfCard;
            
            getNextPlayer()->sendShowOpponentCards( selectedCardId, 
                                                    getCurrentPlayer()->getCard( selectedCardId ),
                                                    posOfPairOfCard,
                                                    getCurrentPlayer()->getCard( posOfPairOfCard ) );
            
            if( getCurrentPlayer()->getTricks() > 0 ){
                
                if( mTwentyButtonClickedThisTurn ){
                    addTricks( getCurrentPlayer(), 20 );
                }else{ // mTwentyButtonClickedThisTurn
                    addTricks( getCurrentPlayer(), 40 );
                }
                
                if( isRoundOver() ){
                    roundOver();
                    
                    for( int i = 0; i < mPlayerList.size(); ++i ){
                        mPlayerList.at( i )->sendCommandsEnd();
                    }
                    
                    return;
                }
                
            }else{ // getCurrentPlayer()->getTricks() > 0
                mWaitingMarriage = new QPair< Player*, int >;
                mWaitingMarriage->first = getCurrentPlayer();
                
                if( mTwentyButtonClickedThisTurn ){
                    mWaitingMarriage->second = 20;
                }else{ //mTwentyButtonClickedThisTurn
                    mWaitingMarriage->second = 40;
                }
            }
            
            mTwentyButtonClickedThisTurn = false;
            mFortyButtonClickedThisTurn = false;            
        }
        
        if( mClickedToCloseButtonThisTurn ){
            getNextPlayer()->sendOpponentClickedToCloseButton();
        }
        
        //getNextPlayer()->sendOpponentSelectedCardId( selectedCardId );
        
        //mCentralCards->add( getCurrentPlayer()->takeCard( selectedCardId ) );
        
        //getNextPlayer()->sendOpponentAddNewCentralCard( mCentralCards->getCard( 0 ) );
        Card* selectedCard = getCurrentPlayer()->takeCard( selectedCardId );
        //mCentralCards->add( selectedCard );
        mCentralCards.append( selectedCard );
        getNextPlayer()->sendOpponentSelectedCard( selectedCardId, selectedCard );
        
        setCurrentPlayer( getNextPlayer() );
        
        //if( !mPlayerWhoClickedToCloseButtonThisRound && mDeck->getDeckSize() > 0 ){
        if( !mPlayerWhoClickedToCloseButtonThisRound && !mCardDeck.empty() ){
            getCurrentPlayer()->sendSelectableAllCards();
            getNextPlayer()->sendOpponentInAction();
        }else{ // mDeck->getDeckSize() == 0
            getCurrentPlayer()->sendSelectableCertainCards( mCentralCards, mTrump );
            getNextPlayer()->sendOpponentInAction();
        }
        
        for( int i = 0; i < mPlayerList.size(); ++i ){
            mPlayerList.at( i )->sendCommandsEnd();
        }
        
    }else{ //mCentralCards->isEmpty()
        kDebug() << "isn't empty";
        //getNextPlayer()->sendOpponentSelectedCardId( selectedCardId );
        
        //mCentralCards->add( getCurrentPlayer()->takeCard( selectedCardId ) );
        
        //getNextPlayer()->sendOpponentAddNewCentralCard( mCentralCards->getCard( 1 ) );
        Card* selectedCard = getCurrentPlayer()->takeCard( selectedCardId );
        //mCentralCards->add( selectedCard );
        mCentralCards.append( selectedCard );
        getNextPlayer()->sendOpponentSelectedCard( selectedCardId, selectedCard );
        
        QTimer::singleShot( 1000, this, SLOT( slotCheckCentralCards() ) );
        
        //
        getNextPlayer()->sendCommandsEnd();
    }
    
    /*for( int i = 0; i < mPlayerList.size(); ++i ){
        mPlayerList.at( i )->sendCommandsEnd();
    }*/
}


void Server::slotPlayerTwentyButtonClicked()
{
    kDebug() << getCurrentPlayer()->getName() << "Clicked to twenty button.";
    getCurrentPlayer()->setSelectableRegularMarriageCards( mTrump );
    
    mTwentyButtonClickedThisTurn = true;
    
    //Send to the other(s) the player clicked to the twenty button
    for( int i = 0; i < mPlayerList.size(); ++i ){
        if( mPlayerList.at( i ) != getCurrentPlayer() ){
            mPlayerList.at( i )->sendOpponentTwentyButtonClicked();
        }
    }
}

void Server::slotPlayerFortyButtonClicked()
{
    kDebug() << getCurrentPlayer()->getName() << "Clicked to forty button.";
    getCurrentPlayer()->setSelectableTrumpMarriageCards( mTrump );
    
    mFortyButtonClickedThisTurn = true;
    
    //Send to the other(s) the player clicked to the forty button
    for( int i = 0; i < mPlayerList.size(); ++i ){
        if( mPlayerList.at( i ) != getCurrentPlayer() ){
            mPlayerList.at( i )->sendOpponentFortyButtonClicked();
        }
    }
}

void Server::slotPlayerSchnapsenButtonClicked()
{
    kDebug() << getCurrentPlayer()->getName() << "Clicked to schnapsen button.";
    
    mSchnapsenButtonClickedThisRound = true;
    
    //Send to the other(s) the player clicked to the schnapsen button
    for( int i = 0; i < mPlayerList.size(); ++i ){
        if( mPlayerList.at( i ) != getCurrentPlayer() ){
            mPlayerList.at( i )->sendOpponentSchnapsenButtonClicked();
        }
    }
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
    
    /*mTrump->getCard()->setSelectable( false );
    player->getCard( id )->setSelectable( true );*/
    mTrump->getCard()->setSelectable( false );
    player->getCard( player->getNumberOfCards() - 1 )->setSelectable( true );
        
    if( player->haveRegularMarriage( mTrump ) ){
        player->setTwentyButtonVisible( true );
    }
    
    if( player->haveTrumpMarriage( mTrump ) ){
        player->setFortyButtonVisible( true );
    }
    
    for( int i = 0; i < mPlayerList.size(); ++i ){
        
        if( mPlayerList.at(i) != player ){
            //mPlayerList.at(i)->sendNewTrumpCard( mTrump );
            mPlayerList.at( i )->sendOpponentChangeTrumpCard( id, mTrump );
        }
        
    }
    
    kDebug() << "End.";
}

void Server::slotCheckCentralCards()
{
    kDebug() << "Check the central cards.";
    
    //const int centralCard0Point = mCentralCards->getCard( 0 )->getCardPoint();
    const int centralCard0Point = mCentralCards.first()->getCardPoint();    
    //const int centralCard1Point = mCentralCards->getCard( 1 )->getCardPoint();
    const int centralCard1Point = mCentralCards.last()->getCardPoint();
    
    kDebug() << "centralCard0Point" << centralCard0Point;
    kDebug() << "centralCard1Point" << centralCard1Point;
    
    //const Card::Suit centralCard0Suit = mCentralCards->getCard( 0 )->getSuit();
    const Card::Suit centralCard0Suit = mCentralCards.first()->getSuit();
    //const Card::Suit centralCard1Suit = mCentralCards->getCard( 1 )->getSuit();
    const Card::Suit centralCard1Suit = mCentralCards.last()->getSuit();
    
    bool currentPlayerStartNextTurn;
    
    if( mTrump->getCardSuit() == centralCard0Suit && mTrump->getCardSuit() != centralCard1Suit ){
        currentPlayerStartNextTurn = false;
    }else if( mTrump->getCardSuit() != centralCard0Suit && mTrump->getCardSuit() == centralCard1Suit ){
        currentPlayerStartNextTurn = true;
    }else if( centralCard0Suit == centralCard1Suit ){
    
        if( centralCard0Point < centralCard1Point ){
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
    
    kDebug() << getCurrentPlayer()->getName() << "get" << centralCard0Point+centralCard1Point << "points.";
    addTricks( getCurrentPlayer(), centralCard0Point + centralCard1Point );
    
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
        clearCentralCards();
        
        for( int i = 0; i < mPlayerList.size(); ++i ){
            
            if( getCurrentPlayer() == mPlayerList.at( i ) ){
                mPlayerList.at( i )->sendPlayerGetCentralCards();
            }else{
                mPlayerList.at( i )->sendOpponentGetCentralCards();
            }
            
        }
        
        //If player closed the deck
        if( !mPlayerWhoClickedToCloseButtonThisRound ){
            //If the dech have card yet, then add new cards to players, first who won the last turn
            //if( mDeck->getDeckSize() > 0 ){
            if( !mCardDeck.empty() ){
                //addNewCard( getCurrentPlayer(), mDeck->takeCard() );
                addNewCard( getCurrentPlayer(), mCardDeck.takeLast() );
                
                //if( mDeck->getDeckSize() > 0 ){
                if( !mCardDeck.empty() ){
                    //addNewCard( getNextPlayer(), mDeck->takeCard() );
                    addNewCard( getNextPlayer(), mCardDeck.takeLast() );
                    
                }else{
                    //addNewCard( getNextPlayer(), mTrump->getCard() );
                    
                    //mTrump->clearTrumpCard( false );
                    
                    /*for( int i = 0; i < mPlayerList.size(); ++i ){
                        mPlayerList.at( i )->sendClearTrumpCard();
                    }*/
                    
                    addTrumpCardToPlayer( getNextPlayer() );
                }
                
            }
        }
        
        getCurrentPlayer()->sendSelectableAllCards();
        for( int i = 0; i < mPlayerList.size(); ++i ){
            if( mPlayerList.at( i ) != getCurrentPlayer() ){
                mPlayerList.at( i )->sendOpponentInAction();
            }
        }
        
        if( getCurrentPlayer()->haveRegularMarriage( mTrump ) ){
            kDebug() << getCurrentPlayer()->getName() << "have regular marriages.";
            getCurrentPlayer()->setTwentyButtonVisible( true );
            getCurrentPlayer()->sendTwentyButtonVisible();
        }
        
        if( getCurrentPlayer()->haveTrumpMarriage( mTrump ) ){
            kDebug() << getCurrentPlayer()->getName() << "have trump marriages.";
            getCurrentPlayer()->setFortyButtonVisible( true );
            getCurrentPlayer()->sendFortyButtonVisible();
        }
        
        if( getCurrentPlayer()->canChangeTrumpCard( mTrump ) ){
            kDebug() << getCurrentPlayer()->getName() << "can change trump card.";
            getCurrentPlayer()->sendSelectableTrumpCard();
        }
        
        //if( !mPlayerWhoClickedToCloseButtonThisRound && mDeck->getDeckSize() > 3 ){
        if( !mPlayerWhoClickedToCloseButtonThisRound && mCardDeck.size() > 3 ){
            getCurrentPlayer()->sendCloseButtonVisible();
        }
        
    }
    
    for( int i = 0; i < mPlayerList.size(); ++i ){
        mPlayerList.at( i )->sendCommandsEnd();
    }
    
}

/*void Server::slotPlayerWantStartNextRound( Player *player )
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
}*/

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

void Server::addBot( QString name, Knapsen::GameDifficulty difficulty )
{
    mBot = new Bot( name, difficulty );
    
    mBot->connectToHost( QHostAddress::LocalHost , serverPort() );
}

void Server::startGame()
{
    kDebug() << "Start game.";
    
    for( int i = 0; i < mPlayerList.size(); ++i ){
        for( int j = 0; j < mPlayerList.size(); ++j ){
            if( mPlayerList.at( j ) != mPlayerList.at( i ) ){
                mPlayerList.at( i )->sendInitializeTable( mPlayerList.at( j )->getName(), mTypeOfCards, mSizeOfDeck );
            }
        }
    }
    
    newGame();

    for( int i = 0; i < mPlayerList.size(); ++i ){
        mPlayerList.at( i )->sendCommandsEnd();
    }
}
