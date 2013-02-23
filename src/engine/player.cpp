#include <KDE/KDebug>
#include "centralcards.h"
#include "trump.h"
#include "player.h"

Player::Player( QString name ): 
    mName( name ),
    mLowestCardType( Card::Jack ),
    //mNumberOfCardsInHand( 0 ),
    mTricks( 0 ),
    mScores( 0 ),
    mTwentyButtonVisible( false ),
    mFortyButtonVisible( false ),
    mCloseButtonVisible( false ),
    mSchnapsenButtonVisible( false )
{       
    //mCards = 0;
    
    connect( this, SIGNAL( readyRead() ), this, SLOT( slotReadyRead() ) );
    connect( this, SIGNAL( disconnected() ), this, SLOT( slotDisconnected() ) );
}

Player::~Player()
{
    kDebug() << getName() << "deleting.";
    
    /*if( mCards != 0 ){
        for( int i = 0; i < mNumberOfCardsInHand; ++i ){
            if( mCards[i] != 0 ){
                delete mCards[i];
            }
        }
        delete[] mCards;
    }*/
}

QString Player::getCommandPartOfCommand( QString text )
{
    if( text.indexOf('=') == -1 ){
        return text;
    }
    
    return text.mid( 0, text.indexOf( '=' )+1 ); //The '=' too.
}

QString Player::getValuePartOfCommand( QString text )
{
    return text.mid( text.indexOf( '=' )+1 );
}

/*void Player::sendInitializeTable( QString opponentName, Knapsen::TypeOfCards typeOfCards, int sizeOfDeck, int numberOfCardsInHand )
{
    QString typeOfCardsValue;
    if( typeOfCards == Knapsen::GermanSuits ){
        typeOfCardsValue = TYPE_OF_CARDS_GERMAN_SUITS_VALUE;
    }else{ // typeOfCards == Knapsen::FrenchSuits
        typeOfCardsValue = TYPE_OF_CARDS_FRENCH_SUITS_VALUE;
    }
    
    setLowestCard( sizeOfDeck );
    
    //setNumberOfCardsInHand( numberOfCardsInHand );
    
    sendCommand( INITIALIZE_TABLE_COMMAND+opponentName+","+typeOfCardsValue+","+QString::number( sizeOfDeck )+","+QString::number( numberOfCardsInHand ) );
}*/

void Player::sendInitializeTable( QString opponentName, Knapsen::TypeOfCards typeOfCards, int sizeOfDeck )
{
    QString typeOfCardsValue;
    if( typeOfCards == Knapsen::GermanSuits ){
        typeOfCardsValue = TYPE_OF_CARDS_GERMAN_SUITS_VALUE;
    }else{ // typeOfCards == Knapsen::FrenchSuits
        typeOfCardsValue = TYPE_OF_CARDS_FRENCH_SUITS_VALUE;
    }
    
    setLowestCard( sizeOfDeck );
    
    //setNumberOfCardsInHand( numberOfCardsInHand );
    
    sendCommand( INITIALIZE_TABLE_COMMAND+opponentName+","+typeOfCardsValue+","+QString::number( sizeOfDeck ) );
}

/*void Player::setNumberOfCardsInHand( int size )
{
    mNumberOfCardsInHand = size;
    
    mCards = new Card*[ mNumberOfCardsInHand ];
    
    for( int i = 0; i < size; ++i ){
        mCards[ i ] = 0;
    }
}*/

/*int Player::addNewCard( Card *card )
{
    int cardId = -1;
    
    for( int i = 0; i < mNumberOfCardsInHand; ++i ){
        if( mCards[ i ] == 0 ){
            mCards[ i ] = card;
            cardId = i;
            break;
        }
    }
    return cardId;
}*/

/*void Player::removeCard( int id )
{
    //Why not delete? Becouse when call the removeCard(int) function when move the card to central cards 
    //mCards[ id ] = 0;
    
    
}*/

int Player::changeTrumpCard( Trump *trump )
{
    /*//This value will be modify. Only for don't write warning the compiler.
    
    for( int i = 0; i < mNumberOfCardsInHand; ++i ){
        if( mCards[i] != 0 && mCards[i]->getSuit() == trump->getCardSuit() && mCards[i]->getType() == mLowestCardType ){
            kDebug() << i;
            
            Card *tmpCard = trump->getCard();
            trump->addNewCard( mCards[i] );
            mCards[i]=tmpCard;
            
            return i;
        }
    }*/
    
    for( int i = 0; i < mCards.size(); ++i ){
        if( mCards.at( i )->getSuit() == trump->getCardSuit() && mCards.at( i )->getType() == mLowestCardType ){
            kDebug() << i;
            
            Card* tmpCard = trump->getCard();
            trump->addNewCard( mCards.at( i ) );
            mCards[ i ] = tmpCard;
            
            return i;
        }
    }
    
    return -1; //If this function return this value, then this is a bug.
}

