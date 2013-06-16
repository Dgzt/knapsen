#include <KDE/KDebug>
#include "trump.h"
#include "player.h"

Player::Player( QString name ): 
    QTcpSocket(),
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
    
    while( !mCards.empty() ){
        delete mCards.takeLast();
    }
}

/*QString Player::getCommandPartOfCommand( QString text )
{
    if( text.indexOf('=') == -1 ){
        return text;
    }
    
    return text.mid( 0, text.indexOf( '=' )+1 ); //The '=' too.
}*/

QString Player::getCommandName( QString command )
{
    /*if( command.indexOf('=') == -1 ){
        return command;
    }*/
    if( !command.contains( '=' ) ){
        return command;
    }
    
    return command.mid( 0, command.indexOf( '=' )+1 ); //The '=' too.
}

/*QString Player::getValuePartOfCommand( QString text )
{
    return text.mid( text.indexOf( '=' )+1 );
}*/

QString Player::getCommandValue( QString command )
{
    return command.mid( command.indexOf( '=' )+1 );
}

void Player::sendInitializeTable( QString opponentName, Knapsen::TypeOfCards typeOfCards, int sizeOfDeck )
{
    QString typeOfCardsValue;
    if( typeOfCards == Knapsen::GermanSuits ){
        typeOfCardsValue = TYPE_OF_CARDS_GERMAN_SUITS_VALUE;
    }else{ // typeOfCards == Knapsen::FrenchSuits
        typeOfCardsValue = TYPE_OF_CARDS_FRENCH_SUITS_VALUE;
    }
    
    setLowestCard( sizeOfDeck );
    
    sendCommand( INITIALIZE_TABLE_COMMAND+opponentName+","+typeOfCardsValue+","+QString::number( sizeOfDeck ) );
}

int Player::changeTrumpCard( Trump *trump )
{
    int ret = -1; //If this function return this value, then this is a bug.
    
    for( int i = 0; i < mCards.size(); ++i ){
        if( mCards.at( i )->getSuit() == trump->getCardSuit() && mCards.at( i )->getType() == mLowestCardType ){
            kDebug() << i;
            
            Card* newTrumpCard = mCards.takeAt( i );
            Card* oldTrumpCard = trump->getCard();
            
            mCards.append( oldTrumpCard );
            trump->addNewCard( newTrumpCard );
            
            ret = i;
            break;
        }
    }
    
    return ret;
}

void Player::setSelectableAllCards( bool enabled )
{
    for( int i = 0; i < mCards.size(); ++i ){
        mCards.at(i)->setSelectable( enabled );
        emit signalPlayerCardSelectableChanged( i, enabled );
    }
    
    /*for( int i = mCards.size()-1; i >= 0; --i ){
        mCards.at(i)->setSelectable( enabled );
        emit signalPlayerCardSelectableChanged( i, enabled );
    }*/
}

/*void Player::setSelectableCertainCards( CentralCards *centralCards, Trump *trump )
{
    //Set true, which cards equal type whit central card
    if( setSelectableCardsWhatEqualSuit( centralCards->getCard( 0 )->getSuit() ) == false ){
        //If have not cards which equal type whith central card, then set true, which equal type whith trump card 
        if( setSelectableCardsWhatEqualSuit( trump->getCardSuit() ) == false ){
            //If have not that cards, then equal all card
            setSelectableAllCards( true );    
        }
    }
}*/

void Player::setSelectableCertainCards( const QList< Card* >& centralCards, const Trump *trump )
{
    //Set true, which cards equal type whit central card
    if( setSelectableCardsWhatEqualSuit( centralCards.first()->getSuit() ) == false ){
        //If have not cards which equal type whith central card, then set true, which equal type whith trump card 
        if( setSelectableCardsWhatEqualSuit( trump->getCardSuit() ) == false ){
            //If have not that cards, then equal all card
            setSelectableAllCards( true );    
        }
    }
}

