#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork/QTcpServer>
#include "card.h"
#include "enums.h"

class GameSequence;

class Player;
//
class Bot;
//
class Deck;

//
class CentralCards;
//

//
class TrumpCard;
//

class Server : public QTcpServer
{
	Q_OBJECT
	
	GameSequence* mGameSequence;
	
	//The bot, if play multiplayer game, then this is an unused variable(pointer)
	Bot *mBot;
	
	//Size of deck, 20 or 24
	int mSizeOfDeck;
	
	//Number of cards in hand, 5 or 6
	int mNumberOfCardsInHand;
	
	//Type of cards, German suits or french suits
	Knapsen::TypeOfCards mTypeOfCards;

	//The player lists
	QList<Player*> mPlayerList;
	
	//Deck
	Deck *mDeck;
	
	//Card mTrumpCard;
	TrumpCard *mTrumpCard;

	//Central cards
	CentralCards *mCentralCards;
	
	//
	//Live this pointer, when someone clicked to 20 or 40 button
	QPair<Player*, int> *mWaitingMarriage;
	//
	
	bool mTwentyButtonClickedThisTurn;
	bool mFortyButtonClickedThisTurn;
	
	//Closing deck
	bool mClickedToCloseButtonThisTurn;
	
	/* The closer fails and before he/she closed the deck:
	 *   -the opponent had got tricks, the opponent get 2 scores
	 *   -the opponent hadn't got tricks, the opponent get 3 scores
	 */
	bool mOpponentHaveNotTricksBeforePlayerClickedToCloseButton;
	
	Player *mPlayerWhoClickedToCloseButtonThisRound;
	
	//
	QList<Player *> mPlayerListWhoWantResumeGame;
	QList<Player *> mPlayerListWhoWantNewGame;
	//
	
	void newGame();
	void newRound();
	void roundOver();
	
private slots:
	void slotNewPlayer( Player* );
	void slotPlayerDisconnected( Player* );
	
	//void slotPlayerSelectedCard( Card, int );
	//
	void slotPlayerSelectedCard( Card*, int );
	//
	void slotPlayerTwentyButtonClicked();
	void slotPlayerFortyButtonClicked();
	void slotPlayerClickedToCloseButton();
	//void slotPlayerChangedTrumpCard( Card );
	void slotPlayerChangedTrumpCard( Player* );
	
	void slotCheckCentralCards();
	
	//
	void slotPlayerWantStartNextRound( Player* );
	void slotPlayerWantStartNextGame( Player* );
	//
	
protected:
	void incomingConnection(int);

public:
    Server(QObject* parent = 0);
	~Server();

	//Set who start the game, the first round
	//void setWhoStartGame( Knapsen::WhoStartGame whoStartGame ){ mWhoStartGame = whoStartGame; }
	void setWhoStartGame( Knapsen::WhoStartGame whoStartGame );
	
	//Set the cards in hand, 5 or 6
	void setNumberOfCardsInHand( int numberOfCardsInHand ){ mNumberOfCardsInHand = numberOfCardsInHand; }
	
	//Set the size of deck, 20 or 24
	//void setSizeOfDeck( SizeOfDeck sizeOfDeck ){ mSizeOfDeck = sizeOfDeck; }
	void setSizeOfDeck( int sizeOfDeck ){ mSizeOfDeck = sizeOfDeck; }
	
	//Set type of cards, german suits or french suits
	void setTypeOfCards( Knapsen::TypeOfCards typeOfCards ){ mTypeOfCards = typeOfCards; }
	
	//
	void addBot( QString );
	//
	
	void startGame();
	
signals:
	void signalPlayerConnected( QString );
	void signalPlayerDisconnected( QString );
	void signalServerFull();
	void signalServerEmpty();
};

#endif //SERVER_H
