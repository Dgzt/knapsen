#ifndef PLAYER_H
#define PLAYER_H

#include <QtNetwork/QTcpSocket>
#include "card.h"
#include "enums.h"

//Names
static const QString NAME_COMMAND = ":NAME=";
//
static const QString NAME_IS_BUSY_COMMAND =":NAME_IS_BUSY";
//
//
static const QString SERVER_IS_FULL_COMMAND = ":SERVER_IS_FULL";
//

//Configs
static const QString TYPE_OF_CARDS_GERMAN_SUITS_VALUE = "GERMAN_SUIT";
static const QString TYPE_OF_CARDS_FRENCH_SUITS_VALUE = "FRENCH_SUIT";
static const QString INITIALIZE_TABLE_COMMAND = ":INITIALIZE_TABLE=";

//
static const QString NEW_PLAYER_CARD_COMMAND = ":NEW_PLAYER_CARD=";
//
static const QString NEW_PLAYER_CARD_TRUMP_CARD_COMMAND = ":NEW_PLAYER_CARD_TRUMP_CARD";
//
static const QString NEW_OPPONENT_CARD_COMMAND = ":NEW_OPPONENT_CARD";
//
static const QString NEW_OPPONENT_CARD_TRUMP_CARD_COMMAND = ":NEW_OPPONENT_CARD_TRUMP_CARD";
//

//
static const QString NEW_TRUMP_CARD_COMMAND = ":NEW_TRUMP_CARD=";
static const QString TRUMP_CARD_SELECTABLE_COMMAND = ":TRUMP_CARD_SELECTABLE";
static const QString CHANGE_TRUMP_CARD_COMMAND = ":CHANGE_TRUMP_CARD";
static const QString OPPONENT_CHANGE_TRUMP_CARD_COMMAND = ":OPPONENT_CHANGE_TRUMP_CARD=";

//static const QString CLEAR_TRUMP_CARD_COMMAND = ":CLEAR_TRUMP_CARD";

//
static const QString SELECTABLE_ALL_CARDS_COMMAND = ":SELECTABLE_ALL_CARDS";

//Selectable cards that's type equal with trump card's type or central card's type
static const QString SELECTABLE_CERTAIN_CARDS_COMMAND = ":SELECTABLE_CERTAIN_CARDS";

//
static const QString OPPONENT_IN_ACTION_COMMAND = ":OPPONENT_IN_ACTION";
//

static const QString SELECTED_CARD_ID_COMMAND = ":SELECTED_CARD_ID=";
//static const QString OPPONENT_SELECTED_CARD_ID_COMMAND = ":OPPONENT_SELECTED_CARD_ID=";
//static const QString OPPONENT_ADD_NEW_CENTRAL_CARD_COMMAND = ":OPPONENT_ADD_NEW_CENTRAL_CARD=";
static const QString OPPONENT_SELECTED_CARD_COMMAND = ":OPPONENT_SELECTED_CARD=";

//
static const QString PLAYER_TRICKS_CHANGED_COMMAND = ":PLAYER_TRICKS_CHANGED=";
static const QString OPPONENT_TRICKS_CHANGED_COMMAND = ":OPPONENT_TRICKS_CHANGED=";
static const QString PLAYER_SCORES_CHANGED_COMMAND = ":PLAYER_SCORES_CHANGED=";
static const QString OPPONENT_SCORES_CHANGED_COMMAND = ":OPPONENT_SCORES_CHANGED=";

//
static const QString OPPONENT_DISCONNECTED_COMMAND = ":OPPONENT_DISCONNECTED";

//
//static const QString CLEAR_CENTRAL_CARDS_COMMAND = ":CLEAR_CENTRAL_CARDS";
static const QString PLAYER_GET_CENTRAL_CARDS_COMMAND = ":PLAYER_GET_CENTRAL_CARDS";
static const QString OPPONENT_GET_CENTRAL_CARDS_COMMAND = ":OPPONENT_GET_CENTRAL_CARDS";

//
static const QString TWENTY_BUTTON_VISIBLE_COMMAND = ":TWENTY_BUTTON_VISIBLE";
static const QString FORTY_BUTTON_VISIBLE_COMMAND = ":FORTY_BUTTON_VISIBLE";
static const QString SCHNAPSEN_BUTTON_VISIBLE_COMMAND = ":SCHNAPSEN_BUTTON_VISIBLE";
static const QString TWENTY_BUTTON_CLICKED_COMMAND = ":TWENTY_BUTTON_CLICKED";
static const QString FORTY_BUTTON_CLICKED_COMMAND = ":FORTY_BUTTON_CLICKED";
static const QString SCHNAPSEN_BUTTON_CLICKED_COMMAND = ":SCHNAPSEN_BUTTON_CLICKED";
static const QString VISIBLE_OPPONENT_CARDS_COMMAND = ":VISIBLE_OPPONENT_CARDS=";

