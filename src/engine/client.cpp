#include <KDE/KDebug>
#include "centralcards.h"
#include "trumpcard.h"
#include "client.h"

Client::Client( QString name ): 
	Player( name ),
	mSizeOfDeck( 0 ),
	mSizeOfDeckNow( 0 )
{
	mCentralCards = new CentralCards;
	mTrumpCard = new TrumpCard;
	
	connect( this, SIGNAL( connected() ), this, SLOT( slotConnected() ) );
}

Client::~Client()
{
	/*if( mCentralCards ){
		delete mCentralCards;
		mCentralCards = 0;
	}*/
	delete mCentralCards;
	delete mTrumpCard;
}

QList< QString >* Client::getValues( QString valuesStr )
{
	QList< QString >* valuesArray = new QList< QString >;
	
	int last = 0;
	for( int i = 0; i < valuesStr.length(); ++i ){
		if( valuesStr[i] == ',' ){
			valuesArray->append( valuesStr.mid( last, i-last ) );
			last = i+1;
		}
	}
	valuesArray->append( valuesStr.mid( last, valuesStr.length()-last ) );
	
	return valuesArray;
}

void Client::newCommand( QString command )
{
	kDebug() << command;
	
	if( getCommandPartOfCommand( command ) == OPPONENT_DISCONNECTED_COMMAND ){
		kDebug() << getName() << "Opponent disconnected!";
	
		//emit signalOpponentDisconnected();
		emit signalGameError( Client::OpponentDisconnected );
		
		return;
	}
	
	if( getCommandPartOfCommand( command ) == NAME_IS_BUSY_COMMAND ){
		kDebug() << getName() << "Name is busy command.";
	
		emit signalGameError( Client::NameIsBusy );
		
		return;
	}
	
	if( getCommandPartOfCommand( command ) == SERVER_IS_FULL_COMMAND ){
		kDebug() << getName() << "Server is full.";
		
		emit signalGameError( Client::ServerIsFull );
		
		return;
	}
	
	if( getCommandPartOfCommand( command ) == COMMANDS_END_COMMAND ){
		
		slotProcessCommands();
	}
	
	commandList.append( command );
	
}

