#include "trumpcard.h"

TrumpCard::TrumpCard()
{
	mCard = 0;
}

/*void TrumpCard::addNewTrumpCard( Card *card )
{
	mCard = card;
	
	mCardSuit = mCard->getCardSuit();
}*/

void TrumpCard::addNewCard( Card *card )
{
	mCard = card;
	
	mCardSuit = mCard->getCardSuit();
}

/*void TrumpCard::setSelectable( const bool selectable )
{
	mCard->setSelectable( selectable );
}*/

/*bool TrumpCard::isSelectable()
{
	return mCard->isSelectable();
}*/

/*QString TrumpCard::getCardText()
{
	return mCard->getCardText();
}*/

bool TrumpCard::isEmpty()
{
	if( mCard == 0 ){
		return true;
	}//else
	return false;
}

/*void TrumpCard::clearTrumpCard()
{
	delete mCard;
	mCard = 0;
}*/

void TrumpCard::clearTrumpCard()
{
	mCard = 0;
}

void TrumpCard::clearTrumpCard( bool removeData )
{
	Q_UNUSED( removeData );
	
	delete mCard;
	clearTrumpCard();
}
