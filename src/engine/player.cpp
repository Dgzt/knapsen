#include <KDE/KDebug>
#include "centralcards.h"
#include "trumpcard.h"
#include "player.h"

Player::Player( QObject* parent ): 
	QTcpSocket( parent ),
	mName( "" ),
	mLowestCardType( Card::Jack ),
	mNumberOfCardsInHand( 0 ),
	mTricks( 0 ),
	mScores( 0 ),
	mTwentyButtonVisible( false ),
	mFortyButtonVisible( false ),
	mCloseButtonVisible( false )
{	
	mCards = 0;
	
	connect( this, SIGNAL( readyRead() ), this, SLOT( slotReadyRead() ) );
	connect( this, SIGNAL( disconnected() ), this, SLOT( slotDisconnected() ) );
}

Player::~Player()
{
	kDebug() << getName() << "deleting.";
	
	if( mCards != 0 ){
		for( int i = 0; i < mNumberOfCardsInHand; ++i ){
			if( mCards[i] != 0 ){
				delete mCards[i];
			}
		}
		delete[] mCards;
	}
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

void Player::setNumberOfCardsInHand( int size )
{
	mNumberOfCardsInHand = size;
	
	mCards = new Card*[ mNumberOfCardsInHand ];
	
	for( int i = 0; i < size; ++i ){
		mCards[ i ] = 0;
	}
}

int Player::addNewCard( Card *card )
{
	for( int i = 0; i < mNumberOfCardsInHand; ++i ){
		if( mCards[ i ] == 0 ){
			mCards[ i ] = card;
			return i;
		}
	}
	return -1;
}

void Player::removeCard( int id )
{
	//Why not delete? Becouse when call the removeCard(int) function when move the card to central cards 
	mCards[ id ] = 0;  
}

int Player::changeTrumpCard( TrumpCard *trumpCard )
{
	//This value will be modify. Only for don't write warning the compiler.
	int ret = -1;
	
	for( int i = 0; i < mNumberOfCardsInHand; ++i ){
		if( mCards[i] != 0 && mCards[i]->getCardSuit() == trumpCard->getCardSuit() && mCards[i]->getCardType() == mLowestCardType ){
			kDebug() << i;
			
			Card *tmpCard = trumpCard->getCard();
			trumpCard->addNewCard( mCards[i] );
			mCards[i]=tmpCard;
			
			break;
		}
	}
	
	return ret;
}

void Player::setSelectableAllCards( bool enabled )
{
	for( int i = 0; i < mNumberOfCardsInHand; ++i ){
		if( mCards[ i ] != 0 ){
			mCards[ i ]->setSelectable( enabled );
			emit signalPlayerCardSelectableChanged( i, enabled );
		}
	}
}

void Player::setSelectableCertainCards( CentralCards *centralCards, TrumpCard *trumpCard )
{
	//Set true, which cards equal type whit central card
	//if( setSelectableCardsWhatEqualSuit( centralCards->getCard( 0 ).getCardSuit() ) == false ){
	if( setSelectableCardsWhatEqualSuit( centralCards->getCard( 0 )->getCardSuit() ) == false ){
		//If have not cards which equal type whith central card, then set true, which equal type whith trump card 
		//if( setSelectableCardsWhatEqualSuit( mTrumpCardSuit ) == false ){
		if( setSelectableCardsWhatEqualSuit( trumpCard->getCardSuit() ) == false ){
			//If have not that cards, then equal all card
			setSelectableAllCards( true );
			
		}
		
	}
}

void Player::setSelectableRegularMarriagesCards( TrumpCard *trumpCard )
{
	for( int i = 0; i < mNumberOfCardsInHand; ++i ){
		
		if( mCards[ i ] != 0 && 
			mCards[ i ]->getCardSuit() != trumpCard->getCardSuit() && 
			mCards[ i ]->getCardType() == Card::King
		){
			for( int j = 0; j < mNumberOfCardsInHand; ++j ){
				
				if( mCards[ j ] != 0 &&
					mCards[ j ]->getCardSuit() == mCards[ i ]->getCardSuit() &&
					mCards[ j ]->getCardType() == Card::Queen
					
				){
					mCards[ i ]->setSelectable( true );
					mCards[ j ]->setSelectable( true );
					emit signalPlayerCardSelectableChanged( i, mCards[ i ]->isSelectable() );
					emit signalPlayerCardSelectableChanged( j, mCards[ j ]->isSelectable() );
				}
				
			}
		}
		
	}
}

void Player::setSelectableTrumpMarriagesCards( TrumpCard *trumpCard )
{
	for( int i = 0; i < mNumberOfCardsInHand; ++i ){
		
		if( mCards[ i ] != 0 && 
			mCards[ i ]->getCardSuit() == trumpCard->getCardSuit() && 
			mCards[ i ]->getCardType() == Card::King
		){
			for( int j = 0; j < mNumberOfCardsInHand; ++j ){
				
				if( mCards[ j ] != 0 &&
					mCards[ j ]->getCardSuit() == mCards[ i ]->getCardSuit() &&
					mCards[ j ]->getCardType() == Card::Queen
					
				){
					mCards[ i ]->setSelectable( true );
					mCards[ j ]->setSelectable( true );
					emit signalPlayerCardSelectableChanged( i, mCards[ i ]->isSelectable() );
					emit signalPlayerCardSelectableChanged( j, mCards[ j ]->isSelectable() );
				}
				
			}
		}
		
	}
}

int Player::getNumberOfCardsInHandNow()
{
	int ret = 0;
	
	for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
		//if( getCard( i ).isValid() ){
		if( getCard( i ) != 0 ){
			++ret;
		}
	}
	
	return ret;
}

