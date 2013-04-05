#ifndef BOT_H
#define BOT_H

#include "enums.h"
#include "client.h"

class Bot : public Client
{
    Q_OBJECT

    Knapsen::GameDifficulty mDifficulty;

    bool mDeckIsClosed;

    QPair< Card::Suit, bool > pairOfKingWasInCentralCards[4];
    QPair< Card::Suit, bool > pairOfQueenWasInCentralCards[4];
    
    bool getPairOfCardWasInCentral( Card::Type, Card::Suit );
    
    void easy();
    void medium();
    void hard();
    
    int getRandomCardId();
    
    void tryChangeTrumpCard();
    bool tryMeldMarriage();
    
    bool trySelectNotEqualTrump( Card::Type );
    bool trySelectPairWasNotInCentral( Card::Type ); //Only King or Queen
    bool trySelectEqualTrump( Card::Type );
    bool trySelect( Card::Type );
    
    bool trySelectEqualCentralMinPoints();
    bool trySelectEqualTrumpMinPoints();

private slots:
    void slotSelectCard();
    
    void slotNewRound();
    
    void slotCloseDeck(){ mDeckIsClosed = true; }
    
    void slotNewCentralCard( const Card* );
    
    void slotShowOpponentCards( int, Card, int, Card );
    
    void slotEndRound( QString, int );
    void slotendGame( QString );

public:
    Bot( QString, Knapsen::GameDifficulty );

};

#endif //BOT_H