void Player::setSelectableAllCards( bool enabled )
{
    /*for( int i = 0; i < mNumberOfCardsInHand; ++i ){
        if( mCards[ i ] != 0 ){
            mCards[ i ]->setSelectable( enabled );
            emit signalPlayerCardSelectableChanged( i, enabled );
        }
    }*/
    
    for( int i = 0; i < mCards.size(); ++i ){
        mCards.at(i)->setSelectable( enabled );
        emit signalPlayerCardSelectableChanged( i, enabled );
    }
}

void Player::setSelectableCertainCards( CentralCards *centralCards, Trump *trump )
{
    //Set true, which cards equal type whit central card
    if( setSelectableCardsWhatEqualSuit( centralCards->getCard( 0 )->getSuit() ) == false ){
        //If have not cards which equal type whith central card, then set true, which equal type whith trump card 
        if( setSelectableCardsWhatEqualSuit( trump->getCardSuit() ) == false ){
            //If have not that cards, then equal all card
            setSelectableAllCards( true );    
        }
    }
}

void Player::setSelectableRegularMarriagesCards( Trump* trump )
{
    /*for( int i = 0; i < mNumberOfCardsInHand; ++i ){
        
        if( mCards[ i ] != 0 && 
            mCards[ i ]->getSuit() != trump->getCardSuit() && 
            mCards[ i ]->getType() == Card::King
        ){
            for( int j = 0; j < mNumberOfCardsInHand; ++j ){
            
                if( mCards[ j ] != 0 &&
                    mCards[ j ]->getSuit() == mCards[ i ]->getSuit() &&
                    mCards[ j ]->getType() == Card::Queen
                ){
                    mCards[ i ]->setSelectable( true );
                    mCards[ j ]->setSelectable( true );
                    emit signalPlayerCardSelectableChanged( i, mCards[ i ]->isSelectable() );
                    emit signalPlayerCardSelectableChanged( j, mCards[ j ]->isSelectable() );
                }
            }
        }
    }*/
    
    for( int i = 0; i < mCards.size(); ++i ){
        
        if( mCards.at( i )->getSuit() != trump->getCardSuit() && 
            mCards.at( i )->getType() == Card::King
        ){
            for( int j = 0; j < mCards.size(); ++j ){
            
                if( mCards.at( j )->getSuit() == mCards.at( i )->getSuit() &&
                    mCards.at( j )->getType() == Card::Queen
                ){
                    mCards.at( i )->setSelectable( true );
                    mCards.at( j )->setSelectable( true );
                    emit signalPlayerCardSelectableChanged( i, true );
                    emit signalPlayerCardSelectableChanged( j, true );
                }
            }
        }
    }
}

void Player::setSelectableTrumpMarriagesCards( Trump* trump )
{
    /*for( int i = 0; i < mNumberOfCardsInHand; ++i ){
        
        if( mCards[ i ] != 0 && 
            mCards[ i ]->getSuit() == trump->getCardSuit() && 
            mCards[ i ]->getType() == Card::King
        ){
            for( int j = 0; j < mNumberOfCardsInHand; ++j ){
                
                if( mCards[ j ] != 0 &&
                    mCards[ j ]->getSuit() == mCards[ i ]->getSuit() &&
                    mCards[ j ]->getType() == Card::Queen
                ){
                    mCards[ i ]->setSelectable( true );
                    mCards[ j ]->setSelectable( true );
                    emit signalPlayerCardSelectableChanged( i, mCards[ i ]->isSelectable() );
                    emit signalPlayerCardSelectableChanged( j, mCards[ j ]->isSelectable() );
                }
                
            }
        }

    }*/
    
    for( int i = 0; i < mCards.size(); ++i ){
        
        if( mCards.at( i )->getSuit() == trump->getCardSuit() && 
            mCards.at( i )->getType() == Card::King
        ){
            for( int j = 0; j < mCards.size(); ++j ){
                
                if( mCards.at( j )->getSuit() == mCards.at( i )->getSuit() &&
                    mCards.at( j )->getType() == Card::Queen
                ){
                    mCards.at( i )->setSelectable( true );
                    mCards.at( j )->setSelectable( true );
                    emit signalPlayerCardSelectableChanged( i, true );
                    emit signalPlayerCardSelectableChanged( j, true );
                }
                
            }
        }

    }
}