Card* Player::getCard( int id )
{
	return mCards[ id ];
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
	for( int i = 0; i < mNumberOfCardsInHand; ++i ){
		if( mCards[ i ] != 0 ){
			delete mCards[ i ];
			mCards[ i ] = 0;
		}
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

/*int Player::getPositionOfPairOfCard( Card card )
{
	for( int i = 0; i < mNumberOfCardsInHand; ++i ){
		if( mCards[i] != 0 && card.getCardSuit() == mCards[i]->getCardSuit() &&
			( ( card.getCardType() == Card::King && mCards[i]->getCardType() == Card::Queen ) ||
			  ( card.getCardType() == Card::Queen && mCards[i]->getCardType() == Card::King ) )
		){
			return i;
		}
	}
	
	//Error
	return -1;
}*/

int Player::getPositionOfPairOfCard( const Card* card )
{
	for( int i = 0; i < mNumberOfCardsInHand; ++i ){
		if( mCards[i] != 0 && card->getCardSuit() == mCards[i]->getCardSuit() &&
			( ( card->getCardType() == Card::King && mCards[i]->getCardType() == Card::Queen ) ||
			  ( card->getCardType() == Card::Queen && mCards[i]->getCardType() == Card::King ) )
		){
			return i;
		}
	}
	
	//Error
	return -1;
}

bool Player::haveRegularMarriages( TrumpCard *trumpCard ) const
{
	bool ret = false;
	
	for( int i = 0; i < mNumberOfCardsInHand; ++i ){
		if( mCards[ i ] != 0 && mCards[ i ]->getCardSuit() != trumpCard->getCardSuit() && mCards[ i ]->getCardType() == Card::King ){
			
			for( int j = 0; j < mNumberOfCardsInHand; ++j ){
				if( mCards[j] != 0 && mCards[j]->getCardType() == Card::Queen && mCards[j]->getCardSuit() == mCards[i]->getCardSuit() ){
					ret = true;
				}
			}
			
		}
	}
	
	return ret;
}

bool Player::haveTrumpMarriages( TrumpCard *trumpCard ) const
{
	bool ret = false;
	
	for( int i = 0; i < mNumberOfCardsInHand; ++i ){
		if( mCards[ i ] != 0 && mCards[ i ]->getCardSuit() == trumpCard->getCardSuit() && mCards[ i ]->getCardType() == Card::King ){
			
			for( int j = 0; j < mNumberOfCardsInHand; ++j ){
				if( mCards[j] != 0 && mCards[j]->getCardType() == Card::Queen && mCards[j]->getCardSuit() == mCards[i]->getCardSuit() ){
					ret = true;
				}
			}
			
		}
	}

	return ret;
}

bool Player::canChangeTrumpCard( TrumpCard *trumpCard ) const
{
	bool canChange = false;
	
	for( int i = 0; i < mNumberOfCardsInHand; ++i ){
		
		if( mCards[i] != 0 && mCards[i]->getCardSuit() == trumpCard->getCardSuit() && mCards[i]->getCardType() == mLowestCardType ){
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

void Player::twentyButtonClicked()
{
	setTwentyButtonVisible( false );
	
	if( mFortyButtonVisible ){
		setFortyButtonVisible( false );
	}
	
	setSelectableAllCards( false );
	//setSelectableRegularMarriagesCards();             //Ez meg nincs megoldva a trump card miatt
}

void Player::fortyButtonClicked()
{
	setFortyButtonVisible( false );
	
	if( mTwentyButtonVisible ){
		setTwentyButtonVisible( false );
	}
	
	setSelectableAllCards( false );
	//setSelectableTrumpMarriagesCards();              //Ez meg nincs megoldva a trump card miatt
}

void Player::newCommand( QString command )
{
	//kDebug() << getName() << "command part of command:" << getCommandPartOfCommand( command );
	
	if( getCommandPartOfCommand( command ) == NAME_COMMAND ){
		kDebug() << getName() << "name:" << getValuePartOfCommand( command );
		
		setName( getValuePartOfCommand( command ) );
		
		emit signalNewPlayer( this );
	}
	
	if( getCommandPartOfCommand( command ) == SELECTED_CARD_ID_COMMAND ){
		kDebug() << getName() << "Selected card id:" << getValuePartOfCommand( command );
		
		bool ok;
		int ret = getValuePartOfCommand( command ).toInt( &ok );
		
		if( ok ){
		
			if( mCards[ ret ]->isSelectable() ){
				setSelectableAllCards( false );
				
				if( isTwentyButtonVisible() ){
					setTwentyButtonVisible( false );
				}
				
				if( isFortyButtonVisible() ){
					setFortyButtonVisible( false );
				}
				
				if( isCloseButtonVisible() ){
					setCloseButtonVisible( false );
				}
				
				/*Card selectedCard = Card( getCard( ret )->getValue() );
				removeCard( ret );
				
				emit signalSelectedCard( selectedCard, ret );*/
				
				emit signalSelectedCard( getCard( ret ), ret );
				removeCard( ret );
			}else{
				kDebug() << "ERROR!" << getName() << "selected" << ret << "card id, but this is not selectable!";
			}
			
		}else{
			kDebug() << "ERROR! Cannot convert selected card id value to int!";
		}
	}
	
	if( getCommandPartOfCommand( command ) == TWENTY_BUTTON_CLICKED_COMMAND ){
		kDebug() << getName() << "Twenty button clicked.";
		
		if( mTwentyButtonVisible ){
			twentyButtonClicked();
			//mOpponent->OpponentTwentyButtonClicked();
			emit signalTwentyButtonClicked();
		}else{
			kDebug() << getName() << "ERROR! Twenty button clicked, but twenty button is not visible!";
		}
	}
	
	if( getCommandPartOfCommand( command ) == FORTY_BUTTON_CLICKED_COMMAND ){
		kDebug() << getName() << "Forty button clicked.";
		
		if( mFortyButtonVisible ){
			fortyButtonClicked();
			//mOpponent->sendOpponentFortyButtonClicked();
			emit signalFortyButtonClicked();
		}else{
			kDebug() << getName() << "ERROR! Forty button clicked, but forty button is not visible!";
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
		
		//changeTrumpCard();
		emit signalChangedTrumpCard();
		
		/*
		 * 
		 * 
		 * 
		 * 
		 * 
		 * 
		 * 
		 */
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

bool Player::setSelectableCardsWhatEqualSuit( Card::CardSuit cardSuit )
{
	bool haveSelectableCard = false;
	
	for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
		
		if( mCards[ i ] != 0 && mCards[ i ]->getCardSuit() == cardSuit ){
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
	
	//kDebug() << getName() << "input command:" << command;
	
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

void Player::sendTypeOfCard( Knapsen::TypeOfCards typeOfCards )
{
	if( typeOfCards == Knapsen::GermanSuits ){
		sendCommand( TYPE_OF_CARDS_COMMAND+TYPE_OF_CARDS_GERMAN_SUITS_VALUE );
	}else{ // typeOfCards == Knapsen::FrenchSuits
		sendCommand( TYPE_OF_CARDS_COMMAND+TYPE_OF_CARDS_FRENCH_SUITS_VALUE );
	}
}

void Player::sendNumberOfCardsInHand( int size )
{
	setNumberOfCardsInHand( size );
	sendCommand( NUMBER_OF_CARDS_IN_HAND_COMMAND+QString::number( size ) );
}

int Player::sendNewCard( Card *card )
{
	int id = addNewCard( card );
	
	sendCommand( NEW_PLAYER_CARD_COMMAND+QString::number( getCard( id )->getValue() ) );
	
	return id;
}

void Player::sendNewTrumpCard( TrumpCard *trumpCard )
{
	sendCommand( NEW_TRUMP_CARD_COMMAND+QString::number( trumpCard->getCard()->getValue() ) );
}

void Player::sendSelectableAllCards()
{
	setSelectableAllCards( true );
	sendCommand( SELECTABLE_ALL_CARDS_COMMAND );
}

void Player::sendSelectableCertainCards( CentralCards *centralCards, TrumpCard *trumpCard )
{
	setSelectableCertainCards( centralCards, trumpCard );
	sendCommand( SELECTABLE_CERTAIN_CARDS_COMMAND );
}


/*void Player::sendOpponentAddNewCentralCard( Card card )
{
	kDebug() << getName() << "Opponent add new central card.";
	sendCommand( OPPONENT_ADD_NEW_CENTRAL_CARD_COMMAND+QString::number( card.getValue() ) );
}*/

void Player::sendOpponentAddNewCentralCard( const Card *card )
{
	kDebug() << getName() << "Opponent add new central card.";
	sendCommand( OPPONENT_ADD_NEW_CENTRAL_CARD_COMMAND+QString::number( card->getValue() ) );
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

/*void Player::sendVisibleOpponentCards( int card1Pos, Card card1, int card2Pos, Card *card2 )
{
	sendCommand( VISIBLE_OPPONENT_CARDS_COMMAND+QString::number( card1Pos )+","+QString::number( card1.getValue() )+","+
												QString::number( card2Pos )+","+QString::number( card2->getValue() )
	);
}*/

void Player::sendVisibleOpponentCards( int card1Pos, Card* card1, int card2Pos, Card* card2 )
{
	sendCommand( VISIBLE_OPPONENT_CARDS_COMMAND+QString::number( card1Pos )+","+QString::number( card1->getValue() )+","+
												QString::number( card2Pos )+","+QString::number( card2->getValue() )
	);
}

void Player::sendTwentyButtonVisible()
{
	setTwentyButtonVisible( true );
	sendCommand( TWENTY_BUTTON_VISIBLE_COMMAND );
}

void Player::sendFortyButtonVisible()
{
	setFortyButtonVisible( true );
	sendCommand( FORTY_BUTTON_VISIBLE_COMMAND );
}

void Player::sendCloseButtonVisible()
{
	setCloseButtonVisible( true );
	sendCommand( CLOSE_BUTTON_VISIBLE_COMMAND );
}
