#include <QtCore/QTimer>
#include <KDE/KDebug>
#include "centralcards.h"
#include "trumpcard.h"
#include "bot.h"

const int INVALID_CARD_ID = -1;

Bot::Bot( QString name, Knapsen::GameDifficulty difficulty ):
	Client( name ),
	mDifficulty( difficulty ),
	mDeckIsClosed( false ) 
{
	kDebug() << "Initialize.";
	
	//connect( this, SIGNAL( signalOpponentDisconnected() ),			this, SLOT( slotOpponentDisconnected() ) );
	//connect( this, SIGNAL( signalGameError( Client::GameErrorType ) ),	this, SLOT( slotGameError( Client::GameErrorType ) ) );
	connect( this, SIGNAL( signalPlayerInAction() ),					this, SLOT( slotPlayerInAction() ) );
	connect( this, SIGNAL( signalNewRound() ),							this, SLOT( slotNewRound() ) );
	connect( this, SIGNAL( signalCloseDeck() ),							this, SLOT( slotCloseDeck() ) );
	connect( this, SIGNAL( signalNewCentralCard( int , const Card* ) ),                  this, SLOT( slotNewCentralCard( int, const Card* ) ) );
	//connect( this, SIGNAL( signalEndRound( QString, int ) ),			this, SLOT( slotEndRound( QString, int ) ) );
	//connect( this, SIGNAL( signalEndGame( QString ) ),					this, SLOT( slotendGame( QString ) ) );
	
	//
	pairOfQueenWasInCentralCards[0].first = pairOfKingWasInCentralCards[0].first = Card::Heart;
	pairOfQueenWasInCentralCards[1].first = pairOfKingWasInCentralCards[1].first = Card::Diamond;
	pairOfQueenWasInCentralCards[2].first = pairOfKingWasInCentralCards[2].first = Card::Spade;
	pairOfQueenWasInCentralCards[3].first = pairOfKingWasInCentralCards[3].first = Card::Club;
}

bool Bot::getPairOfCardWasInCentral( Card::CardType cardType, Card::CardSuit cardSuit )
{
    for( int i = 0; i < 4; ++i ){
        if( cardType == Card::King && pairOfKingWasInCentralCards[i].first == cardSuit ){
            return pairOfKingWasInCentralCards[i].second;
        }
        if( cardType == Card::Queen && pairOfQueenWasInCentralCards[i].first == cardSuit ){
            return pairOfQueenWasInCentralCards[i].second;
        }
        
    }
    
    return false;
}

/*void Bot::slotGameError( Client::GameErrorType )
{
	kDebug() << getName() << "Game error.";
	
	disconnectFromHost();
}*/

void Bot::slotPlayerInAction()
{
	QTimer::singleShot( 1000, this, SLOT( slotSelectCard() ) );
}

void Bot::slotSelectCard()
{
    kDebug() << "In Action.";
    
    kDebug() << "Bot cards:";
    for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
        if( getCard( i ) != 0 ){
            kDebug() << getCard( i )->getCardText();
        }
    }
    kDebug() << "";

    
    if( mDifficulty == Knapsen::Easy ){
        easy();
    }else if( mDifficulty == Knapsen::Medium ){
        medium();
    }else{
        hard();
    }
    
}

void Bot::easy()
{
    kDebug() << "Easy";
    
    slotSelectCardId( getRandomCardId() );
}

void Bot::medium()
{
    kDebug() << "Medium";
    
    if( getCentralCards()->isEmpty() ){
        tryChangeTrumpCard();
        
        bool meld = tryMeldMarriage();
        
        if( meld ){
            trySelect( Card::King );
        }else{ //meld
            
            int rand = qrand()%8;
            kDebug() << "rand:" << rand;
            
            switch( rand ){
                case 0 : if( trySelectNotEqualTrump( Card::Ace ) ) break;
                case 1 : if( trySelectNotEqualTrump( Card::Ten ) ) break;
                case 2 : if( trySelectPairWasNotInCentral( Card::King ) ) break;
                case 3 : if( trySelectPairWasNotInCentral( Card::Queen ) ) break;
                case 4 : if( trySelectEqualTrump( Card::Ace ) ) break;
                case 5 : if( trySelectEqualTrump( Card::Ten ) ) break;
                case 6 : if( trySelect( Card::Jack ) ) break;
                case 7 : if( trySelect( Card::Nine ) ) break;
                case 8 : slotSelectCardId( getRandomCardId() );
            }
           
        }
        
    }else{ //getCentralCards()->isEmpty()
        
        int rand;
        
        if( qrand()%3 != 0 ){
            kDebug() << "rand=0";
            rand = 0;
        }else{
            rand = qrand()%10;
            kDebug() << "rand:" << rand;
        }
        
        switch( rand ){
            case 0 : if( trySelectEqualCentralMinPoints() ) break;
            case 1 : if( trySelectEqualTrumpMinPoints() ) break;
            //Bot cannot win
            case 2 : if( trySelect( Card::Nine ) ) break;
            case 3 : if( trySelect( Card::Jack ) ) break;
            case 4 : if( trySelectPairWasNotInCentral( Card::Queen ) ) break;
            case 5 : if( trySelectPairWasNotInCentral( Card::King ) ) break;
            case 6 : if( trySelect( Card::Ten ) ) break;
            case 7 : if( trySelect( Card::Ace ) ) break;
            case 8 : if( trySelect( Card::Queen ) ) break;
            case 9 : if( trySelect( Card::King ) ) break;
            case 10 : slotSelectCardId( getRandomCardId() );
        }
    }
    
}