void Client::slotProcessCommands()
{
	do{
		if( getCommandPartOfCommand( commandList.first() ) == INITIALIZE_TABLE_COMMAND ){
			kDebug() << getName() << "Initialize table, value." << getValuePartOfCommand( commandList.first() );
			
			QList< QString >* valuesArray = getValues( getValuePartOfCommand( commandList.first() ) );
			
			Knapsen::TypeOfCards typeOfCards;
			if( valuesArray->at( 1 ) == TYPE_OF_CARDS_GERMAN_SUITS_VALUE ){
				typeOfCards = Knapsen::GermanSuits;
			}else{ //valuesArray->at( 1 ) == TYPE_OF_CARDS_FRENCH_SUITS_VALUE
				typeOfCards = Knapsen::FrenchSuits;
			}
			
			bool ok;
			mSizeOfDeck = valuesArray->at( 2 ).toInt( &ok );
			mSizeOfDeckNow = mSizeOfDeck;
			int numberOfCardsInHand = valuesArray->at( 3 ).toInt( &ok );
			
			setLowestCard( mSizeOfDeck );
			
			setNumberOfCardsInHand( numberOfCardsInHand );
			
			emit signalInitialize( getName(), valuesArray->at( 0 ), typeOfCards, getNumberOfCardsInHand() );
			
			delete valuesArray;
		}
		
		if( getCommandPartOfCommand( commandList.first() ) == NEW_PLAYER_CARD_COMMAND ){
			kDebug() << getName() << "new card:" << getValuePartOfCommand( commandList.first() );

			bool ok;
			int ret = getValuePartOfCommand( commandList.first() ).toInt( &ok );
			if( ok ){
				//int cardId = addNewCard( Card( ret) );
				int cardId = addNewCard( new Card( ret ) );
				//emit signalNewPlayerCard( cardId, Card( ret ).getCardText() );
				emit signalNewPlayerCard( cardId, getCard( cardId )->getCardText() );
				
				
				mSizeOfDeckNow--;
				if( mSizeOfDeckNow == 0 ){
					//emit signalDeckVisible( false );
					emit signalHideDeck();
				}
			}else{
				kDebug() << "ERROR! Cannot convert new player card value to int!";
			}
		}
		
		if( getCommandPartOfCommand( commandList.first() ) == NEW_ROUND_COMMAND ){
			kDebug() << getName() << "New round.";
			newRound();
			
			mCentralCards->clear();
			emit signalClearCentralCards();
			
			mSizeOfDeckNow = mSizeOfDeck;
			emit signalNewRound();
		}
			
		if( getCommandPartOfCommand( commandList.first() ) == NEW_OPPONENT_CARD_COMMAND_ID ){
			kDebug() << getName() << "new opponent card id:" << getValuePartOfCommand( commandList.first() );
				
			bool ok;
			int ret = getValuePartOfCommand( commandList.first() ).toInt( &ok );
			if( ok ){
				emit signalNewOpponentCardId( ret );
					
				mSizeOfDeckNow--;
				if( mSizeOfDeckNow == 0 ){
					//emit signalDeckVisible( false );
					emit signalHideDeck();
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
				if( !mTrumpCard->isEmpty() ){
					mTrumpCard->clearTrumpCard( true );
				}
				
				mTrumpCard->addNewCard( new Card( ret ) );
				
				emit signalNewTrumpCard( getTrumpCard()->getCard()->getCardText() );
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
			
			mTrumpCard->clearTrumpCard( true );
			emit signalTrumpCardHide();
		}
		
		if( getCommandPartOfCommand( commandList.first() ) == TRUMP_CARD_SELECTABLE_COMMAND ){
			kDebug() << getName() << "Selectable trump card.";
			
			getTrumpCard()->getCard()->setSelectable( true );
			emit signalTrumpCardSelectableChanged( getTrumpCard()->getCard()->isSelectable() );
		}
			
		if( getCommandPartOfCommand( commandList.first() ) == SELECTABLE_ALL_CARDS_COMMAND ){
			kDebug() << getName() << "Selectable all cards.";
			
			setSelectableAllCards( true );
			
			//Emit for bot, it in action
			emit signalPlayerInAction();
		}
			
		if( getCommandPartOfCommand( commandList.first() ) == SELECTABLE_CERTAIN_CARDS_COMMAND ){
			kDebug() << getName() << "Selectable certan cards.";
			
			//setSelectableCertainCards();
			setSelectableCertainCards( mCentralCards , mTrumpCard );
			emit signalPlayerInAction();
		}
		
		if( getCommandPartOfCommand( commandList.first() ) == OPPONENT_IN_ACTION_COMMAND ){
			kDebug() << getName() << "Opponent in action.";
			
			emit signalOpponentInAction();
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
				//addNewCentralCard( Card( ret ) );
				//int pos = mCentralCards->add( Card( ret ) );
				int pos = mCentralCards->add( new Card( ret ) );
				//emit signalNewCentralCard( pos, mCentralCards->getCard( pos ).getCardText() );
				emit signalNewCentralCard( pos, mCentralCards->getCard( pos )->getCardText() );
			}
		}

		if( getCommandPartOfCommand( commandList.first() ) == VISIBLE_OPPONENT_CARDS_COMMAND ){
			kDebug() << getName() << "Visible opponent cards:" << getValuePartOfCommand( commandList.first() );
			
			QList< QString >* valuesArray = getValues( getValuePartOfCommand( commandList.first() ) );
			
			try{
				bool ok;
				
				int card1Pos = valuesArray->at( 0 ).toInt( &ok );
				if( !ok ) throw 1;
				
				int card1Value = valuesArray->at( 1 ).toInt( &ok );
				if( !ok ) throw 1;
				
				int card2Pos = valuesArray->at( 2 ).toInt( &ok );
				if( !ok ) throw 1;
				
				int card2Value = valuesArray->at( 3 ).toInt( &ok );
				if( !ok ) throw 1;
				
				emit signalShowOpponentCards( card1Pos, Card(card1Value).getCardText(), card2Pos, Card(card2Value).getCardText() );
			}catch( int error ){
				kDebug() << "ERROR! Wrong value in visible cards command!";
			}
			
			delete valuesArray;
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
			//clearCentralCards();
			mCentralCards->clear();
			emit signalClearCentralCards();
		}
		
		if( getCommandPartOfCommand( commandList.first() ) == NEW_GAME_COMMAND ){
			kDebug() << getName() << "Start game.";
			
			newGame();
			//
			//mCentralCards = new CentralCards;
			//
			emit signalPlayerScoresChanged( getScores() );
			emit signalOpponentScoresChanged( 0 );
			
			emit signalStartGame();
		}
		
		if( getCommandPartOfCommand( commandList.first() ) == END_ROUND_COMMAND ){
			kDebug() << getName() << "End round.";
			
			/*QString value = getValuePartOfCommand( commandList.first() );
			
			QString winnerName = value.mid( 0, value.indexOf( ',' ) );
			
			bool ok;
			int winnerScore = value.mid( value.indexOf( ',' )+1 ).toInt( &ok );
			if( ok ){
				emit signalEndRound( winnerName, winnerScore );
			}else{
				kDebug() << "ERROR! Cannot convert winner scores command value to int!";
			}*/
			
			QList< QString >* values = getValues( getValuePartOfCommand( commandList.first() ) );
			
			try{
				if( values->size() != 2 ){
					throw "ERROR! Wrong size of values in end round command!";
				}
				
				QString winnerName = values->at( 0 );
				
				bool ok;
				int winnerScore = values->at( 1 ).toInt( &ok );
				if( !ok ){ 
					throw "ERROR! Cannot convert winner scores command value to int!";
				}
				
				emit signalEndRound( winnerName, winnerScore );
			}catch( QString error ){
				kDebug() << error;
			}
			
			delete values;
		}
		
		if( getCommandPartOfCommand( commandList.first() ) == END_GAME_COMMAND ){
			kDebug() << getName() << "End game.";
			
			emit signalEndGame( getValuePartOfCommand( commandList.first() ) );
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

void Client::slotSelectedCardId( int id )
{
	kDebug() << "Selected card:" << id;
	setSelectableAllCards( false );
	//int pos = mCentralCards->add( Card( getCard( id )->getValue() ) );
	int pos = mCentralCards->add( getCard( id ) );
	removeCard( id );
	
	//emit signalNewCentralCard( pos, mCentralCards->getCard( pos ).getCardText() );
	emit signalNewCentralCard( pos, mCentralCards->getCard( pos )->getCardText() );

	if( isTwentyButtonVisible() ){
		setTwentyButtonVisible( false );
	}
	
	if( isFortyButtonVisible() ){
		setFortyButtonVisible( false );
	}
	
	if( isCloseButtonVisible() ){
		setCloseButtonVisible( false );
	}
	
	//if( getTrumpCard().isSelectable() ){
	if( !getTrumpCard()->isEmpty() && getTrumpCard()->getCard()->isSelectable() ){
		getTrumpCard()->getCard()->setSelectable( false );
		emit signalTrumpCardSelectableChanged( false );
	}
	
	sendCommand( SELECTED_CARD_ID_COMMAND+QString::number( id ) );
}

void Client::slotSelectedTrumpCard()
{
	kDebug() << "Selected trump card.";
	
	int ret = changeTrumpCard( getTrumpCard() );
	
	emit signalNewPlayerCard( ret, getCard(ret)->getCardText() );
	emit signalNewTrumpCard( getTrumpCard()->getCard()->getCardText() );
	
	mTrumpCard->getCard()->setSelectable( false );
	emit signalTrumpCardSelectableChanged( false );
	
	getCard( ret )->setSelectable( true );
	emit signalPlayerCardSelectableChanged( ret, true );
	
	//if( haveRegularMarriages( mTrumpCard ) ){
	if( haveRegularMarriages( mTrumpCard ) ){
		setTwentyButtonVisible( true );
	}
	
	//if( haveTrumpMarriages() ){
	if( haveTrumpMarriages( mTrumpCard ) ){
		setFortyButtonVisible( true );
	}
	
	sendChangeTrumpCard();
}

void Client::slotTwentyButtonClicked()
{
	twentyButtonClicked();
	setSelectableRegularMarriagesCards( mTrumpCard );
	sendCommand( TWENTY_BUTTON_CLICKED_COMMAND );
}

void Client::slotFortyButtonClicked()
{
	kDebug();
	fortyButtonClicked();
	setSelectableTrumpMarriagesCards( mTrumpCard );
	sendCommand( FORTY_BUTTON_CLICKED_COMMAND );
}

void Client::slotCloseButtonClicked()
{
	setCloseButtonVisible( false );
	emit signalCloseDeck();
	
	kDebug() << getTrumpCard()->getCard()->isSelectable();
	if( getTrumpCard()->getCard()->isSelectable() ){
		getTrumpCard()->getCard()->setSelectable( false );
		emit signalTrumpCardSelectableChanged( getTrumpCard()->getCard()->isSelectable() );
	}
	
	sendCommand( CLOSE_BUTTON_CLICKED_COMMAND );
}
