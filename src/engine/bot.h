#ifndef BOT_H
#define BOT_H

#include "enums.h"
#include "client.h"

class Bot : public Client
{
	Q_OBJECT
	
	Knapsen::GameDifficulty mDifficulty;
	
	bool mDeckIsClosed;
	
	//
	QPair< Card::CardSuit, bool > pairOfKingWasInCentralCards[4];
	QPair< Card::CardSuit, bool > pairOfQueenWasInCentralCards[4];
	
	//bool getPairOfKingWasInCentralCards( Card::CardSuit );
	//bool getPairOfQueenWasInCentralCards( Card::CardSuit );
	bool getPairOfCardWasInCentral( Card::CardType, Card::CardSuit );
        //
	
private slots:
	//void slotOpponentDisconnected();
	//void slotGameError( Client::GameErrorType );
	
	void slotPlayerInAction();
	
	void slotSelectCard();
        
        void easy();
        void medium();
        void hard();
        
        int getRandomCardId();
        
        //
        void tryChangeTrumpCard();
        bool tryMeldMarriage();
        
        bool trySelectNotEqualTrump( Card::CardType );
        bool trySelectPairWasNotInCentral( Card::CardType ); //Only King or Queen
        bool trySelectEqualTrump( Card::CardType );
        //
        //bool trySelectPairHaveNotInHand( Card::CardType ); //Only King or Queen
        //
        bool trySelect( Card::CardType );
        //
        
        //
        bool trySelectEqualCentralNotEqualTrumpMinPoints();
        //
	
	void slotNewRound();
	
	void slotCloseDeck(){ mDeckIsClosed = true; }
	
	//void slotCentralCardChanged( int, Card );
	//void slotCentralCardChanged( int, Card* );
	void slotNewCentralCard( int, Card* );
	
	//void slotEndRound( QString, int );
	
	//void slotendGame( QString );
	
public:
    Bot( QString, Knapsen::GameDifficulty );
	
};

#endif //BOT_H