void Bot::hard()
{
    kDebug() << "Hard";
    
    if( getCentralCards()->isEmpty() ){
        tryChangeTrumpCard();
        
        bool meld = tryMeldMarriage();
        
        if( meld ){
            trySelect( Card::King );
        }else{ // meld
            
            switch( 0 ){
                case 0 : if( trySelectNotEqualTrump( Card::Ace ) ) break;
                case 1 : if( trySelectNotEqualTrump( Card::Ten ) ) break;
                case 2 : if( trySelectPairWasNotInCentral( Card::King ) ) break;
                case 3 : if( trySelectPairWasNotInCentral( Card::Queen ) ) break;
                case 4 : if( trySelectEqualTrump( Card::Ace ) ) break;
                case 5 : if( trySelectEqualTrump( Card::Ten ) ) break;
                case 6 : if( trySelect( Card::Jack ) ) break;
                case 7 : trySelect( Card::Nine );
            }
            
        }
        
    }else{ // getCentralCards()->isEmpty()
        
        switch( 0 ){
            case 0 : if( trySelectEqualCentralMinPoints() ) break;
            case 1 : if( trySelectEqualTrumpMinPoints() ) break;
            //Bot cannot win
            case 2 : if( trySelect( Card::Nine ) ) break;
            case 3 : if( trySelect( Card::Jack ) ) break;
            case 4 : if( trySelectPairWasNotInCentral( Card::Queen ) ) break;
            case 5 : if( trySelectPairWasNotInCentral( Card::King ) ) break;
            case 6 : if( trySelect( Card::Ten ) ) break;
            case 7 : if( trySelect( Card::Ace ) ) break;
            case 8 : if( trySelect( Card::Queen ) ) break;
            case 9 : trySelect( Card::King );
        }
        
    }
    
}

int Bot::getRandomCardId()
{
    kDebug() << "Return a random card id";
    
    QList<int> selectableCards;
    for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
        if( getCard( i ) != 0 && getCard( i )->isSelectable() ){
            selectableCards.append( i );
        }
    }
    
    return selectableCards.at( qrand()%selectableCards.size() );
}

void Bot::tryChangeTrumpCard()
{
    kDebug() << "Try change trump card.";
    
    if( !getTrumpCard()->isEmpty() && getTrumpCard()->getCard()->isSelectable() ){
        kDebug() << "Can change trump card. Change it.";
        
        slotSelectTrumpCard();
    }
}

bool Bot::tryMeldMarriage()
{
    kDebug() << "Try meld marriage.";

    bool clicktedToFortyOrTwentyButton = false;
                
    //If have forty marriage, meld this
    if( isFortyButtonVisible() ){
        kDebug() << "Click to forty button.";
        
        slotFortyButtonClicked();
        clicktedToFortyOrTwentyButton = true;
    }
    
    //If have twenty marriage, meld this
    if( isTwentyButtonVisible() ){
        kDebug() << "Click to twenty button.";
        
        slotTwentyButtonClicked();
        clicktedToFortyOrTwentyButton = true;
    }
    
    return clicktedToFortyOrTwentyButton;
}

bool Bot::trySelectNotEqualTrump( Card::CardType cardType )
{
    kDebug() << "If have" << cardType << "and the type of card is not equal with type of trump card then select card.";
    Card* card;
    Card::CardSuit trumpCardSuit = getTrumpCard()->getCard()->getCardSuit();
    
    for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
        card = getCard( i );
        
        if( card != 0 && card->isSelectable() && ( card->getCardSuit() != trumpCardSuit ) && ( card->getCardType() == cardType ) ){
            slotSelectCardId( i );
            return true;
        }
                                
    }
    
    return false;
}

bool Bot::trySelectPairWasNotInCentral( Card::CardType cardType )
{
    kDebug() << "If have card, what the pair of card was in the central cards _OR_ the deck of cards count 0 then select card.";
    
    Card::CardType pairCardType;
    if( cardType == Card::King ){
        pairCardType = Card::Queen;
    }else{ //cardType == Card::King
        pairCardType = Card::King;
    }
    
    Card* card;
    
    for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
        card = getCard( i );
        
        if( card != 0 && card->isSelectable() && ( card->getCardType() == cardType ) && ( mDeckIsClosed || getSizeOfDeckNow() == 0 || ( getPairOfCardWasInCentral( pairCardType, card->getCardSuit() ) ) ) ){
            slotSelectCardId( i );
            return true;
        }
        
    }
    
    return false;
}

