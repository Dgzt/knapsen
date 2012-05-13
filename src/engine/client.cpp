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
	if( getCommandPartOfCommand( command ) == OPPONENT_DISCONNECTED_COMMAND ){
		kDebug() << getName() << "Opponent disconnected!";
	
		emit signalOpponentDisconnected();
	}
	
	if( getCommandPartOfCommand( command ) == COMMANDS_END_COMMAND ){
		
		slotProcessCommands();
	}
	
	commandList.append( command );
	
}

void Client::slotProcessCommands()
{
	do{
		if( getCommandPartOfCommand( commandList.first() ) == OPPONENT_NAME_COMMAND ){
			kDebug() << getName() << "opponent name:" << getValuePartOfCommand( commandList.first() );
			mOpponentName = getValuePartOfCommand( commandList.first() );
		}
	
		if( getCommandPartOfCommand( commandList.first() ) == TYPE_OF_CARDS_COMMAND ){
			kDebug() << getName() << "Type of cards:" << getValuePartOfCommand( commandList.first() );
		
			if( getValuePartOfCommand( commandList.first() ) == TYPE_OF_CARDS_GERMAN_SUITS_VALUE ){
				mTypeOfCards = Knapsen::GermanSuits;
			}else{ //getValuePartOfCommand( command ) == TYPE_OF_CARDS_FRENCH_SUITS_VALUE
				mTypeOfCards = Knapsen::FrenchSuits;
			}
		}
		
		if( getCommandPartOfCommand( commandList.first() ) == SIZE_OF_DECK_COMMAND ){
			kDebug() << getName() << "size of deck:" << getValuePartOfCommand( commandList.first() );
		
			bool ok;
			int ret = getValuePartOfCommand( commandList.first() ).toInt( &ok );
		
			if( ok ){
				mSizeOfDeck = ret;
			}else{
				kDebug() << "ERROR! Cannot convert size of deck value to int!";
			}
		}
		
		if( getCommandPartOfCommand( commandList.first() ) == NUMBER_OF_CARDS_IN_HAND_COMMAND ){
			kDebug() << getName() << "Number of cards in hand:" << getValuePartOfCommand( commandList.first() );
		
			bool ok;
			int ret = getValuePartOfCommand( commandList.first() ).toInt( &ok );
		
			if( ok ){
				setNumberOfCardsInHand( ret );
			}else{
				kDebug() << "ERROR! Cannot convert number of cards in hand value to int!";
			}
		}
		
		if( getCommandPartOfCommand( commandList.first() ) == INITIALIZE_TABLE_COMMAND ){
			kDebug() << getName() << "Initialize table.";
		
			emit signalInitialize( getName(), mOpponentName, mTypeOfCards, getNumberOfCardsInHand() );
		}
		
		if( getCommandPartOfCommand( commandList.first() ) == NEW_PLAYER_CARD_COMMAND ){
			kDebug() << getName() << "new card:" << getValuePartOfCommand( commandList.first() );

			bool ok;
			int ret = getValuePartOfCommand( commandList.first() ).toInt( &ok );
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
		
		if( getCommandPartOfCommand( commandList.first() ) == NEW_ROUND_COMMAND ){
			kDebug() << getName() << "New round.";
			newRound();
			emit signalNewRound();
		}
			
		if( getCommandPartOfCommand( commandList.first() ) == NEW_OPPONENT_CARD_COMMAND_ID ){
			kDebug() << getName() << "new opponent card id:" << getValuePartOfCommand( commandList.first() );
				
			bool ok;
			int ret = getValuePartOfCommand( commandList.first() ).toInt( &ok );
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
			
		if( getCommandPartOfCommand( commandList.first() ) == NEW_TRUMP_CARD_COMMAND ){
			kDebug() << getName() << "Trump card:" << getValuePartOfCommand( commandList.first() );
				
			bool ok;
			int ret = getValuePartOfCommand( commandList.first() ).toInt( &ok );
			if( ok ){
				setTrumpCard( Card( ret ) );
				emit signalNewTrumpCard( getTrumpCard().getCardText( mTypeOfCards ) );
			}else{
				kDebug() << "ERROR! Cannot convert new trump card command value to int!";
			}
		}
			
		if( getCommandPartOfCommand( commandList.first() ) == TWENTY_BUTTON_VISIBLE_COMMAND ){
			kDebug() << getName() << "Twenty button visible.";
			
			setTwentyButtonVisible( true );
		}
			
		if( getCommandPartOfCommand( commandList.first() ) == FORTY_BUTTON_VISIBLE_COMMAND ){
			kDebug() << getName() << "Forty button visible.";
			
			setFortyButtonVisible( true );
		}
		
		if( getCommandPartOfCommand( commandList.first() ) == CLOSE_BUTTON_VISIBLE_COMMAND ){
			kDebug() << getName() << "Close button visible.";
			
			setCloseButtonVisible( true );
		}
			
		if( getCommandPartOfCommand( commandList.first() ) == CLEAR_TRUMP_CARD_COMMAND ){
			kDebug() << getName() << "Clear trump card.";
			
			clearTrumpCard();
			emit signalTrumpCardHide();
		}
		
		if( getCommandPartOfCommand( commandList.first() ) == TRUMP_CARD_SELECTABLE_COMMAND ){
			kDebug() << getName() << "Selectable trump card.";
			getTrumpCard().setSelectable( true );
			emit signalTrumpCardSelectableChanged( getTrumpCard().isSelectable() );
		}
			
		if( getCommandPartOfCommand( commandList.first() ) == SELECTABLE_ALL_CARDS_COMMAND ){
			kDebug() << getName() << "Selectable all cards.";
			
			setSelectableAllCards( true );
			
			//Emit for bot, it in action
			emit signalInAction();
		}
			
		if( getCommandPartOfCommand( commandList.first() ) == SELECTABLE_CERTAIN_CARDS_COMMAND ){
			kDebug() << getName() << "Selectable certan cards.";
			
			setSelectableCertainCards();
			emit signalInAction();
		}
			
		if( getCommandPartOfCommand( commandList.first() ) == OPPONENT_SELECTED_CARD_ID_COMMAND ){
			kDebug() << getName() << "Opponent selected card id:" << getValuePartOfCommand( commandList.first() );
			
			bool ok;
			int ret = getValuePartOfCommand( commandList.first() ).toInt( &ok );
			if( ok ){
				emit signalOpponentSelectedCardId( ret );
			}else{
				kDebug() << "ERROR! Cannot convert opponent selected card id command value to int!";
			}
		}
			
		if( getCommandPartOfCommand( commandList.first() ) == OPPONENT_ADD_NEW_CENTRAL_CARD_COMMAND ){
			kDebug() << getName() << "Opponent add new central card.";
			
			bool ok;
			int ret = getValuePartOfCommand( commandList.first() ).toInt( &ok );
			if( ok ){
				addNewCentralCard( Card( ret ) );
			}
		}

		if( getCommandPartOfCommand( commandList.first() ) == VISIBLE_OPPONENT_CARDS_COMMAND ){
			kDebug() << getName() << "Visible opponent cards:" << getValuePartOfCommand( commandList.first() );
			
			QString value( getValuePartOfCommand( commandList.first() ) );
			
			bool ok;

			int temp = value.indexOf( ',' );
			int card1Pos = value.mid( 0, temp ).toInt( &ok );
				
			value = value.right( value.size()-temp-1 );
			temp = value.indexOf( ',' );
			int card1Value = value.mid( 0, temp ).toInt( &ok );
			
			value = value.right( value.size()-temp-1 );
			temp = value.indexOf( ',' );
			int card2Pos = value.mid( 0, temp ).toInt( &ok );
			
			value = value.right( value.size()-temp-1 );
			int card2Value = value.toInt( &ok );
			
		
			if( ok ){
				kDebug() << card1Pos;
				kDebug() << card1Value;
				kDebug() << card2Pos;
				kDebug() << card2Value;
				
				emit signalShowOpponentCards( card1Pos, Card(card1Value).getCardText( mTypeOfCards ), card2Pos, Card(card2Value).getCardText( mTypeOfCards ) );
			}else{
				kDebug() << "ERROR! Wrong value in visible cards command!";
			}
			
		}
			
		if( getCommandPartOfCommand( commandList.first() ) == PLAYER_TRICKS_CHANGED_COMMAND ){
			kDebug() << getName() << "Player tricks changed:" << getValuePartOfCommand( commandList.first() );
			
			bool ok;
			int ret = getValuePartOfCommand( commandList.first() ).toInt( &ok );
			if( ok ){
				setTricks( ret );
				emit signalPlayerTricksChanged( getTricks() );
			}else{
				kDebug() << "ERROR! Cannot convert player tricks changed command value to int!";
			}
		}
		
		if( getCommandPartOfCommand( commandList.first() ) == PLAYER_SCORES_CHANGED_COMMAND ){
			kDebug() << getName() << "Player scores changed:" << getValuePartOfCommand( commandList.first() );
			
			bool ok;
			int ret = getValuePartOfCommand( commandList.first() ).toInt( &ok );
			if( ok ){
				setScores( ret );
				emit signalPlayerScoresChanged( ret );
			}else{
				kDebug() << "ERROR! Cannot convert player scores changed command value to int!";
			}
		}
			
		if( getCommandPartOfCommand( commandList.first() ) == OPPONENT_TRICKS_CHANGED_COMMAND ){
			kDebug() << getName() << "Opponent tricks changed:" << getValuePartOfCommand( commandList.first() );
			
			bool ok;
			int ret = getValuePartOfCommand( commandList.first() ).toInt( &ok );
			if( ok ){
				emit signalOpponentTricksChanged( ret );
			}else{
				kDebug() << "ERROR! Cannot convert opponent tricks changed command value to int!";
			}
		}
		
		if( getCommandPartOfCommand( commandList.first() ) == OPPONENT_SCORES_CHANGED_COMMAND ){
			kDebug() << getName() << "Opponent scores changed:" << getValuePartOfCommand( commandList.first() );
			
			bool ok;
			int ret = getValuePartOfCommand( commandList.first() ).toInt( &ok );
			if( ok ){
				emit signalOpponentScoresChanged( ret );
			}else{
				kDebug() << "ERROR! Cannot convert opponent scores changed command value to int!";
			}
		}
		
		if( getCommandPartOfCommand( commandList.first() ) == OPPONENT_CLICKED_TO_CLOSE_BUTTON_COMMAND ){
			kDebug() << getName() << "Opponent clicked to close button.";
			emit signalCloseDeck();
		}
			
		if( getCommandPartOfCommand( commandList.first() ) == CLEAR_CENTRAL_CARDS_COMMAND ){
			kDebug() << getName() << "Clear central cards command.";
			clearCentralCards();
		}
		
		if( getCommandPartOfCommand( commandList.first() ) == START_GAME_COMMAND ){
			kDebug() << getName() << "Start game.";
			emit signalStartGame();
		}
		
		if( getCommandPartOfCommand( commandList.first() ) == END_ROUND_COMMAND ){
			kDebug() << getName() << "End round.";
			
			QString value = getValuePartOfCommand( commandList.first() );
			
			QString winnerName = value.mid( 0, value.indexOf( ',' ) );
			
			bool ok;
			int winnerScore = value.mid( value.indexOf( ',' )+1 ).toInt( &ok );
			if( ok ){
				emit signalEndRound( winnerName, winnerScore );
			}else{
				kDebug() << "ERROR! Cannot convert winner scores command value to int!";
			}
			
		}
		
		
		
		if( getName() != "Bot" && getCommandPartOfCommand( commandList.first() ) == VISIBLE_OPPONENT_CARDS_COMMAND ){
			kDebug() << "Break loop.";
			commandList.removeFirst();
			break;
		}
		
		commandList.removeFirst();
			
	}while( commandList.size() != 0 );
}

void Client::slotConnected()
{
	kDebug() << getName() << "connected.";
	
	sendCommand( NAME_COMMAND+getName() );
}

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
	
	if( isTwentyButtonVisible() ){
		setTwentyButtonVisible( false );
	}
	
	if( isFortyButtonVisible() ){
		setFortyButtonVisible( false );
	}
	
	if( isCloseButtonVisible() ){
		setCloseButtonVisible( false );
	}
	
	if( getTrumpCard().isSelectable() ){
		getTrumpCard().setSelectable( false );
		emit signalTrumpCardSelectableChanged( false );
	}
	
	sendCommand( SELECTED_CARD_ID_COMMAND+QString::number( id ) );
}

void Client::slotSelectedTrumpCard()
{
	kDebug() << "Selected trump card.";
	
	int ret = changeTrumpCard();
	emit signalNewPlayerCard( ret, getCard(ret).getCardText( mTypeOfCards ) );
	emit signalNewTrumpCard( getTrumpCard().getCardText( mTypeOfCards ) );
	emit signalTrumpCardSelectableChanged( false );
	
	sendChangeTrumpCard();
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

void Client::slotCloseButtonClicked()
{
	setCloseButtonVisible( false );
	emit signalCloseDeck();
	
	kDebug() << getTrumpCard().isSelectable();
	if( getTrumpCard().isSelectable() ){
		getTrumpCard().setSelectable( false );
		emit signalTrumpCardSelectableChanged( getTrumpCard().isSelectable() );
	}
	
	sendCommand( CLOSE_BUTTON_CLICKED_COMMAND );
}