/*int Player::getNumberOfCardsInHandNow()
{
    int ret = 0;
    
    for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
        if( getCard( i ) != 0 ){
            ++ret;
        }
    }
    
    return ret;
}*/

/*Card* Player::getCard( int id )
{
    return mCards[ id ];
}*/

void Player::newGame()
{
    //Clear scores
    setScores( 0 );
    
    //Clear others in the 'newRound()'
}

void Player::newRound()
{
    //Clear cards
    while( mCards.size() != 0 ){
        Card* card = mCards.takeAt(0);
        delete card;
    }
    
    //Clear tricks
    setTricks( 0 );
}

void Player::setLowestCard( int sizeOfDeck )
{
    if( sizeOfDeck == 20 ){
        mLowestCardType = Card::Jack;
    }else{ // sizeOfDeck == 24
        mLowestCardType = Card::Nine;
    }
}

/*int Player::getPositionOfPairOfCard( const Card* card )
{
    for( int i = 0; i < mCards.size(); ++i ){
        if( card->getSuit() == mCards.at( i )->getSuit() &&
            ( ( card->getType() == Card::King && mCards.at( i )->getType() == Card::Queen ) ||
              ( card->getType() == Card::Queen && mCards.at( i )->getType() == Card::King ) )
        ){
            //return i;
            return i+1;
        }
    }
    
    //Error
    return -1;
}*/

int Player::getPositionOfPairOfCardId( int id )
{
    
    for( int i = 0; i < mCards.size(); ++i ){
        if( mCards.at( id )->getSuit() == mCards.at( i )->getSuit() &&
            ( ( mCards.at( id )->getType() == Card::King && mCards.at( i )->getType() == Card::Queen ) ||
              ( mCards.at( id )->getType() == Card::Queen && mCards.at( i )->getType() == Card::King ) )
        ){
            return i;
        }
    }
    
    //Error
    return -1;
}

bool Player::haveRegularMarriages( Trump* trump ) const
{
    /*bool ret = false;
    
    for( int i = 0; i < mNumberOfCardsInHand; ++i ){
        if( mCards[ i ] != 0 && mCards[ i ]->getSuit() != trump->getCardSuit() && mCards[ i ]->getType() == Card::King ){
            
            for( int j = 0; j < mNumberOfCardsInHand; ++j ){
                if( mCards[j] != 0 && mCards[j]->getType() == Card::Queen && mCards[j]->getSuit() == mCards[i]->getSuit() ){
                    ret = true;
                }
            }
            
        }
    }
    
    return ret;*/
    
    bool ret = false;
    
    for( int i = 0; i < mCards.size(); ++i ){
        if( mCards.at( i )->getSuit() != trump->getCardSuit() && mCards.at( i )->getType() == Card::King ){
            
            for( int j = 0; j < mCards.size(); ++j ){
                if( mCards.at( j )->getType() == Card::Queen && mCards.at( j )->getSuit() == mCards.at( i )->getSuit() ){
                    ret = true;
                }
            }
            
        }
    }
    
    return ret;
}

bool Player::haveTrumpMarriages( Trump* trump ) const
{
    /*bool ret = false;
    
    for( int i = 0; i < mNumberOfCardsInHand; ++i ){
        if( mCards[ i ] != 0 && mCards[ i ]->getSuit() == trump->getCardSuit() && mCards[ i ]->getType() == Card::King ){
            
            for( int j = 0; j < mNumberOfCardsInHand; ++j ){
                if( mCards[j] != 0 && mCards[j]->getType() == Card::Queen && mCards[j]->getSuit() == mCards[i]->getSuit() ){
                    ret = true;
                }
            }
            
        }
    }
    
    return ret;*/
    
    bool ret = false;
    
    for( int i = 0; i < mCards.size(); ++i ){
        if( mCards.at( i )->getSuit() == trump->getCardSuit() && mCards.at( i )->getType() == Card::King ){
            
            for( int j = 0; j < mCards.size(); ++j ){
                if( mCards.at( j )->getType() == Card::Queen && mCards.at( j )->getSuit() == mCards.at( i )->getSuit() ){
                    ret = true;
                }
            }
            
        }
    }
    
    return ret;
}

