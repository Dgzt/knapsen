#include <KDE/KDebug>
#include "client.h"

Client::Client( QObject* parent ) : 
	Player( parent ),
	mOpponentName( "" ),
	mTypeOfCards( Knapsen::GermanSuits )
{
	mSizeOfDeck = 0;
	
	connect( this, SIGNAL( connected() ), this, SLOT( slotConnected() ) );
	connect( this, SIGNAL( signalCentralCardChanged( int, Card ) ), this, SLOT( slotCentralCardChanged( int, Card ) ) );
}

void Client::newCommand( QString command )
{
    if( getCommandPartOfCommand( command ) == OPPONENT_NAME_COMMAND ){
		kDebug() << getName() << "opponent name:" << getValuePartOfCommand( command );
		mOpponentName = getValuePartOfCommand( command );
	}
	
	if( getCommandPartOfCommand( command ) == TYPE_OF_CARDS_COMMAND ){
		kDebug() << getName() << "Type of cards:" << getValuePartOfCommand( command );
		
		if( getValuePartOfCommand( command ) == TYPE_OF_CARDS_GERMAN_SUITS_VALUE ){
			mTypeOfCards = Knapsen::GermanSuits;
		}else{ //getValuePartOfCommand( command ) == TYPE_OF_CARDS_FRENCH_SUITS_VALUE
			mTypeOfCards = Knapsen::FrenchSuits;
		}
	}
	
	if( getCommandPartOfCommand( command ) == SIZE_OF_DECK_COMMAND ){
		kDebug() << getName() << "size of deck:" << getValuePartOfCommand( command );
		
		bool ok;
		int ret = getValuePartOfCommand( command ).toInt( &ok );
		
		if( ok ){
			mSizeOfDeck = ret;
		}else{
			kDebug() << "ERROR! Cannot convert size of deck value to int!";
		}
	}
	
	if( getCommandPartOfCommand( command ) == NUMBER_OF_CARDS_IN_HAND_COMMAND ){
		kDebug() << getName() << "Number of cards in hand:" << getValuePartOfCommand( command );
		
		bool ok;
		int ret = getValuePartOfCommand( command ).toInt( &ok );
		
		if( ok ){
			setNumberOfCardsInHand( ret );
		}else{
			kDebug() << "ERROR! Cannot convert number of cards in hand value to int!";
		}
	}
	
	if( getCommandPartOfCommand( command ) == INITIALIZE_TABLE_COMMAND ){
		kDebug() << getName() << "Initialize table.";
		
		emit signalInitialize( getName(), mOpponentName, mTypeOfCards, getNumberOfCardsInHand() );
	}
	
	if( getCommandPartOfCommand( command ) == NEW_PLAYER_CARD_COMMAND ){
		kDebug() << getName() << "new card:" << getValuePartOfCommand( command );

		bool ok;
		int ret = getValuePartOfCommand( command ).toInt( &ok );
		if( ok ){
			int cardId = addNewCard( Card( ret) );
			emit signalNewPlayerCard( cardId, Card( ret ).getCardText( mTypeOfCards ) );
			
			mSizeOfDeck--;
			if( mSizeOfDeck == 0 ){
				emit signalDeckVisible( false );
			}
		}else{
			kDebug() << "ERROR! Cannot convert new player card value to int!";
		}
	}
	
	if( getCommandPartOfCommand( command ) == NEW_OPPONENT_CARD_COMMAND_ID ){
		kDebug() << getName() << "new opponent card id:" << getValuePartOfCommand( command );
		
		bool ok;
		int ret = getValuePartOfCommand( command ).toInt( &ok );
		if( ok ){
			emit signalNewOpponentCardId( ret );
			
			mSizeOfDeck--;
			if( mSizeOfDeck == 0 ){
				emit signalDeckVisible( false );
			}
		}else{
			kDebug() << "ERROR! Cannot covert new opponent card command id value to int!";
		}
	}
	
	if( getCommandPartOfCommand( command ) == NEW_TRUMP_CARD_COMMAND ){
		kDebug() << getName() << "Trump card:" << getValuePartOfCommand( command );
		
		bool ok;
		int ret = getValuePartOfCommand( command ).toInt( &ok );
		if( ok ){
			setTrumpCard( Card( ret ) );
			emit signalNewTrumpCard( getTrumpCard().getCardText( mTypeOfCards ) );
		}else{
			kDebug() << "ERROR! Cannot convert new trump card command value to int!";
		}
	}
	
	if( getCommandPartOfCommand( command ) == TWENTY_BUTTON_VISIBLE_COMMAND ){
		kDebug() << getName() << "Twenty button visible.";
		
		setTwentyButtonVisible( true );
	}
	
	if( getCommandPartOfCommand( command ) == FORTY_BUTTON_VISIBLE_COMMAND ){
		kDebug() << getName() << "Forty button visible.";
		
		setFortyButtonVisible( true );
	}
	
	if( getCommandPartOfCommand( command ) == CLEAR_TRUMP_CARD_COMMAND ){
		kDebug() << getName() << "Clear trump card.";
		
		clearTrumpCard();
		emit signalTrumpCardHide();
	}
	
	if( getCommandPartOfCommand( command ) == SELECTABLE_ALL_CARDS_COMMAND ){
		kDebug() << getName() << "Selectable all cards.";
		
		setSelectableAllCards( true );
		
		//Emit for bot, it in action
		emit signalInAction();
	}
	
	if( getCommandPartOfCommand( command ) == SELECTABLE_CERTAIN_CARDS_COMMAND ){
		kDebug() << getName() << "Selectable certan cards.";
		
		setSelectableCertainCards();
		emit signalInAction();
	}
	
	if( getCommandPartOfCommand( command ) == OPPONENT_SELECTED_CARD_ID_COMMAND ){
		kDebug() << getName() << "Opponent selected card id:" << getValuePartOfCommand( command );
		
		bool ok;
		int ret = getValuePartOfCommand( command ).toInt( &ok );
		if( ok ){
			emit signalOpponentSelectedCardId( ret );
		}else{
			kDebug() << "ERROR! Cannot convert opponent selected card id command value to int!";
		}
	}
	
	if( getCommandPartOfCommand( command ) == OPPONENT_DISCONNECTED_COMMAND ){
		kDebug() << getName() << "Opponent disconnected!";
		
		emit signalOpponentDisconnected();
	}
	
	if( getCommandPartOfCommand( command ) == OPPONENT_ADD_NEW_CENTRAL_CARD_COMMAND ){
		kDebug() << getName() << "Opponent add new central card.";
		
		bool ok;
		int ret = getValuePartOfCommand( command ).toInt( &ok );
		if( ok ){
			addNewCentralCard( Card( ret ) );
		}
	}

	if( getCommandPartOfCommand( command ) == VISIBLE_OPPONENT_CARDS_COMMAND ){
		kDebug() << getName() << "Visible opponent cards:" << getValuePartOfCommand( command );
	}
	
	if( getCommandPartOfCommand( command ) == PLAYER_TRICKS_CHANGED_COMMAND ){
		kDebug() << getName() << "Player tricks changed:" << getValuePartOfCommand( command );
		
		bool ok;
		int ret = getValuePartOfCommand( command ).toInt( &ok );
		if( ok ){
			setTricks( ret );
			emit signalPlayerTricksChanged( getTricks() );
		}else{
			kDebug() << "ERROR! Cannot convert player tricks changed command value to int!";
		}
	}
	
	if( getCommandPartOfCommand( command ) == OPPONENT_TRICKS_CHANGED_COMMAND ){
		kDebug() << getName() << "Opponent tricks changed:" << getValuePartOfCommand( command );
		
		bool ok;
		int ret = getValuePartOfCommand( command ).toInt( &ok );
		if( ok ){
			emit signalOpponentTricksChanged( ret );
		}else{
			kDebug() << "ERROR! Cannot convert opponent tricks changed command value to int!";
		}
	}
	
	if( getCommandPartOfCommand( command ) == CLEAR_CENTRAL_CARDS_COMMAND ){
		kDebug() << getName() << "Clear central cards command.";
		clearCentralCards();
	}
	
	if( getCommandPartOfCommand( command ) == START_GAME_COMMAND ){
		kDebug() << getName() << "Start game.";
		emit signalStartGame();
	}
	
}

void Client::slotConnected()
{
	kDebug() << getName() << "connected.";
	
	sendCommand( NAME_COMMAND+getName() );
}

//void Client::slotNewCentralCard( int id, Card card )
void Client::slotCentralCardChanged( int id, Card card )
{
	emit signalCentralCardChanged( id, card.getCardText( mTypeOfCards ) );
}

void Client::slotSelectedCardId( int id )
{
	kDebug() << "Selected card:" << id;
	setSelectableAllCards( false );
	addNewCentralCard( getCard( id ) );
	removeCard( id );
	sendCommand( SELECTED_CARD_ID_COMMAND+QString::number( id ) );
}

void Client::slotTwentyButtonClicked()
{
	twentyButtonClicked();
	sendCommand( TWENTY_BUTTON_CLICKED_COMMAND );
}

void Client::slotFortyButtonClicked()
{
	kDebug();
	fortyButtonClicked();
	sendCommand( FORTY_BUTTON_CLICKED_COMMAND );
}
