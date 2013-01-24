#ifndef TRUMP_H
#define TRUMP_H

#include "card.h"

class Trump
{
    Card *mCard;
    
    Card::Suit mCardSuit;
    
public:
    Trump();
    
    void addNewCard( Card* );
    Card* getCard(){ return mCard; }
    
    Card::Suit getCardSuit(){ return mCardSuit; }
    
    bool isEmpty();
    
    void clearTrumpCard( bool );
    
};

#endif //TRUMP_CARD_H
