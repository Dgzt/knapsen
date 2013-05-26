#include <QtCore/QStringList>
#include <QtCore/QTimer>
#include <KDE/KDebug>
//#include "centralcards.h"
#include "trump.h"
#include "client.h"

const int WRONG_VALUE_ARRAY_SIZE = -1;
const int WRONG_VALUE = -2;

const int INITIALIZE_TABLE_VALUES_SIZE = 3;
const int NEW_PLAYER_CARD_VALUES_SIZE = 1;
const int NEW_TRUMP_CARD_VALUES_SIZE = 1;
const int OPPONENT_CHANGE_TRUMP_CARD_VALUES_SIZE = 2;
const int VISIBLE_OPPONENT_CARDS_VALUES_SIZE = 4;
const int END_ROUND_VALUES_SIZE = 2;

const int OPPONENT_DELAY = 1000;
const int GET_CENTRAL_CARDS_DELAY = 1000;

Client::Client( QString name ): 
    Player( name ),
    //mStartDeckSize( 0 ),
    //mDeckSize( 0 ),
    mCommandsUnderProcessing( false )
{
    mTrump = new Trump;
    
    connect( this, SIGNAL( connected() ), this, SLOT( slotConnected() ) );
}

Client::~Client()
{
    delete mTrump;
}

/*QList< QString >* Client::getValues( QString valuesStr )
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
}*/