bool Player::canChangeTrumpCard( Trump* trump ) const
{
    /*bool canChange = false;
    
    for( int i = 0; i < mNumberOfCardsInHand; ++i ){
        
        if( mCards[i] != 0 && mCards[i]->getSuit() == trump->getCardSuit() && mCards[i]->getType() == mLowestCardType ){
            canChange = true;
        }
        
    }
    
    return canChange;*/
    
    bool canChange = false;
    
    for( int i = 0; i < mCards.size(); ++i ){
        
        if( mCards.at( i )->getSuit() == trump->getCardSuit() && mCards.at( i )->getType() == mLowestCardType ){
            canChange = true;
        }
        
    }
    
    return canChange;
}

void Player::setTwentyButtonVisible( bool twentyButtonVisible )
{
    mTwentyButtonVisible = twentyButtonVisible;
    emit signalTwentyButtonVisible( mTwentyButtonVisible );
}

void Player::setFortyButtonVisible( bool fortyButtonVisible )
{
    mFortyButtonVisible = fortyButtonVisible;
    emit signalFortyButtonVisible( mFortyButtonVisible );
}

void Player::setSchnapsenButtonVisible( bool schnapsenButtonVisible )
{
    mSchnapsenButtonVisible = schnapsenButtonVisible;
    emit signalSchnapsenButtonVisible( mSchnapsenButtonVisible );
}

void Player::twentyButtonClicked()
{
    setTwentyButtonVisible( false );
    
    if( mFortyButtonVisible ){
        setFortyButtonVisible( false );
    }
    
    setSelectableAllCards( false );
}

void Player::fortyButtonClicked()
{
    setFortyButtonVisible( false );
    
    if( mTwentyButtonVisible ){
        setTwentyButtonVisible( false );
    }
    
    setSelectableAllCards( false );
}

void Player::newCommand( QString command )
{
    //kDebug() << getName() << "command part of command:" << getCommandPartOfCommand( command );
    
    if( getCommandPartOfCommand( command ) == NAME_COMMAND ){
        kDebug() << getName() << "name:" << getValuePartOfCommand( command );
        
        mName = getValuePartOfCommand( command );
        
        emit signalNewPlayer( this );
    }
    
    if( getCommandPartOfCommand( command ) == SELECTED_CARD_ID_COMMAND ){
        kDebug() << getName() << "Selected card id:" << getValuePartOfCommand( command );
        
        bool ok;
        int ret = getValuePartOfCommand( command ).toInt( &ok );
        
        if( ok ){
            
            if( mCards.at( ret )->isSelectable() ){
                setSelectableAllCards( false );
                
                if( isTwentyButtonVisible() ){
                    setTwentyButtonVisible( false );
                }
                
                if( isFortyButtonVisible() ){
                    setFortyButtonVisible( false );
                }
                
                if( isSchnapsenButtonVisible() ){
                    setSchnapsenButtonVisible( false );
                }
                
                if( isCloseButtonVisible() ){
                    setCloseButtonVisible( false );
                }
                
                //emit signalSelectedCard( takeCard( ret ), ret );
                emit signalSelectedCardId( ret );
                
            }else{
                kDebug() << "ERROR!" << getName() << "selected" << ret << "card id, but this is not selectable!";
            }
            
        }else{
            kDebug() << "ERROR! Cannot convert selected card id value to int!";
        }
    }
    
    if( getCommandPartOfCommand( command ) == TWENTY_BUTTON_CLICKED_COMMAND ){
        kDebug() << getName() << "Twenty button clicked.";
        
        if( isTwentyButtonVisible() ){
            twentyButtonClicked();
            emit signalTwentyButtonClicked();
        }else{
            kDebug() << getName() << "ERROR! Twenty button clicked, but twenty button is not visible!";
        }
    }
    
    if( getCommandPartOfCommand( command ) == FORTY_BUTTON_CLICKED_COMMAND ){
        kDebug() << getName() << "Forty button clicked.";
        
        if( isFortyButtonVisible() ){
            fortyButtonClicked();
            emit signalFortyButtonClicked();
        }else{
            kDebug() << getName() << "ERROR! Forty button clicked, but forty button is not visible!";
        }
    }
    
    if( getCommandPartOfCommand( command ) == SCHNAPSEN_BUTTON_CLICKED_COMMAND ){
        kDebug() << getName() << "Schnapsen button clicked.";
        
        if( mSchnapsenButtonVisible ){
            setSchnapsenButtonVisible( false );
            emit signalSchnapsenButtonClicked();
        }else{
            kDebug() << getName() << "ERROR! Schnapsen button clicked, but schnapsen button is not visible!";
        }
        
    }
    
    if( getCommandPartOfCommand( command ) == CLOSE_BUTTON_CLICKED_COMMAND ){
        kDebug() << getName() << "Close button clicked.";
        
        if( isCloseButtonVisible() ){
            setCloseButtonVisible( false );
            emit signalCloseButtonClicked();
        }else{
            kDebug() << getName() << "ERROR! Clicked to close button, but this is not visible!";
        }
    
    }
    
    if( getCommandPartOfCommand( command ) == CHANGE_TRUMP_CARD_COMMAND ){
        kDebug() << getName() << "Change trump card.";
        
        emit signalChangeTrumpCard( this );
    }
    
    if( getCommandPartOfCommand( command ) == START_NEXT_ROUND_COMMAND ){
        kDebug() << getName() << "Start next round.";
        
        emit signalStartNextRound( this );
    }
    
    if( getCommandPartOfCommand( command ) == START_NEXT_GAME_COMMAND ){
        kDebug() << getName() << "Start next game.";
        
        emit signalStartNextGame( this );
    }
    
}