void Player::setSelectableRegularMarriageCards( const Trump* trump )
{
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

void Player::setSelectableTrumpMarriageCards( const Trump* trump )
{
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

void Player::setLowestCard( int deckSize )
{
    if( deckSize == 20 ){
        mLowestCardType = Card::Jack;
    }else{
        mLowestCardType = Card::Nine;
    }
}

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

bool Player::haveRegularMarriage( const Trump* trump ) const
{
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

bool Player::haveTrumpMarriage( const Trump* trump ) const
{
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

bool Player::canChangeTrumpCard( const Trump* trump ) const
{
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
    
    if( getCommandName( command ) == NAME_COMMAND ){
        kDebug() << getName() << "name:" << getCommandValue( command );
        
        mName = getCommandValue( command );
        
        emit signalNewPlayer( this );
    }
    
    if( getCommandName( command ) == SELECTED_CARD_ID_COMMAND ){
        kDebug() << getName() << "Selected card id:" << getCommandValue( command );
        
        bool ok;
        int ret = getCommandValue( command ).toInt( &ok );
        
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
    
    if( getCommandName( command ) == TWENTY_BUTTON_CLICKED_COMMAND ){
        kDebug() << getName() << "Twenty button clicked.";
        
        if( isTwentyButtonVisible() ){
            twentyButtonClicked();
            emit signalTwentyButtonClicked();
        }else{
            kDebug() << getName() << "ERROR! Twenty button clicked, but twenty button is not visible!";
        }
    }
    
    if( getCommandName( command ) == FORTY_BUTTON_CLICKED_COMMAND ){
        kDebug() << getName() << "Forty button clicked.";
        
        if( isFortyButtonVisible() ){
            fortyButtonClicked();
            emit signalFortyButtonClicked();
        }else{
            kDebug() << getName() << "ERROR! Forty button clicked, but forty button is not visible!";
        }
    }
    
    if( getCommandName( command ) == SCHNAPSEN_BUTTON_CLICKED_COMMAND ){
        kDebug() << getName() << "Schnapsen button clicked.";
        
        if( mSchnapsenButtonVisible ){
            setSchnapsenButtonVisible( false );
            emit signalSchnapsenButtonClicked();
        }else{
            kDebug() << getName() << "ERROR! Schnapsen button clicked, but schnapsen button is not visible!";
        }
        
    }
    
    if( getCommandName( command ) == CLOSE_BUTTON_CLICKED_COMMAND ){
        kDebug() << getName() << "Close button clicked.";
        
        if( isCloseButtonVisible() ){
            setCloseButtonVisible( false );
            emit signalCloseButtonClicked();
        }else{
            kDebug() << getName() << "ERROR! Clicked to close button, but this is not visible!";
        }
    
    }
    
    if( getCommandName( command ) == CHANGE_TRUMP_CARD_COMMAND ){
        kDebug() << getName() << "Change trump card.";
        
        emit signalChangeTrumpCard( this );
    }
    
    if( getCommandName( command ) == START_NEXT_ROUND_COMMAND ){
        kDebug() << getName() << "Start next round.";
        
        emit signalStartNextRound( this );
    }
    
    if( getCommandName( command ) == START_NEXT_GAME_COMMAND ){
        kDebug() << getName() << "Start next game.";
        
        emit signalStartNextGame( this );
    }
    
}

bool Player::setSelectableCardsWhatEqualSuit( Card::Suit cardSuit )
{
    bool haveSelectableCard = false;
    
    for( int i = 0; i < getNumberOfCards(); ++i ){
        
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

void Player::sendNewCard( Card* card )
{
    addNewCard( card );
    
    sendCommand( NEW_PLAYER_CARD_COMMAND+QString::number( card->getValue() ) );
}

void Player::sendNewCardTrumpCard( Card* card )
{
    addNewCard( card );
    
    sendCommand( NEW_PLAYER_CARD_TRUMP_CARD_COMMAND );
}

void Player::sendNewTrumpCard( const Trump* trump )
{
    sendCommand( NEW_TRUMP_CARD_COMMAND+QString::number( trump->getCard()->getValue() ) );
}

void Player::sendOpponentChangeTrumpCard( int id, const Trump* trump )
{
    sendCommand( OPPONENT_CHANGE_TRUMP_CARD_COMMAND + QString::number( id ) + "," + QString::number( trump->getCard()->getValue() ) );
}

void Player::sendSelectableAllCards()
{
    setSelectableAllCards( true );
    sendCommand( SELECTABLE_ALL_CARDS_COMMAND );
}

/*void Player::sendSelectableCertainCards( CentralCards *centralCards, Trump* trump )
{
    setSelectableCertainCards( centralCards, trump );
    sendCommand( SELECTABLE_CERTAIN_CARDS_COMMAND );
}*/

void Player::sendSelectableCertainCards( const QList< Card* >& centralCards, const Trump* trump )
{
    setSelectableCertainCards( centralCards, trump );
    sendCommand( SELECTABLE_CERTAIN_CARDS_COMMAND );
}

void Player::sendOpponentSelectedCard( int id , const Card* card )
{
    kDebug() << getName() << id << card->getCardText();
    
    sendCommand( OPPONENT_SELECTED_CARD_COMMAND + QString::number( id ) + "," + QString::number( card->getValue() ) );
}

void Player::addTricks( int newTricks )
{
    setTricks( getTricks() + newTricks );
    sendCommand( PLAYER_TRICKS_CHANGED_COMMAND+QString::number( getTricks() ) );
}

void Player::addScores( int newScores )
{
    setScores( getScores() + newScores );
    sendCommand( PLAYER_SCORES_CHANGED_COMMAND+QString::number( getScores() ) );
}

void Player::sendShowOpponentCards( int card1Pos, Card* card1, int card2Pos, Card* card2 )
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