bool Bot::trySelectEqualTrump( Card::CardType cardType )
{
    kDebug() << "If have card, what the type of card is equal with the type of trump card, then select card.";
    
    Card* card;
    Card::CardSuit trumpCardSuit = getTrumpCard()->getCard()->getCardSuit();
    
    for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
        card = getCard( i );
        
        if( card != 0 && card->isSelectable() && ( card->getCardSuit() == trumpCardSuit ) && ( card->getCardType() == cardType ) ){
            slotSelectCardId( i );
            return true;
        }
                                
    }
    
    return false;
}

bool Bot::trySelect( Card::CardType cardType )
{
    kDebug() << "If have " << cardType << " then I will select this.";
    
    for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
        if( getCard( i ) != 0 && getCard( i )->isSelectable() && getCard( i )->getCardType() == cardType ){
            slotSelectCardId( i );
            return true;
        }
    }
    
    return false;
}

bool Bot::trySelectEqualCentralMinPoints()
{
    kDebug() << "Try select the smallest points card, what's suit is equal whith central card's suit and the card's point is more then central card's point.";
    
    Card* card;
    int selectCardId = INVALID_CARD_ID;
    Card* centralCard0 = getCentralCards()->getCard(0);
                
    for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
        
        card = getCard( i );
        
        if( card != 0 &&
            card->isSelectable() &&
            card->getCardSuit() == centralCard0->getCardSuit() &&
            card->getCardPoint() > centralCard0->getCardPoint() )
        {
            if( ( getSizeOfDeckNow() == 0 || mDeckIsClosed ) ||
                ( ( card->getCardType() == Card::King && getPairOfCardWasInCentral( Card::King, card->getCardSuit() ) ) ||
                  ( card->getCardType() == Card::Queen && getPairOfCardWasInCentral( Card::Queen, card->getCardSuit() ) ) ||
                  ( card->getCardType() != Card::King && card->getCardType() != Card::Queen ) )   
            ){
                if( ( selectCardId == INVALID_CARD_ID ) ||
                    ( card->getCardPoint() < getCard( selectCardId )->getCardPoint()  )
                ){
                    selectCardId = i;
                }
            }
            
            
        }
        
    }
    
    if( selectCardId != INVALID_CARD_ID ){
       slotSelectCardId( selectCardId );
       return true;
    }
    
    return false;
}

bool Bot::trySelectEqualTrumpMinPoints()
{
    kDebug() << "Try select the smallest points card, what's suit is equal whith trump card's suit and the card's point is more then central card's point.";
    
    if( getCentralCards()->getCard(0)->getCardSuit() != getTrumpCard()->getCardSuit() ){ //If is equal, that was check the previous function
        Card* card;
        int selectCardId = INVALID_CARD_ID;
        
        for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
            
            card = getCard( i );
            
            if( card != 0 &&
                card->isSelectable() &&
                card->getCardSuit() == getTrumpCard()->getCardSuit() )
            {
                if( ( getSizeOfDeckNow() == 0 || mDeckIsClosed ) ||
                    ( ( card->getCardType() == Card::King && getPairOfCardWasInCentral( Card::King, card->getCardSuit() ) ) ||
                    ( card->getCardType() == Card::Queen && getPairOfCardWasInCentral( Card::Queen, card->getCardSuit() ) ) ||
                    ( card->getCardType() != Card::King && card->getCardType() != Card::Queen ) )   
                ){
                    if( ( selectCardId == INVALID_CARD_ID ) ||
                        ( card->getCardPoint() < getCard( selectCardId )->getCardPoint()  )
                    ){
                        selectCardId = i;
                    }
                }
            }
        }
    
        if( selectCardId != INVALID_CARD_ID ){
            slotSelectCardId( selectCardId );
            return true;
        }
    }
    
    return false;
}

void Bot::slotNewRound()
{
	mDeckIsClosed = false;
        
        for( int i = 0; i < 4; ++i ){
            pairOfQueenWasInCentralCards[i].second = pairOfKingWasInCentralCards[i].second = false;
        }
}

void Bot::slotNewCentralCard( int , const Card* card )
{
    if( card->getCardType() == Card::Queen ){
    
        for( int i = 0; i < 4; ++i ){
        
            if( pairOfKingWasInCentralCards[i].first == card->getCardSuit() ){
                pairOfKingWasInCentralCards[i].second = true;
            }
                        
        }
                
    }else if( card->getCardType() == Card::King ){
    
        for( int i = 0; i < 4; ++i ){
        
            if( pairOfQueenWasInCentralCards[i].first == card->getCardSuit() ){
                pairOfQueenWasInCentralCards[i].second = true;
            }
                        
        }
                
    }
}

/*void Bot::slotEndRound( QString /roundWinnerName/ , int /scores/ )
{
	startNextRound();
}
*/

/*void Bot::slotendGame( QString /gameWinnerName/ )
{
	startNextGame();
}
*/
