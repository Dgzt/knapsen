#ifndef CLIENT_H
#define CLIENT_H

#include "player.h"
#include "enums.h"
	
class Client : public Player
{
	Q_OBJECT
	
	QString mOpponentName;
	
	Knapsen::TypeOfCards mTypeOfCards;
	
	int mSizeOfDeck;
	int mSizeOfDeckNow;
	
	//
	QList<QString> commandList;
	//
	
	virtual void newCommand( QString );
	
private slots:
	void slotConnected();
	
	//void slotNewCentralCard( int, Card );
	void slotCentralCardChanged( int, Card );
	
public:
    Client( QObject* parent = 0 );
	
	void startNextRound(){ sendCommand( START_NEXT_ROUND_COMMAND ); }
	
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
	void signalInitialize( QString, QString, Knapsen::TypeOfCards, int );
	
	void signalNewOpponentCardId( int );
	
	void signalNewPlayerCard( int, QString );
	
	void signalNewTrumpCard( QString );
	
	void signalCentralCardChanged( int, QString );
	
	void signalOpponentSelectedCardId( int );
	
	void signalOpponentDisconnected();
	
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
	
	void signalStartGame();
	
	//
	void signalEndRound( QString, int );
	//
	
	//For bot
	void signalInAction();

};	
	
#endif //CLIENT_H
