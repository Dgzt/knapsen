#ifndef CLIENT_H
#define CLIENT_H

#include "player.h"
#include "enums.h"

class CentralCards;
	
class TrumpCard;

class Client : public Player
{
	Q_OBJECT
	
	int mSizeOfDeck;
	int mSizeOfDeckNow;
	
	CentralCards *mCentralCards;
	TrumpCard *mTrumpCard;
	
	//
	QList<QString> commandList;
	//
	
	virtual void newCommand( QString );
	
private slots:
	void slotConnected();
	
protected:
	//
	int getSizeOfDeckNow(){ return mSizeOfDeckNow; }
	//
	
	CentralCards* getCentralCards(){ return mCentralCards; }
	
	TrumpCard* getTrumpCard(){ return mTrumpCard; }
	
public:
    enum GameErrorType{ NameIsBusy, ServerIsFull, OpponentDisconnected };
	
	//Client( QObject* parent = 0 );
    Client( QString );
    /*virtual */~Client();
	
	void startNextRound(){ sendCommand( START_NEXT_ROUND_COMMAND ); }
	void startNextGame(){ sendCommand( START_NEXT_GAME_COMMAND ); }
	
public slots:
	void slotSelectedCardId( int );
	void slotSelectedTrumpCard();
	void slotTwentyButtonClicked();
	void slotFortyButtonClicked();
	void slotCloseButtonClicked();
	//
	void slotProcessCommands();
	//
	
signals:
	//Error signals
	//void signalClientError();
	void signalGameError( Client::GameErrorType );
	//
	
	void signalInitialize( QString, QString, Knapsen::TypeOfCards, int );
	
	void signalNewOpponentCardId( int );
	
	void signalNewPlayerCard( int, QString );
	
	void signalNewTrumpCard( QString );
	
	void signalNewCentralCard( int, QString );
	
	void signalClearCentralCards();
	
	void signalOpponentSelectedCardId( int );
	
	//void signalOpponentDisconnected();
	
	void signalOpponentTricksChanged( int );
	
	void signalOpponentScoresChanged( int );
	
	void signalPlayerTricksChanged( int );
	
	void signalPlayerScoresChanged( int );
	
	//void signalDeckVisible( bool );
	void signalHideDeck();
	
	void signalCloseDeck();
	
	void signalTrumpCardHide();
	//
	void signalTrumpCardSelectableChanged( bool );
	//
	
	void signalShowOpponentCards( int, QString, int, QString );
	
	//
	void signalNewRound();
	//
	
	void signalPlayerInAction();
	//
	void signalOpponentInAction();
	//
	
	void signalStartGame();
	
	//
	void signalEndRound( QString, int );
	//
	
	//
	void signalEndGame( QString );
	//
	
};	
	
#endif //CLIENT_H
