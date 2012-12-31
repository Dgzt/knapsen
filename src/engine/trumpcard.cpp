#include "trumpcard.h"

TrumpCard::TrumpCard()
{
	mCard = 0;
}

void TrumpCard::addNewCard( Card *card )
{
	mCard = card;
	
	mCardSuit = mCard->getCardSuit();
}

bool TrumpCard::isEmpty()
{
	if( mCard == 0 ){
		return true;
	}//else
	return false;
}

void TrumpCard::clearTrumpCard( bool removeData )
{
	if( removeData ){
		delete mCard;
	}
	
	mCard = 0;
}