QStringList Client::getValues( const QString& valuesStr )
{
    QStringList valuesArray;
    
    int last = 0;
    for( int i = 0; i < valuesStr.length(); ++i ){
        if( valuesStr[i] == ',' ){
            valuesArray.append( valuesStr.mid( last, i-last ) );
            last = i+1;
        }
    }
    valuesArray.append( valuesStr.mid( last, valuesStr.length()-last ) );
    
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
            commandInitializeTable( getCommandValue( command ) );
        }
        
        if( getCommandName( command ) == NEW_GAME_COMMAND ){
            commandNewGame();
            
            return;
        }
        
        if( getCommandName( command ) == NEW_ROUND_COMMAND ){
            commandNewRound();
            
            return;
        }
        
        if( getCommandName( command ) == NEW_PLAYER_CARD_COMMAND ){
            commandNewPlayerCard( getCommandValue( command ) );
            
            return;
        }
        
        if( getCommandName( command ) == NEW_OPPONENT_CARD_COMMAND ){
            commandNewOpponentCard();
            
            return;
        }
        
        if( getCommandName( command ) == NEW_PLAYER_CARD_TRUMP_CARD_COMMAND ){
            commandNewPlayerTrumpCard();
            
            return;
        }
        
        if( getCommandName( command ) == NEW_OPPONENT_CARD_TRUMP_CARD_COMMAND ){
            commandNewOpponentTrumpCard();
            
            return;
        }
        
        if( getCommandName( command ) == NEW_TRUMP_CARD_COMMAND ){
            commandNewTrumpCard( getCommandValue( command ) );
            
            return;
        }
        
        if( getCommandName( command ) == OPPONENT_CHANGE_TRUMP_CARD_COMMAND ){
            commandOpponentChangeTrumpCard( getCommandValue( command ) );
            
            return;
        }
        
        if( getCommandName( command ) == TWENTY_BUTTON_VISIBLE_COMMAND ){
            commandTwentyButtonVisible();
        }
        
        if( getCommandName( command ) == FORTY_BUTTON_VISIBLE_COMMAND ){
            commandFortyButtonVisible();
        }
        
        if( getCommandName( command ) == SCHNAPSEN_BUTTON_VISIBLE_COMMAND ){
            commandSchnapsenButtonVisible();
        }
        
        if( getCommandName( command ) == CLOSE_BUTTON_VISIBLE_COMMAND ){
            commandCloseButtonVisible();
        }
        
        if( getCommandName( command ) == TRUMP_CARD_SELECTABLE_COMMAND ){
            commandTrumpCardSelectable();
        }
        
        if( getCommandName( command ) == SELECTABLE_ALL_CARDS_COMMAND ){
            commandSelectableAllCards();
        }
        
        if( getCommandName( command ) == SELECTABLE_CERTAIN_CARDS_COMMAND ){
            commandSelectableCertainCards();
        }
        
        if( getCommandName( command ) == OPPONENT_SCHNAPSEN_BUTTON_CLICKED_COMMAND ){
            commandOpponentSchnapsenButtonClicked();
        }
        
        if( getCommandName( command ) == OPPONENT_FORTY_BUTTON_CLICKED_COMMAND ){
            commandOpponentFortyButtonClicked();
        }
        
        if( getCommandName( command ) == OPPONENT_TWENTY_BUTTON_CLICKED_COMMAND ){
            commandOpponentTwentyButtonClicked();
        }
        
        if( getCommandName( command ) == OPPONENT_IN_ACTION_COMMAND ){
            commandOpponentInAction();
        }
        
        if( getCommandName( command ) == OPPONENT_SELECTED_CARD_COMMAND ){
            commandOpponentSelectedCard( getCommandValue( command ) );
            
            return;
        }
        
        if( getCommandName( command ) == VISIBLE_OPPONENT_CARDS_COMMAND ){
            commandVisibleOpponentCards( getCommandValue( command ) );
            
            return;
        }
        
        if( getCommandName( command ) == PLAYER_TRICKS_CHANGED_COMMAND ){
            commandPlayerTricksChanged( getCommandValue( command ) );
        }
        
        if( getCommandName( command ) == PLAYER_SCORES_CHANGED_COMMAND ){
            commandPlayerScoresChanged( getCommandValue( command ) );
        }
        
        if( getCommandName( command ) == OPPONENT_TRICKS_CHANGED_COMMAND ){
            commandOpponentTricksChanged( getCommandValue( command ) );
        }
        
        if( getCommandName( command ) == OPPONENT_SCORES_CHANGED_COMMAND ){
            commandOpponentScoresChanged( getCommandValue( command ) );
        }
        
        if( getCommandName( command ) == OPPONENT_CLICKED_TO_CLOSE_BUTTON_COMMAND ){
            commandOpponentCloseButtonClicked();
        }
        
        if( getCommandName( command ) == PLAYER_GET_CENTRAL_CARDS_COMMAND ){
            commandPlayerGetCentralCards();
            
            return;
        }
        
        if( getCommandName( command ) == OPPONENT_GET_CENTRAL_CARDS_COMMAND ){
            commandOpponentGetCentralCards();
            
            return;
        }
        
        if( getCommandName( command ) == END_ROUND_COMMAND ){
            commandEndRound( getCommandValue( command ) );
        }
        
        if( getCommandName( command ) == END_GAME_COMMAND ){
            commandEndGame( getCommandValue( command ) );
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

void Client::commandInitializeTable( const QString& commandValue )
{
    kDebug() << getName() << "Initialize table, value." << commandValue;
    
    //QList< QString >* valuesArray = getValues( commandValue );
    QStringList valuesArray = getValues( commandValue );
    
    try{
        if( valuesArray.size() != INITIALIZE_TABLE_VALUES_SIZE ) throw WRONG_VALUE_ARRAY_SIZE;
        
        QString opponentName = valuesArray.at( 0 );
        
        Knapsen::TypeOfCards typeOfCards;
        if( valuesArray.at( 1 ) == TYPE_OF_CARDS_GERMAN_SUITS_VALUE ){
            typeOfCards = Knapsen::GermanSuits;
        }else if(valuesArray.at( 1 ) == TYPE_OF_CARDS_FRENCH_SUITS_VALUE ){
            typeOfCards = Knapsen::FrenchSuits;
        }else{
            throw WRONG_VALUE;
        }
        
        bool ok;
        mStartDeckSize = valuesArray.at( 2 ).toInt( &ok );
        if( !ok ) throw WRONG_VALUE;
        
        mDeckSize = mStartDeckSize;
        
        setLowestCard( mStartDeckSize );
        
        emit signalInitialize( getName(), opponentName, typeOfCards );
    }catch( int error ){
        if( error == WRONG_VALUE_ARRAY_SIZE ) kDebug() << "ERROR! Wrong size of value array!";
        if( error == WRONG_VALUE ) kDebug() << "ERROR! Wrong value!";
    }
        
    //delete valuesArray;
}

void Client::commandNewGame()
{
    kDebug() << getName() << "Start game.";
    
    newGame();
    
    emit signalOpponentScoresChanged( 0 );
    emit signalNewGame();
}

void Client::commandNewRound()
{
    kDebug() << getName() << "New round.";
    newRound();
    
    clearCentralCards();
    
    mDeckSize = mStartDeckSize;
    
    emit signalOpponentTricksChanged( 0 );
    
    emit signalNewRound();        
}

void Client::commandNewPlayerCard( const QString& commandValue )
{
    kDebug() << getName() << "new card:" << getCommandValue( commandValue );
    
    //QList< QString >* values = getValues( commandValue );
    QStringList values = getValues( commandValue );
    
    try{
        if( values.size() != NEW_PLAYER_CARD_VALUES_SIZE ) throw WRONG_VALUE_ARRAY_SIZE;
        
        bool ok;
        int ret = getCommandValue( values.at( 0 ) ).toInt( &ok );
        if( !ok ) throw WRONG_VALUE;
        
        bool lastCard = false;
        --mDeckSize;
        if( mDeckSize == 0 ){
            lastCard = true;
        }
        
        Card* card = new Card( ret );
        addNewCard( card );
    
        emit signalNewPlayerCard( lastCard, card );
    }catch( int error ){
        if( error == WRONG_VALUE_ARRAY_SIZE ) kDebug() << "ERROR! Wrong new player card values size!";
        if( error == WRONG_VALUE ) kDebug() << "ERROR! Cannot convert new player card value to int!";
    }
    
    //delete values;
}

void Client::commandNewOpponentCard()
{
    kDebug() << getName() << "New opponent card.";
    
    bool lastCard = false;
    
    --mDeckSize;
    if( mDeckSize == 0 ){
        lastCard = true;
    }
    
    emit signalNewOpponentCard( lastCard );
}

void Client::commandNewPlayerTrumpCard()
{
    kDebug() << getName() << "Add the trump card to player.";
    
    //Card* newCard = mTrump->takeCard();
    //addNewCard( newCard );
    addNewCard( mTrump->takeCard() );
    
    emit signalNewPlayerCardTrumpCard();
}

void Client::commandNewOpponentTrumpCard()
{
    kDebug() << getName() << "Add the trump card to the opponent.";
    
    mTrump->clearTrumpCard();
    
    emit signalNewOpponentCardTrumpCard();
}

void Client::commandNewTrumpCard( const QString& commandValue )
{
    kDebug() << getName() << "Trump card:" << getCommandValue( commandValue );
    
    //QList< QString >* values = getValues( commandValue );
    QStringList values = getValues( commandValue );
    
    try{
        if( values.size() != NEW_TRUMP_CARD_VALUES_SIZE ) throw WRONG_VALUE_ARRAY_SIZE;
        
        bool ok;
        int ret = getCommandValue( values.at( 0 ) ).toInt( &ok );
        if( !ok ) throw WRONG_VALUE;
        
        if( !mTrump->isEmpty() ){
            //mTrump->clearTrumpCard( true );
            mTrump->clearTrumpCard();
        }
        
        mTrump->addNewCard( new Card( ret ) );
        
        --mDeckSize;
        
        emit signalNewTrumpCard( mTrump->getCard() );
        //break;
        
    }catch( int error ){
        if( error == WRONG_VALUE_ARRAY_SIZE ) kDebug() << "ERROR! Wrong new trump card values size";
        if( error == WRONG_VALUE ) kDebug() << "ERROR! Cannot convert new trump card command value to int!";
    }
    
    //delete values;
}

void Client::commandOpponentChangeTrumpCard( const QString& commandValue )
{
    kDebug() << getName() << "Opponent changed trump card.";
    
    //QList< QString >* valueArray = getValues( getCommandValue( commandValue ) );
    QStringList values = getValues( commandValue );
    
    try{
        if( values.size() != OPPONENT_CHANGE_TRUMP_CARD_VALUES_SIZE ) throw WRONG_VALUE_ARRAY_SIZE;
        
        bool ok;
        int cardId = values.at( 0 ).toInt( &ok );
        if( !ok ) throw WRONG_VALUE;
        
        int trumpCardValue = values.at( 1 ).toInt( &ok );
        if( !ok ) throw WRONG_VALUE;
        
        mTrump->clearTrumpCard();
        mTrump->addNewCard( new Card( trumpCardValue ) );
        
        emit signalOpponentChangeTrumpCard( cardId, mTrump->getCard() );
        
    }catch( int error ){
        if( error == WRONG_VALUE_ARRAY_SIZE ) kDebug() << "ERROR! Wrong size of values in opponent change trump card command!";
        if( error == WRONG_VALUE ) kDebug() << "ERROR! Wrong value in opponent change trump card command!";
    }
    
    //delete valueArray;
}

void Client::commandTwentyButtonVisible()
{
    kDebug() << getName() << "Twenty button visible.";
    
    setTwentyButtonVisible( true );
}

void Client::commandFortyButtonVisible()
{
    kDebug() << getName() << "Forty button visible.";
    
    setFortyButtonVisible( true );
}

void Client::commandSchnapsenButtonVisible()
{
    kDebug() << getName() << "Schnapsen button visible";
    
    setSchnapsenButtonVisible( true );
}

void Client::commandCloseButtonVisible()
{
    kDebug() << getName() << "Close button visible.";
    
    setCloseButtonVisible( true );
}

void Client::commandTrumpCardSelectable()
{
    kDebug() << getName() << "Selectable trump card.";
    
    mTrump->getCard()->setSelectable( true );
    emit signalTrumpCardSelectableChanged( true );
}

void Client::commandSelectableAllCards()
{
    kDebug() << getName() << "Selectable all cards.";
    
    setSelectableAllCards( true );
    
    //Emit for bot, it in action
    emit signalPlayerInAction();
}

void Client::commandSelectableCertainCards()
{
    kDebug() << getName() << "Selectable certan cards.";
    
    setSelectableCertainCards( mCentralCards , mTrump );
    emit signalPlayerInAction();
}

void Client::commandOpponentSchnapsenButtonClicked()
{
    kDebug() << getName() << "Opponent clicked to schnapsen button.";
    
    emit signalOpponentSchnapsenButtonClicked();
}

void Client::commandOpponentFortyButtonClicked()
{
    kDebug() << getName() << "Opponent clicked to forty button.";
    
    emit signalOpponentFortyButtonClicked();
}

void Client::commandOpponentTwentyButtonClicked()
{
    kDebug() << getName() << "Opponent clicked to twenty button.";
    
    emit signalOpponentTwentyButtonClicked();
}

void Client::commandOpponentInAction()
{
    kDebug() << getName() << "Opponent in action.";
    
    emit signalOpponentInAction();
}

void Client::commandOpponentSelectedCard( const QString& commandValue )
{
    kDebug() << getName() << "Opponent selected new card.";
    
    //QList< QString >* valuesArray = getValues( getCommandValue( commandValue ) );
    QStringList values = getValues( commandValue );
    
    try{
        if( values.size() != 2 ) throw WRONG_VALUE_ARRAY_SIZE;
                
        bool ok;
        
        int cardId = values.at( 0 ).toInt( &ok );
        if( !ok ) throw WRONG_VALUE;
        
        int cardValue = values.at( 1 ).toInt( &ok );
        if( !ok ) throw WRONG_VALUE;
        
        Card* card = new Card( cardValue );
        mCentralCards.append( card );
        
        emit signalOpponentSelectedCard( cardId, card );
    }catch( int error ){
        if( error == WRONG_VALUE_ARRAY_SIZE ) kDebug() << "ERROR! Wrong values size!";
        if( error == WRONG_VALUE ) kDebug() << "ERROR! Wrong value!";
    }
            
    //delete valuesArray;
}

void Client::commandVisibleOpponentCards( const QString& commandValue )
{
    kDebug() << getName() << "Visible opponent cards:" << getCommandValue( commandValue );
    
    //QList< QString >* valuesArray = getValues( getCommandValue( commandValue ) );
    QStringList values = getValues( commandValue );
    
    try{
        if( values.size() != VISIBLE_OPPONENT_CARDS_VALUES_SIZE ) throw WRONG_VALUE_ARRAY_SIZE;

        bool ok;
        
        int card1Pos = values.at( 0 ).toInt( &ok );
        if( !ok ) throw WRONG_VALUE;
        
        int card1Value = values.at( 1 ).toInt( &ok );
        if( !ok ) throw WRONG_VALUE;
        
        int card2Pos = values.at( 2 ).toInt( &ok );
        if( !ok ) throw WRONG_VALUE;
        
        int card2Value = values.at( 3 ).toInt( &ok );
        if( !ok ) throw WRONG_VALUE;
        
        emit signalShowOpponentCards( card1Pos, Card(card1Value), card2Pos, Card(card2Value) );
    }catch( int error ){
        if( error == WRONG_VALUE_ARRAY_SIZE ) kDebug() << "ERROR! Wrong size of values in visible opponent cards command!";
        if( error == WRONG_VALUE ) kDebug() << "ERROR! Wrong value in visible cards command!";
    }
    
    //delete valuesArray;
}

void Client::commandPlayerTricksChanged( const QString& commandValue )
{
    kDebug() << getName() << "Player tricks changed:" << getCommandValue( commandValue );
    
    bool ok;
    int ret = getCommandValue( commandValue ).toInt( &ok );
    if( ok ){
        setTricks( ret );
    }else{
        kDebug() << "ERROR! Cannot convert player tricks changed command value to int!";
    }
}

void Client::commandPlayerScoresChanged( const QString& commandValue )
{
    kDebug() << getName() << "Player scores changed:" << getCommandValue( commandValue );
    
    bool ok;
    int ret = getCommandValue( commandValue ).toInt( &ok );
    if( ok ){
        setScores( ret );
    }else{
        kDebug() << "ERROR! Cannot convert player scores changed command value to int!";
    }
}

void Client::commandOpponentTricksChanged( const QString& commandValue )
{
    kDebug() << getName() << "Opponent tricks changed:" << getCommandValue( commandValue );
    
    bool ok;
    int ret = getCommandValue( commandValue ).toInt( &ok );
    if( ok ){
        emit signalOpponentTricksChanged( ret );
    }else{
        kDebug() << "ERROR! Cannot convert opponent tricks changed command value to int!";
    }
}

void Client::commandOpponentScoresChanged( const QString& commandValue )
{
    kDebug() << getName() << "Opponent scores changed:" << getCommandValue( commandValue );
    
    bool ok;
    int ret = getCommandValue( commandValue ).toInt( &ok );
    if( ok ){
        emit signalOpponentScoresChanged( ret );
    }else{
        kDebug() << "ERROR! Cannot convert opponent scores changed command value to int!";
    }
}

void Client::commandPlayerGetCentralCards()
{
    kDebug() << getName() << "Player get central cards.";
    
    clearCentralCards();
    
    //emit signalPlayerGetCentralCards();
    QTimer::singleShot( GET_CENTRAL_CARDS_DELAY, this, SLOT( slotPlayerGetCentralCards() ) );
}

void Client::commandOpponentGetCentralCards()
{
    kDebug() << getName() << "Opponent get central cards.";
    
    clearCentralCards();
    
    //emit signalOpponentGetCentralCards();
    QTimer::singleShot( GET_CENTRAL_CARDS_DELAY, this, SLOT( slotOpponentGetCentralCards() ) );
}

void Client::commandEndRound( const QString& commandValue )
{
    kDebug() << getName() << "End round.";
    
    //QList< QString >* values = getValues( getCommandValue( commandValue ) );
    QStringList values = getValues( commandValue );
    
    try{
        if( values.size() != END_ROUND_VALUES_SIZE ) throw WRONG_VALUE_ARRAY_SIZE;
        
        QString winnerName = values.at( 0 );
        
        bool ok;
        int winnerScores = values.at( 1 ).toInt( &ok );
        if( !ok ) throw WRONG_VALUE;
        
        emit signalEndRound( winnerName, winnerScores );
    }catch( int error ){
        if( error == WRONG_VALUE_ARRAY_SIZE ) kDebug() << "ERROR! Wrong size of values in end round command!";
        if( error == WRONG_VALUE ) kDebug() << "ERROR! Cannot convert winner scores command value to int!";
    }
            
    //delete values;
}

void Client::commandEndGame(const QString& commandValue)
{
    kDebug() << getName() << "End game.";
    
    emit signalEndGame( getCommandValue( commandValue ) );
}

/*void Client::slotSelectCardId( int cardId )
{
    kDebug() << "Select card:" << cardId;
    setSelectableAllCards( false );
    
    Card* card = takeCard( cardId );
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

    sendCommand( SELECTED_CARD_ID_COMMAND+QString::number( cardId ) );
}*/

void Client::slotSelectCardId( int cardId, int delay )
{
    kDebug() << "Select card:" << cardId;
    setSelectableAllCards( false );
    
    Card* card = takeCard( cardId );
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

    //sendCommand( SELECTED_CARD_ID_COMMAND+QString::number( cardId ) );
    mLastSelectedCardId = cardId;
    QTimer::singleShot( delay, this, SLOT( slotSendSelectedCardId() ) );
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
    kDebug();
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