bool Player::setSelectableCardsWhatEqualSuit( Card::Suit cardSuit )
{
    bool haveSelectableCard = false;
    
    for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
        
        if( mCards[ i ] != 0 && mCards[ i ]->getSuit() == cardSuit ){
            mCards[ i ]->setSelectable( true );
            emit signalPlayerCardSelectableChanged( i, true );
            haveSelectableCard = true;
        }
    
    }
    
    return haveSelectableCard;
}

void Player::slotReadyRead()
{
    QString command = readLine();
    
    //Sometimes more commands in one command, then do many pieces
    for( int lastI = 0, i = 1; i < command.size(); ++i ){
        
        if( command.at( i ) == ':' ){
            newCommand( command.mid( lastI, i-lastI ) );
            lastI = i;
        }
        
        if( i == command.size()-1 ){
            newCommand( command.mid( lastI ) );
        }
        
    }
}

/*int Player::sendNewCard( Card *card )
{
    int id = addNewCard( card );
    
    sendCommand( NEW_PLAYER_CARD_COMMAND+QString::number( getCard( id )->getValue() ) );
    
    return id;
}*/

void Player::sendNewCard( Card* card )
{
    addNewCard( card );
    
    sendCommand( NEW_PLAYER_CARD_COMMAND+QString::number( card->getValue() ) );
}


void Player::sendNewTrumpCard( Trump* trump )
{
    sendCommand( NEW_TRUMP_CARD_COMMAND+QString::number( trump->getCard()->getValue() ) );
}

void Player::sendSelectableAllCards()
{
    setSelectableAllCards( true );
    sendCommand( SELECTABLE_ALL_CARDS_COMMAND );
}

void Player::sendSelectableCertainCards( CentralCards *centralCards, Trump* trump )
{
    setSelectableCertainCards( centralCards, trump );
    sendCommand( SELECTABLE_CERTAIN_CARDS_COMMAND );
}

/*void Player::sendOpponentAddNewCentralCard( const Card *card )
{
    kDebug() << getName() << "Opponent add new central card.";
    sendCommand( OPPONENT_ADD_NEW_CENTRAL_CARD_COMMAND+QString::number( card->getValue() ) );
}*/

void Player::sendOpponentSelectedCard( int id , const Card* card )
{
    kDebug() << getName() << id << card->getCardText();
    
    sendCommand( OPPONENT_SELECTED_CARD_COMMAND + QString::number( id ) + "," + QString::number( card->getValue() ) );
}

void Player::addTricks( int tricks )
{
    setTricks( getTricks() + tricks );
    sendCommand( PLAYER_TRICKS_CHANGED_COMMAND+QString::number( getTricks() ) );
}

void Player::addScores( int scores )
{
    setScores( getScores() + scores );
    sendCommand( PLAYER_SCORES_CHANGED_COMMAND+QString::number( getScores() ) );
}

void Player::sendVisibleOpponentCards( int card1Pos, Card* card1, int card2Pos, Card* card2 )
{
    sendCommand( VISIBLE_OPPONENT_CARDS_COMMAND+QString::number( card1Pos )+","+QString::number( card1->getValue() )+","+
                 QString::number( card2Pos )+","+QString::number( card2->getValue() )
    );
}

void Player::sendCloseButtonVisible()
{
    setCloseButtonVisible( true );
    sendCommand( CLOSE_BUTTON_VISIBLE_COMMAND );
}
