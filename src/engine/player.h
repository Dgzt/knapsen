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
static const QString OPPONENT_NAME_COMMAND = ":OPPONENT_NAME=";

//
static const QString SERVER_IS_FULL_COMMAND = ":SERVER_IS_FULL";
//

//Configs
static const QString TYPE_OF_CARDS_COMMAND = ":TYPE_OF_CARD=";
static const QString TYPE_OF_CARDS_GERMAN_SUITS_VALUE = "GERMAN_SUIT";
static const QString TYPE_OF_CARDS_FRENCH_SUITS_VALUE = "FRENCH_SUIT";

static const QString SIZE_OF_DECK_COMMAND = ":SIZE_OF_DECK=";
static const QString NUMBER_OF_CARDS_IN_HAND_COMMAND = ":NUMBER_OF_CARDS_IN_HAND=";
static const QString INITIALIZE_TABLE_COMMAND = ":INITIALIZE_TABLE";

//
static const QString NEW_PLAYER_CARD_COMMAND = ":NEW_PLAYER_CARD=";
static const QString NEW_OPPONENT_CARD_COMMAND_ID = ":NEW_OPPONENT_CARD_ID=";

//
static const QString NEW_TRUMP_CARD_COMMAND = ":NEW_TRUMP_CARD=";
static const QString CLEAR_TRUMP_CARD_COMMAND = ":CLEAR_TRUMP_CARD";
static const QString TRUMP_CARD_SELECTABLE_COMMAND = ":TRUMP_CARD_SELECTABLE";
static const QString CHANGE_TRUMP_CARD_COMMAND = ":CHANGE_TRUMP_CARD";

//
static const QString SELECTABLE_ALL_CARDS_COMMAND = ":SELECTABLE_ALL_CARDS";

//Selectable cards that's type equal with trump card's type or central card's type
static const QString SELECTABLE_CERTAIN_CARDS_COMMAND = ":SELECTABLE_CERTAIN_CARDS";

//
static const QString OPPONENT_IN_ACTION_COMMAND = ":OPPONENT_IN_ACTION";
//

static const QString SELECTED_CARD_ID_COMMAND = ":SELECTED_CARD_ID=";
static const QString OPPONENT_SELECTED_CARD_ID_COMMAND = ":OPPONENT_SELECTED_CARD_ID=";
static const QString OPPONENT_ADD_NEW_CENTRAL_CARD_COMMAND = ":OPPONENT_ADD_NEW_CENTRAL_CARD=";

//
static const QString PLAYER_TRICKS_CHANGED_COMMAND = ":PLAYER_TRICKS_CHANGED=";
static const QString OPPONENT_TRICKS_CHANGED_COMMAND = ":OPPONENT_TRICKS_CHANGED=";
static const QString PLAYER_SCORES_CHANGED_COMMAND = ":PLAYER_SCORES_CHANGED=";
static const QString OPPONENT_SCORES_CHANGED_COMMAND = ":OPPONENT_SCORES_CHANGED=";

//
static const QString OPPONENT_DISCONNECTED_COMMAND = ":OPPONENT_DISCONNECTED";

//
static const QString CLEAR_CENTRAL_CARDS_COMMAND = ":CLEAR_CENTRAL_CARDS";

//
static const QString TWENTY_BUTTON_VISIBLE_COMMAND = ":TWENTY_BUTTON_VISIBLE";
static const QString FORTY_BUTTON_VISIBLE_COMMAND = ":FORTY_BUTTON_VISIBLE";
static const QString TWENTY_BUTTON_CLICKED_COMMAND = ":TWENTY_BUTTON_CLICKED";
static const QString FORTY_BUTTON_CLICKED_COMMAND = ":FORTY_BUTTON_CLICKED";
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

class TrumpCard;

class Player : public QTcpSocket
{
	Q_OBJECT

	//Player's name
	QString mName;
	
	Card::CardType mLowestCardType;
	
	int mNumberOfCardsInHand;
	Card* *mCards;
	
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
	
	
	
	virtual void newCommand( QString );
	
	bool setSelectableCardsWhatEqualSuit( Card::CardSuit );
	
private slots:
	void slotReadyRead();
	void slotDisconnected(){ emit signalPlayerDisconnected( this ); }
	
protected:
	QString getCommandPartOfCommand( QString );
	QString getValuePartOfCommand( QString );
	
	void setNumberOfCardsInHand( int );
	int getNumberOfCardsInHand() const{ return mNumberOfCardsInHand; }
	
	void newGame();
	void newRound();
	
	void setLowestCard( int );
	Card::CardType getLowestCardType(){ return mLowestCardType; }
	
	int addNewCard( Card* );
	void removeCard( int );

	
	//Set selectable OR not selectable all avalibe card
	void setSelectableAllCards( bool );
	//Set selectable cards, which suit of cards equal the suit of central card or suit of trump card
	void setSelectableCertainCards( CentralCards *, TrumpCard * );
	
	//void setSelectableRegularMarriagesCards();
	void setSelectableRegularMarriagesCards( TrumpCard * );
	//void setSelectableTrumpMarriagesCards();
	void setSelectableTrumpMarriagesCards( TrumpCard * );
	
	void setTwentyButtonVisible( bool );
	bool isTwentyButtonVisible() const{ return mTwentyButtonVisible; }
	void setFortyButtonVisible( bool );
	bool isFortyButtonVisible() const{ return mFortyButtonVisible; }
	void twentyButtonClicked();
	void fortyButtonClicked();
	void setCloseButtonVisible( bool visible ){ mCloseButtonVisible = visible; emit signalCloseButtonVisible( visible ); }
	bool isCloseButtonVisible() const{ return mCloseButtonVisible; }
	
