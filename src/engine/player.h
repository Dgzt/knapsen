#ifndef PLAYER_H
#define PLAYER_H

#include <QtNetwork/QTcpSocket>
#include "card.h"
#include "enums.h"

//Names
static const QString NAME_COMMAND = ":NAME=";
static const QString OPPONENT_NAME_COMMAND = ":OPPONENT_NAME=";

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

//
static const QString SELECTABLE_ALL_CARDS_COMMAND = ":SELECTABLE_ALL_CARDS";

//Selectable cards that's type equal with trump card's type or central card's type
static const QString SELECTABLE_CERTAIN_CARDS_COMMAND = ":SELECTABLE_CERTAIN_CARDS";

static const QString SELECTED_CARD_ID_COMMAND = ":SELECTED_CARD_ID=";
static const QString OPPONENT_SELECTED_CARD_ID_COMMAND = ":OPPONENT_SELECTED_CARD_ID=";
static const QString OPPONENT_ADD_NEW_CENTRAL_CARD_COMMAND = ":OPPONENT_ADD_NEW_CENTRAL_CARD=";

//
static const QString PLAYER_TRICKS_CHANGED_COMMAND = ":PLAYER_TRICKS_CHANGED=";
static const QString OPPONENT_TRICKS_CHANGED_COMMAND = ":OPPONENT_TRICKS_CHANGED=";

//
static const QString OPPONENT_DISCONNECTED_COMMAND = ":OPPONENT_DISCONNECTED";

//
static const QString CLEAR_CENTRAL_CARDS_COMMAND = ":CLEAR_CENTRAL_CARDS";

//
static const QString TWENTY_BUTTON_VISIBLE_COMMAND = ":TWENTY_VISIBLE_BUTTON";
static const QString FORTY_BUTTON_VISIBLE_COMMAND = ":FORTY_VISIBLE_BUTTON";
static const QString TWENTY_BUTTON_CLICKED_COMMAND = ":TWENTY_BUTTON_CLICKED";
static const QString OPPONENT_TWENTY_BUTTON_CLICKED_COMMAND = ":OPPONENT_TWENTY_BUTTON_CLICKED";
static const QString FORTY_BUTTON_CLICKED_COMMAND = ":FORTY_BUTTON_CLICKED";
static const QString OPPONENT_FORTY_BUTTON_CLICKED_COMMAND = ":OPPONENT_FORTY_BUTTON_CLICKED";


//
static const QString START_GAME_COMMAND = ":START_GAME";

class Player : public QTcpSocket
{
	Q_OBJECT

	//Player's name
	QString mName;
	
	//The opponent, send to whan player get new card, selected card, etc
	Player* mOpponent;
	
	//Player's cards
	QList<Card> mCards;
	
	int mTricks;
	
	//int mScores;
	
	//Trump card
	Card mTrumpCard;
	//
	Card::CardSuit mTrumpCardSuit;
	//
	
	//Central cards
	QList<Card> mCentralCards;
	
	bool mTwentyButtonVisible;
	bool mFortyButtonVisible;
	
	virtual void newCommand( QString );
	
	//Send to player the new opponent card id
	void sendNewOpponentCard( int id ){ mOpponent->sendCommand( NEW_OPPONENT_CARD_COMMAND_ID+QString::number( id ) ); }
	//
	void sendOpponentTwentyButtonClicked(){ sendCommand( OPPONENT_TWENTY_BUTTON_CLICKED_COMMAND ); }
	void sendOpponentFortyButtonClicked(){ sendCommand( OPPONENT_FORTY_BUTTON_CLICKED_COMMAND ); }
	//
	
	bool setSelectableCardsWhatEqualSuit( Card::CardSuit );
	
private slots:
	void slotReadyRead();
	void slotDisconnected(){ emit signalPlayerDisconnected( this ); }
	
protected:
	QString getCommandPartOfCommand( QString );
	QString getValuePartOfCommand( QString );
	
	void setNumberOfCardsInHand( int );
	int getNumberOfCardsInHand() const{ return mCards.size(); }
	
	int addNewCard( Card );
	Card getCard( int id ){ return mCards.at( id ); }
	void removeCard( int id ){ mCards[ id ] = Card(); }
	
	//Trump card
	void setTrumpCard( Card trumpCard ) { mTrumpCard = trumpCard; mTrumpCardSuit = mTrumpCard.getCardSuit(); }
	Card getTrumpCard() const { return mTrumpCard; }
	void clearTrumpCard() { mTrumpCard = Card(); }
	
	//Set selectable OR not selectable all avalibe card
	void setSelectableAllCards( bool );
	void setSelectableCertainCards();
	
	//Add new card to central cards
	void addNewCentralCard( Card );
	void clearCentralCards();
	
	//
	void setTwentyButtonVisible( bool );
	void setFortyButtonVisible( bool );
	void twentyButtonClicked();
	void fortyButtonClicked();
	//
	
	void setTricks( int tricks ){ mTricks = tricks; }
	
	//Send command to socket
	void sendCommand( QString command ){ write( command.toAscii() ); }
	
public:
    Player( QObject* parent = 0 );
	~Player();
	
	//Set opponent's socket
	void setOpponent( Player* opponent ){ mOpponent = opponent; }
	
	//Set nickname
	void setName( QString name ){ mName = name; }
	
	//Get nickname
	QString getName() const { return mName; }
	
	int getNumberOfCardsInHandNow();
	
	//
	int getTricks() const{ return mTricks; }
	//
	
	bool haveRegularMarriages() const;
	bool haveTrumpMarriages() const;
	
	//-- Commands --
	
	//Send the opponent's name
	void sendOpponentName(){ sendCommand( OPPONENT_NAME_COMMAND+mOpponent->getName() ); }
	
	//Send play with german suits cards or french suits cards
	void sendTypeOfCard( Knapsen::TypeOfCards );
	
	//Send 20 or 24 cards in deck
	void sendSizeOfDeck( int size ){ sendCommand( SIZE_OF_DECK_COMMAND+QString::number( size ) ); }
	
	//Send play with 5 or 6 cards in hand
	void sendNumberOfCardsInHand( int size );
	
	//Build up the table
	void sendInitializeTable(){ sendCommand( INITIALIZE_TABLE_COMMAND ); }
	
	//Set and send the new card
	void sendNewCard( const Card );
	
	//Set and send the new trump card
	void sendNewTrumpCard( const Card );
	//
	void sendClearTrumpCard();
	//
	
	//Set and send enabled all card
	void sendSelectableAllCards();
	//
	void sendSelectableCertainCards();
	//
	
	void sendOpponentAddNewCentralCard( Card );
	
	void sendOpponentDisconnected(){ sendCommand( OPPONENT_DISCONNECTED_COMMAND ); }
	
	void addTricks( int );
	
	void sendClearCentralCards();
	
	void sendTwentyButtonVisible();
	void sendFortyButtonVisible();
	
	
	void sendStartGame(){ sendCommand( START_GAME_COMMAND ); }
	
signals:
	//Signals to server
	void signalNewPlayer( Player* );
	void signalPlayerDisconnected( Player* );
	void signalSelectedCard( Card );
	//
	void signalTwentyButtonClicked();
	void signalFortyButtonClicked();
	//
	
	//Signals to player
	void signalPlayerCardSelectableChanged( int, bool );
	void signalTwentyButtonVisible( bool );
	void signalFortyButtonVisible( bool );
	void signalCentralCardChanged( int, Card );
};

#endif //PLAYER_H
