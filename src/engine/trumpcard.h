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
	
	//Don't delete the mCard data, when add the card to player when the deck is empty.
	void clearTrumpCard();
	//Delete the mCard data, when at the player clear the trump card.
	void clearTrumpCard( bool );
	
};

#endif //TRUMP_CARD_H