	void setTricks( int tricks ){ mTricks = tricks; }
	void setScores( int scores ){ mScores = scores; }
	
	//Send command to socket
	void sendCommand( QString command ){ write( command.toAscii() ); }
	
public:
    Player( QObject* parent = 0 );
	~Player();
	
	//Set nickname
	void setName( QString name ){ mName = name; }
	
	//Get nickname
	QString getName() const { return mName; }
	
	int getNumberOfCardsInHandNow();
	
	Card* getCard( int );
	
	//If this card is king, then return equal suit of queen, 
	//else this card is queen, then return equal suit of king
	int getPositionOfPairOfCard( Card );
	
	void addTricks( int );
	int getTricks() const{ return mTricks; }
	void addScores( int );
	int getScores() const{ return mScores; }
	
	bool haveRegularMarriages( TrumpCard * ) const;
	bool haveTrumpMarriages( TrumpCard * ) const;
	
	bool canChangeTrumpCard( TrumpCard * ) const;
	int changeTrumpCard( TrumpCard * );
	
	//-- Commands --
	
	//
	void sendNameIsBusy(){ sendCommand( NAME_IS_BUSY_COMMAND ); }
	
	void sendServerIsFull(){ sendCommand( SERVER_IS_FULL_COMMAND ); }
	//
	
	//Send the opponent's name
	//void sendOpponentName(){ sendCommand( OPPONENT_NAME_COMMAND+mOpponent->getName() ); }
	void sendOpponentName( QString name ){ sendCommand( OPPONENT_NAME_COMMAND+name ); }
	
	//Send play with german suits cards or french suits cards
	void sendTypeOfCard( Knapsen::TypeOfCards );
	
	//Send 20 or 24 cards in deck
	void sendSizeOfDeck( int size ){ setLowestCard( size ); sendCommand( SIZE_OF_DECK_COMMAND+QString::number( size ) ); }
	
	//Send play with 5 or 6 cards in hand
	void sendNumberOfCardsInHand( int size );
	
	//Build up the table
	void sendInitializeTable(){ sendCommand( INITIALIZE_TABLE_COMMAND ); }
	
	//Set and send the new card
	int sendNewCard( Card* );
	
	//Send to player the new opponent card id
	void sendNewOpponentCard( int id ){ sendCommand( NEW_OPPONENT_CARD_COMMAND_ID+QString::number( id ) ); }
	
	
	//Set and send the new trump card
	void sendNewTrumpCard( TrumpCard* );
	
	void sendClearTrumpCard(){ sendCommand( CLEAR_TRUMP_CARD_COMMAND ); }
	
	//Set and send enabled all card
	void sendSelectableAllCards();

	//void sendSelectableCertainCards();
	void sendSelectableCertainCards( CentralCards *, TrumpCard * );
	
	void sendOpponentInAction(){ sendCommand( OPPONENT_IN_ACTION_COMMAND ); }
	
	void sendOpponentSelectedCardId( int id ){ sendCommand( OPPONENT_SELECTED_CARD_ID_COMMAND+QString::number( id ) ); }
	
	void sendOpponentAddNewCentralCard( Card );
	
	void sendOpponentDisconnected(){ sendCommand( OPPONENT_DISCONNECTED_COMMAND ); }
	
	void sendClearCentralCards(){ sendCommand( CLEAR_CENTRAL_CARDS_COMMAND ); }
	
	void sendTwentyButtonVisible();
	void sendFortyButtonVisible();

	void sendCloseButtonVisible();
	
	void sendOpponentClickedToCloseButton(){ sendCommand( OPPONENT_CLICKED_TO_CLOSE_BUTTON_COMMAND ); }
	
	void sendSelectableTrumpCard(){ sendCommand( TRUMP_CARD_SELECTABLE_COMMAND ); }
	void sendChangeTrumpCard(){ sendCommand( CHANGE_TRUMP_CARD_COMMAND ); }

	
	//If the opponent clicked to twenty or forty button, then show that cards
	void sendVisibleOpponentCards( int, Card, int, Card* );
	
	void sendNewRound(){ newRound(); sendCommand( NEW_ROUND_COMMAND ); }
	
	void sendNewGame(){ newGame(); sendCommand( NEW_GAME_COMMAND ); }
	
	void sendEndRound( QString winnerName, int winnnerScores ){ sendCommand( END_ROUND_COMMAND+winnerName+","+QString::number( winnnerScores ) ); }
	
	void sendEndGame( QString winnerName ){ sendCommand( END_GAME_COMMAND+winnerName ); }
	
	void sendCommandsEnd(){ sendCommand( COMMANDS_END_COMMAND ); }
	
signals:
	//Signals to server
	void signalNewPlayer( Player* );
	void signalPlayerDisconnected( Player* );
	void signalSelectedCard( Card, int );
	void signalTwentyButtonClicked();
	void signalFortyButtonClicked();
	void signalCloseButtonClicked();
	void signalChangedTrumpCard();

	void signalStartNextRound( Player* );
	void signalStartNextGame( Player* );

	
	//Signals to player
	void signalPlayerCardSelectableChanged( int, bool );
	void signalTwentyButtonVisible( bool );
	void signalFortyButtonVisible( bool );
	void signalCloseButtonVisible( bool );
	
};

#endif //PLAYER_H