//
static const QString CLOSE_BUTTON_VISIBLE_COMMAND = ":CLOSE_BUTTON_VISIBLE";
static const QString CLOSE_BUTTON_CLICKED_COMMAND = ":CLOSE_BUTTON_CLICKED";
static const QString OPPONENT_CLICKED_TO_CLOSE_BUTTON_COMMAND = ":OPPONENT_CLICKED_TO_CLOSE_BUTTON";

//
static const QString NEW_ROUND_COMMAND = ":NEW_ROUND";
static const QString END_ROUND_COMMAND = ":END_ROUND=";
static const QString START_NEXT_ROUND_COMMAND = ":START_NEXT_ROUND";

static const QString COMMANDS_END_COMMAND = ":COMMANDS_END";

//
static const QString NEW_GAME_COMMAND = ":NEW_GAME";

//
static const QString END_GAME_COMMAND = ":END_GAME=";
static const QString START_NEXT_GAME_COMMAND = ":START_NEXT_GAME";
//

class CentralCards;

class Trump;

class Player : public QTcpSocket
{
    Q_OBJECT
    
    //Player's name
    QString mName;
    
    Card::Type mLowestCardType;
    
    //int mNumberOfCardsInHand;
    //Card** mCards;
    QList< Card* > mCards;
    
    //Player's tricks
    int mTricks;
    
    //Player's scores
    int mScores;
    
    //If this variable is visible, then visible the twenty button
    bool mTwentyButtonVisible;
    //If this variable is visible, then visible the forty button
    bool mFortyButtonVisible;
    //If this variable is visible, then visible the close button
    bool mCloseButtonVisible;
    
    //If this variable is visible, then visible the schnapsen button
    bool mSchnapsenButtonVisible;
    
    
    virtual void newCommand( QString );
    
    bool setSelectableCardsWhatEqualSuit( Card::Suit );
    
private slots:
    void slotReadyRead();
    void slotDisconnected(){ emit signalPlayerDisconnected( this ); }
    
protected:
    QString getCommandPartOfCommand( QString );
    QString getValuePartOfCommand( QString );
    
    void newGame();
    void newRound();
    
    void setLowestCard( int );
    Card::Type getLowestCardType(){ return mLowestCardType; }
    
    //int addNewCard( Card* );
    void addNewCard( Card* card ){ mCards.append( card ); }
    //void removeCard( int );

    
    
    //Set selectable OR not selectable all avalibe card
    void setSelectableAllCards( bool );
    //Set selectable cards, which suit of cards equal the suit of central card or suit of trump card
    void setSelectableCertainCards( CentralCards* , Trump* );
    
    void twentyButtonClicked();
    void fortyButtonClicked();
    
    void setCloseButtonVisible( bool visible ){ mCloseButtonVisible = visible; emit signalCloseButtonVisible( visible ); }
    bool isCloseButtonVisible() const{ return mCloseButtonVisible; }
    
    void setTricks( int tricks ){ mTricks = tricks; }
    void setScores( int scores ){ mScores = scores; }
    
    //Send command to socket
    void sendCommand( QString command ){ write( command.toAscii() ); }
    
public:
    Player( QString );
    ~Player();
    
    //Get nickname
    QString getName() const { return mName; }
    
    //int getNumberOfCardsInHandNow();
    int getNumberOfCardsInHand(){ return mCards.size(); }
    
    //Card* getCard( int );
    Card* getCard( int id ){ return mCards[ id ]; }
    Card* takeCard( int id ){ return mCards.takeAt( id ); }
    
    //If this card is king, then return equal suit of queen, 
    //else this card is queen, then return equal suit of king
    //int getPositionOfPairOfCard( Card );
    //int getPositionOfPairOfCard( const Card* );
    int getPositionOfPairOfCardId( int );
    
    
    void addTricks( int );
    int getTricks() const{ return mTricks; }
    void addScores( int );
    int getScores() const{ return mScores; }
    
    bool haveRegularMarriages( Trump* ) const;
    bool haveTrumpMarriages( Trump* ) const;
    
    bool canChangeTrumpCard( Trump* ) const;
    int changeTrumpCard( Trump* );
    
    void setTwentyButtonVisible( bool );
    bool isTwentyButtonVisible() const{ return mTwentyButtonVisible; }
    void setFortyButtonVisible( bool );
    bool isFortyButtonVisible() const{ return mFortyButtonVisible; }
    //
    void setSchnapsenButtonVisible( bool );
    bool isSchnapsenButtonVisible(){ return mSchnapsenButtonVisible; }
    //
    
    void setSelectableRegularMarriagesCards( Trump* );
    void setSelectableTrumpMarriagesCards( Trump* );
    
    //-- Commands --
    
    //
    void sendNameIsBusy(){ sendCommand( NAME_IS_BUSY_COMMAND ); }
    
    void sendServerIsFull(){ sendCommand( SERVER_IS_FULL_COMMAND ); }
    //

