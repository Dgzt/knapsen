#include "trump.h"

Trump::Trump()
{
	mCard = 0;
}

void Trump::addNewCard( Card *card )
{
	mCard = card;
	
	mCardSuit = mCard->getCardSuit();
}

bool Trump::isEmpty()
{
	if( mCard == 0 ){
		return true;
	}//else
	return false;
}

void Trump::clearTrumpCard( bool removeData )
{
	if( removeData ){
		delete mCard;
	}
	
	mCard = 0;
}
