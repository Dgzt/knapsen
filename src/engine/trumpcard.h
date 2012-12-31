#ifndef TRUMP_CARD_H
#define TRUMP_CARD_H

#include "card.h"

class TrumpCard
{
	Card *mCard;
	
	Card::CardSuit mCardSuit;
	
public:
	TrumpCard();
	
	void addNewCard( Card* );
	Card* getCard(){ return mCard; }
	
	Card::CardSuit getCardSuit(){ return mCardSuit; }
	
	bool isEmpty();
	
	void clearTrumpCard( bool );
	
};

#endif //TRUMP_CARD_H