    //void sendInitializeTable( QString, Knapsen::TypeOfCards, int, int );
    void sendInitializeTable( QString, Knapsen::TypeOfCards, int );
    
    //Set and send the new card
    void sendNewCard( Card* );
    void sendNewCardTrumpCard(){ sendCommand( NEW_PLAYER_CARD_TRUMP_CARD_COMMAND ); }
    
    //Send to player the new opponent card id
    void sendNewOpponentCard(){ sendCommand( NEW_OPPONENT_CARD_COMMAND ); }
    void sendNewOpponentCardTrumpCard(){ sendCommand( NEW_OPPONENT_CARD_TRUMP_CARD_COMMAND ); }
    
    //Set and send the new trump card
    void sendNewTrumpCard( Trump* );
    
    //Send the opponent change trump card
    void sendOpponentChangeTrumpCard( int, Trump* );
    
    //void sendClearTrumpCard(){ sendCommand( CLEAR_TRUMP_CARD_COMMAND ); }
    
    //Set and send enabled all card
    void sendSelectableAllCards();
    
    //void sendSelectableCertainCards();
    void sendSelectableCertainCards( CentralCards *, Trump* );
    
    void sendOpponentInAction(){ sendCommand( OPPONENT_IN_ACTION_COMMAND ); }
    
    //void sendOpponentSelectedCardId( int id ){ sendCommand( OPPONENT_SELECTED_CARD_ID_COMMAND+QString::number( id ) ); }
    
    //void sendOpponentAddNewCentralCard( const Card* );
    
    //
    void sendOpponentSelectedCard( int, const Card* );
    //
    
    //
    void sendOpponentTricksChanged( int value ){ sendCommand( OPPONENT_TRICKS_CHANGED_COMMAND+QString::number( value ) ); }
    void sendOpponentScoresChanged( int value ){ sendCommand( OPPONENT_SCORES_CHANGED_COMMAND+QString::number( value ) ); }
    //
    
    void sendOpponentDisconnected(){ sendCommand( OPPONENT_DISCONNECTED_COMMAND ); }
    
    //void sendClearCentralCards(){ sendCommand( CLEAR_CENTRAL_CARDS_COMMAND ); }
    void sendPlayerGetCentralCards(){ sendCommand( PLAYER_GET_CENTRAL_CARDS_COMMAND ); }
    void sendOpponentGetCentralCards(){ sendCommand( OPPONENT_GET_CENTRAL_CARDS_COMMAND ); }
    
    void sendTwentyButtonVisible(){ sendCommand( TWENTY_BUTTON_VISIBLE_COMMAND ); }
    void sendFortyButtonVisible(){ sendCommand( FORTY_BUTTON_VISIBLE_COMMAND ); }
    //
    void sendSchnapsenButtonVisible(){ sendCommand( SCHNAPSEN_BUTTON_VISIBLE_COMMAND ); }
    //
    
    void sendCloseButtonVisible();
    
    void sendOpponentClickedToCloseButton(){ sendCommand( OPPONENT_CLICKED_TO_CLOSE_BUTTON_COMMAND ); }
    
    void sendSelectableTrumpCard(){ sendCommand( TRUMP_CARD_SELECTABLE_COMMAND ); }
    void sendChangeTrumpCard(){ sendCommand( CHANGE_TRUMP_CARD_COMMAND ); }     
    
    //If the opponent clicked to twenty or forty button, then show that cards
    void sendVisibleOpponentCards( int, Card*, int, Card* );
    
    void sendNewRound(){ newRound(); sendCommand( NEW_ROUND_COMMAND ); }
    
    void sendNewGame(){ newGame(); sendCommand( NEW_GAME_COMMAND ); }
    
    void sendEndRound( QString winnerName, int winnnerScores ){ sendCommand( END_ROUND_COMMAND+winnerName+","+QString::number( winnnerScores ) ); }
    
    void sendEndGame( QString winnerName ){ sendCommand( END_GAME_COMMAND+winnerName ); }
    
    void sendCommandsEnd(){ sendCommand( COMMANDS_END_COMMAND ); }
    
signals:
    //Signals to server
    void signalNewPlayer( Player* );
    void signalPlayerDisconnected( Player* );
    //void signalSelectedCard( Card*, int );
    void signalSelectedCardId( int );
    void signalTwentyButtonClicked();
    void signalFortyButtonClicked();
    void signalSchnapsenButtonClicked();
    void signalCloseButtonClicked();
    void signalChangeTrumpCard( Player* );
    
    void signalStartNextRound( Player* );
    void signalStartNextGame( Player* );
    
    
    //Signals to player
    void signalPlayerCardSelectableChanged( int, bool );
    void signalTwentyButtonVisible( bool );
    void signalFortyButtonVisible( bool );
    void signalSchnapsenButtonVisible( bool );
    void signalCloseButtonVisible( bool );
    
};

#endif //PLAYER_H
