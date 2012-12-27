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

class Server : public QTcpServer
{
//public:
	//enum WhoStartGame{ AdminPlayer, OpponentPlayer, RandomPlayer };
	//enum SizeOfDeck{ Number20, Number24 };
	//enum CardsInHand{ Number5, Number6 };
	//enum TypeOfCards{ GermanSuits, FrenchSuits }; In enums.h
	
//private:
	
	Q_OBJECT
	
	GameSequence* mGameSequence;
	
	//QString mAdminName;
	
	//
	Bot *mBot;
	//
	
	//SizeOfDeck mSizeOfDeck;
	int mSizeOfDeck;
	
	//CardsInHand mCardsInHand;
	int mNumberOfCardsInHand;
	
	Knapsen::TypeOfCards mTypeOfCards;

	QList<Player*> mPlayerList;
	
	Deck *mDeck;
	
	//QString mTrumpCard;
	Card mTrumpCard;
	
	//QList<Card> mCentralCards;
	CentralCards *mCentralCards;
	
	//
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
	
	void slotPlayerSelectedCard( Card, int );
	void slotPlayerTwentyButtonClicked();
	void slotPlayerFortyButtonClicked();
	void slotPlayerClickedToCloseButton();
	void slotPlayerChangedTrumpCard( Card );
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
	
	//Set the admin name
	//void setAdminName( QString adminName ){ mAdminName = adminName; }

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
