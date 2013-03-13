#include <KDE/KDebug>
//#include "centralcards.h"
#include "trump.h"
#include "client.h"

const int WRONG_VALUE_ARRAY_SIZE = 1;
const int WRONG_VALUE = 2;

Client::Client( QString name ): 
    Player( name ),
    mSizeOfDeck( 0 ),
    mSizeOfDeckNow( 0 ),
    mCommandsUnderProcessing( false )
{
    //mCentralCards = new CentralCards;
    mTrump = new Trump;
    
    connect( this, SIGNAL( connected() ), this, SLOT( slotConnected() ) );
}

Client::~Client()
{
    //delete mCentralCards;
    delete mTrump;
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
    //kDebug() << command;
    
    if( getCommandName( command ) == OPPONENT_DISCONNECTED_COMMAND ){
        kDebug() << getName() << "Opponent disconnected!";
        
        emit signalGameError( Client::OpponentDisconnected );
        return;
    }
    
    if( getCommandName( command ) == NAME_IS_BUSY_COMMAND ){
        kDebug() << getName() << "Name is busy command.";
        
        emit signalGameError( Client::NameIsBusy );
        return;
    }
    
    if( getCommandName( command ) == SERVER_IS_FULL_COMMAND ){
        kDebug() << getName() << "Server is full.";
        
        emit signalGameError( Client::ServerIsFull );
        return;
    }
    
    if( getCommandName( command ) == COMMANDS_END_COMMAND ){
        //slotProcessCommands();
        
        if( !mCommandsUnderProcessing ){
            mCommandsUnderProcessing = true;
            slotProcessCommands();
        }
    }else{
        mCommandList.append( command );
    }
}

void Client::slotProcessCommands()
{
    while( !mCommandList.empty() ){
        QString command = mCommandList.takeFirst();
        kDebug() << mCommandList.size() << command;
        
        if( getCommandName( command ) == INITIALIZE_TABLE_COMMAND ){
            kDebug() << getName() << "Initialize table, value." << getCommandValue( command );
            
            QList< QString >* valuesArray = getValues( getCommandValue( command ) );
            
            Knapsen::TypeOfCards typeOfCards;
            if( valuesArray->at( 1 ) == TYPE_OF_CARDS_GERMAN_SUITS_VALUE ){
                typeOfCards = Knapsen::GermanSuits;
            }else{ //valuesArray->at( 1 ) == TYPE_OF_CARDS_GERMAN_SUITS_VALUE
                typeOfCards = Knapsen::FrenchSuits;
            }
            
            bool ok;
            mSizeOfDeck = valuesArray->at( 2 ).toInt( &ok );
            mSizeOfDeckNow = mSizeOfDeck;
            
            setLowestCard( mSizeOfDeck );
            
            emit signalInitialize( getName(), valuesArray->at( 0 ), typeOfCards );
            
            delete valuesArray;
        }
        
        if( getCommandName( command ) == NEW_GAME_COMMAND ){
            kDebug() << getName() << "Start game.";
            
            newGame();
            
            //emit signalPlayerScoresChanged( getScores() );
            emit signalOpponentScoresChanged( 0 );
            emit signalNewGame();
            
            return;
        }
        
        if( getCommandName( command ) == NEW_ROUND_COMMAND ){
            kDebug() << getName() << "New round.";
            newRound();
            
            //mCentralCards->clear();
            clearCentralCards();
            
            mSizeOfDeckNow = mSizeOfDeck;
            
            emit signalNewRound();
            //break;
            return;
        }
        
        if( getCommandName( command ) == NEW_PLAYER_CARD_COMMAND ){
            kDebug() << getName() << "new card:" << getCommandValue( command );
            
            bool ok;
            int ret = getCommandValue( command ).toInt( &ok );
            if( ok ){
                
                bool lastCard = false;
                --mSizeOfDeckNow;
                if( mSizeOfDeckNow == 0 ){
                    lastCard = true;
                }
                
                Card* card = new Card( ret );
                addNewCard( card );
                
                emit signalNewPlayerCard( lastCard, card );
                
            }else{
                kDebug() << "ERROR! Cannot convert new player card value to int!";
            }
            return;
        }
        
        if( getCommandName( command ) == NEW_OPPONENT_CARD_COMMAND ){
            kDebug() << getName() << "New opponent card.";
            
            bool lastCard = false;

            --mSizeOfDeckNow;
            if( mSizeOfDeckNow == 0 ){
                lastCard = true;
            }
            
            emit signalNewOpponentCard( lastCard );
            return;
        }
        
        if( getCommandName( command ) == NEW_PLAYER_CARD_TRUMP_CARD_COMMAND ){
            kDebug() << "Add the trump card to player.";
            
            Card* newCard = mTrump->getCard();
            mTrump->clearTrumpCard( false );
            
            addNewCard( newCard );
            
            emit signalNewPlayerCardTrumpCard();
            return;
        }
        
        if( getCommandName( command ) == NEW_OPPONENT_CARD_TRUMP_CARD_COMMAND ){
            kDebug() << "Add the trump card to the opponent.";
            
            mTrump->clearTrumpCard( true );
            
            emit signalNewOpponentCardTrumpCard();
            return;
        }
        
        if( getCommandName( command ) == NEW_TRUMP_CARD_COMMAND ){
            kDebug() << getName() << "Trump card:" << getCommandValue( command );
            
            bool ok;
            int ret = getCommandValue( command ).toInt( &ok );
            if( ok ){
                if( !mTrump->isEmpty() ){
                    mTrump->clearTrumpCard( true );
                }
                
                mTrump->addNewCard( new Card( ret ) );
                
                --mSizeOfDeckNow;
                
                emit signalNewTrumpCard( mTrump->getCard() );
                //break;
                
            }else{
                kDebug() << "ERROR! Cannot convert new trump card command value to int!";
            }
            return;
        }
        
        if( getCommandName( command ) == OPPONENT_CHANGE_TRUMP_CARD_COMMAND ){
            kDebug() << getName() << "Opponent changed trump card.";
            
            QList< QString >* valueArray = getValues( getCommandValue( command ) );
            try{
                if( valueArray->size() != 2 ){
                    throw WRONG_VALUE_ARRAY_SIZE;
                }
                
                bool ok;
                int cardId = valueArray->at( 0 ).toInt( &ok );
                if( !ok ) throw WRONG_VALUE;
                
                int trumpCardValue = valueArray->at( 1 ).toInt( &ok );
                if( !ok ) throw WRONG_VALUE;
                
                mTrump->clearTrumpCard( true );
                mTrump->addNewCard( new Card( trumpCardValue ) );
                
                emit signalOpponentChangeTrumpCard( cardId, mTrump->getCard() );
                
            }catch( int error ){
                if( error == WRONG_VALUE_ARRAY_SIZE ){
                    kDebug() << "ERROR! Wrong size of values in opponent change trump card command!";
                }
                if( error == WRONG_VALUE ){
                    kDebug() << "ERROR! Wrong value in opponent change trump card command!";
                }
            }
            
            delete valueArray;
            return;
        }
        
        if( getCommandName( command ) == TWENTY_BUTTON_VISIBLE_COMMAND ){
            kDebug() << getName() << "Twenty button visible.";
            
            setTwentyButtonVisible( true );
        }
        
        if( getCommandName( command ) == FORTY_BUTTON_VISIBLE_COMMAND ){
            kDebug() << getName() << "Forty button visible.";
            
            setFortyButtonVisible( true );
        }
        
        if( getCommandName( command ) == SCHNAPSEN_BUTTON_VISIBLE_COMMAND ){
            kDebug() << getName() << "Schnapsen button visible";
            
            setSchnapsenButtonVisible( true );
        }
        
        if( getCommandName( command ) == CLOSE_BUTTON_VISIBLE_COMMAND ){
            kDebug() << getName() << "Close button visible.";
            
            setCloseButtonVisible( true );
        }
        
        if( getCommandName( command ) == TRUMP_CARD_SELECTABLE_COMMAND ){
            kDebug() << getName() << "Selectable trump card.";
            
            mTrump->getCard()->setSelectable( true );
            emit signalTrumpCardSelectableChanged( true );
        }
        
        if( getCommandName( command ) == SELECTABLE_ALL_CARDS_COMMAND ){
            kDebug() << getName() << "Selectable all cards.";
            
            setSelectableAllCards( true );
            
            //Emit for bot, it in action
            emit signalPlayerInAction();
        }
        
        if( getCommandName( command ) == SELECTABLE_CERTAIN_CARDS_COMMAND ){
            kDebug() << getName() << "Selectable certan cards.";
            
            setSelectableCertainCards( mCentralCards , mTrump );
            emit signalPlayerInAction();
        }
        
        if( getCommandName( command ) == OPPONENT_SCHNAPSEN_BUTTON_CLICKED_COMMAND ){
            kDebug() << getName() << "Opponent clicked to schnapsen button.";
            
            emit signalOpponentSchnapsenButtonClicked();
        }
        
        if( getCommandName( command ) == OPPONENT_FORTY_BUTTON_CLICKED_COMMAND ){
            kDebug() << getName() << "Opponent clicked to forty button.";
            
            emit signalOpponentFortyButtonClicked();
        }
        
        if( getCommandName( command ) == OPPONENT_TWENTY_BUTTON_CLICKED_COMMAND ){
            kDebug() << getName() << "Opponent clicked to twenty button.";
            
            emit signalOpponentTwentyButtonClicked();
        }
        
        if( getCommandName( command ) == OPPONENT_IN_ACTION_COMMAND ){
            kDebug() << getName() << "Opponent in action.";
            
            emit signalOpponentInAction();
        }
        
        if( getCommandName( command ) == OPPONENT_SELECTED_CARD_COMMAND ){
            kDebug() << getName() << "Opponent selected new card.";
            
            QList< QString >* valuesArray = getValues( getCommandValue( command ) );
            
            try{
                if( valuesArray->size() != 2 ){
                    throw 1;
                }
                
                bool ok;
                
                int cardId = valuesArray->at( 0 ).toInt( &ok );
                if( !ok ) throw 2;
                
                int cardValue = valuesArray->at( 1 ).toInt( &ok );
                if( !ok ) throw 2;
                
                Card* card = new Card( cardValue );
                //mCentralCards->add( card );
                mCentralCards.append( card );
                
                emit signalOpponentSelectedCard( cardId, card );
                //break
            }catch( int error ){
                
            }
            
            delete valuesArray;
            return;
        }
        
        if( getCommandName( command ) == VISIBLE_OPPONENT_CARDS_COMMAND ){
            kDebug() << getName() << "Visible opponent cards:" << getCommandValue( command );
            
            QList< QString >* valuesArray = getValues( getCommandValue( command ) );
            
            try{
                if( valuesArray->size() != 4 ){
                    throw 1;
                }
                
                bool ok;
                
                int card1Pos = valuesArray->at( 0 ).toInt( &ok );
                if( !ok ) throw 2;
                
                int card1Value = valuesArray->at( 1 ).toInt( &ok );
                if( !ok ) throw 2;
                
                int card2Pos = valuesArray->at( 2 ).toInt( &ok );
                if( !ok ) throw 2;
                
                int card2Value = valuesArray->at( 3 ).toInt( &ok );
                if( !ok ) throw 2;
                
                emit signalShowOpponentCards( card1Pos, Card(card1Value), card2Pos, Card(card2Value) );
            }catch( int error ){
                if( error == 1 ){
                    kDebug() << "ERROR! Wrong size of values in visible opponent cards command!";
                }
                if( error == 2 ){
                    kDebug() << "ERROR! Wrong value in visible cards command!";
                }
            }
            
            delete valuesArray;
            return;
        }
        
        if( getCommandName( command ) == PLAYER_TRICKS_CHANGED_COMMAND ){
            kDebug() << getName() << "Player tricks changed:" << getCommandValue( command );
            
            bool ok;
            int ret = getCommandValue( command ).toInt( &ok );
            if( ok ){
                setTricks( ret );
                //emit signalPlayerTricksChanged( getTricks() );
            }else{
                kDebug() << "ERROR! Cannot convert player tricks changed command value to int!";
            }
        }
        
        if( getCommandName( command ) == PLAYER_SCORES_CHANGED_COMMAND ){
            kDebug() << getName() << "Player scores changed:" << getCommandValue( command );
            
            bool ok;
            int ret = getCommandValue( command ).toInt( &ok );
            if( ok ){
                setScores( ret );
                //emit signalPlayerScoresChanged( ret );
            }else{
                kDebug() << "ERROR! Cannot convert player scores changed command value to int!";
            }
        }
        
        if( getCommandName( command ) == OPPONENT_TRICKS_CHANGED_COMMAND ){
            kDebug() << getName() << "Opponent tricks changed:" << getCommandValue( command );
            
            bool ok;
            int ret = getCommandValue( command ).toInt( &ok );
            if( ok ){
                emit signalOpponentTricksChanged( ret );
            }else{
                kDebug() << "ERROR! Cannot convert opponent tricks changed command value to int!";
            }
        }
        
        if( getCommandName( command ) == OPPONENT_SCORES_CHANGED_COMMAND ){
            kDebug() << getName() << "Opponent scores changed:" << getCommandValue( command );
            
            bool ok;
            int ret = getCommandValue( command ).toInt( &ok );
            if( ok ){
                emit signalOpponentScoresChanged( ret );
            }else{
                kDebug() << "ERROR! Cannot convert opponent scores changed command value to int!";
            }
        }
        
        if( getCommandName( command ) == OPPONENT_CLICKED_TO_CLOSE_BUTTON_COMMAND ){
            kDebug() << getName() << "Opponent clicked to close button.";
            emit signalCloseDeck();
        }
        
        if( getCommandName( command ) == PLAYER_GET_CENTRAL_CARDS_COMMAND ){
            kDebug() << getName() << "Player get central cards.";
            
            clearCentralCards();
            
            emit signalPlayerGetCentralCards();
            //break;
            return;
        }
        
        if( getCommandName( command ) == OPPONENT_GET_CENTRAL_CARDS_COMMAND ){
            kDebug() << getName() << "Opponent get central cards.";
            
            clearCentralCards();
            
            emit signalOpponentGetCentralCards();
            //break;
            return;
        }
        
        if( getCommandName( command ) == END_ROUND_COMMAND ){
            kDebug() << getName() << "End round.";
            
            QList< QString >* values = getValues( getCommandValue( command ) );
            
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
        
        if( getCommandName( command ) == END_GAME_COMMAND ){
            kDebug() << getName() << "End game.";
            
            emit signalEndGame( getCommandValue( command ) );
        }
        
    }
    
    kDebug() << getName() << "out of loop" << mCommandList.size();
    mCommandsUnderProcessing = false;
}

void Client::slotConnected()
{
    kDebug() << getName() << "connected.";
    
    sendCommand( NAME_COMMAND+getName() );
}

/*void Client::slotSelectCardId( int id )
{
    kDebug() << "Select card:" << id;
    setSelectableAllCards( false );
    
    Card* card = takeCard( id );
    mCentralCards->add( card );
    
    emit signalNewCentralCard( card );
    
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
    
    if( !mTrump->isEmpty() && mTrump->getCard()->isSelectable() ){
        mTrump->getCard()->setSelectable( false );
        emit signalTrumpCardSelectableChanged( false );
    }
    
    sendCommand( SELECTED_CARD_ID_COMMAND+QString::number( id ) );
}*/

void Client::slotSelectCardId( int id )
{
    kDebug() << "Select card:" << id;
    setSelectableAllCards( false );
    
    Card* card = takeCard( id );
    //mCentralCards->add( card );
    mCentralCards.append( card );
    
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
    
    if( !mTrump->isEmpty() && mTrump->getCard()->isSelectable() ){
        mTrump->getCard()->setSelectable( false );
        emit signalTrumpCardSelectableChanged( false );
    }
    
    sendCommand( SELECTED_CARD_ID_COMMAND+QString::number( id ) );
}

/*void Client::slotSelectTrumpCard()
{
    kDebug() << "Select trump card.";
    
    int ret = changeTrumpCard( mTrump );
    
    emit signalChangePlayerCard( ret, getCard( ret ) );
    emit signalNewTrumpCard( mTrump->getCard() );
    
    mTrump->getCard()->setSelectable( false );
    emit signalTrumpCardSelectableChanged( false );
    
    getCard( ret )->setSelectable( true );
    emit signalPlayerCardSelectableChanged( ret, true );
    
    if( haveRegularMarriages( mTrump ) ){
        setTwentyButtonVisible( true );
    }
    
    if( haveTrumpMarriages( mTrump ) ){
        setFortyButtonVisible( true );
    }
    
    sendChangeTrumpCard();
}*/

void Client::slotSelectTrumpCard()
{
    kDebug();
    
    int id = changeTrumpCard( mTrump );
    
    mTrump->getCard()->setSelectable( false );
    
    //Set the new trump card's selectable to false
    emit signalPlayerCardSelectableChanged( id, false );
    
    //Change cards on the GUI
    emit signalPlayerChangeTrumpCard( id );
    
    if( haveRegularMarriage( mTrump ) ){
        setTwentyButtonVisible( true );
    }
    
    if( haveTrumpMarriage( mTrump ) ){
        setFortyButtonVisible( true );
    }
    
    sendChangeTrumpCard();
}

void Client::slotTwentyButtonClicked()
{
    twentyButtonClicked();
    setSelectableRegularMarriageCards( mTrump );
    sendCommand( TWENTY_BUTTON_CLICKED_COMMAND );
}

void Client::slotFortyButtonClicked()
{
    kDebug();
    fortyButtonClicked();
    setSelectableTrumpMarriageCards( mTrump );
    sendCommand( FORTY_BUTTON_CLICKED_COMMAND );
}

void Client::slotSchnapsenButtonClicked()
{
    kDebug();
    
    setSchnapsenButtonVisible( false );
    
    sendCommand( SCHNAPSEN_BUTTON_CLICKED_COMMAND );
}

void Client::slotCloseButtonClicked()
{
    setCloseButtonVisible( false );
    emit signalCloseDeck();
    
    kDebug() << mTrump->getCard()->isSelectable();
    if( mTrump->getCard()->isSelectable() ){
        mTrump->getCard()->setSelectable( false );
        emit signalTrumpCardSelectableChanged( mTrump->getCard()->isSelectable() );
    }
    
    sendCommand( CLOSE_BUTTON_CLICKED_COMMAND );
}
