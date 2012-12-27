//#include <KDE/KDebug>
#include "centralcards.h"

const int MAX_CENTRALCARDS = 2;

CentralCards::CentralCards()
{
	mCards = new QList<Card>;
}

CentralCards::~CentralCards()
{
	delete mCards;
	mCards = 0;
}

/*void CentralCards::add( Card card )
{
	mCards->append( card );
}*/

int CentralCards::add( Card card )
{
	mCards->append( card );

	return mCards->size()-1;
}

void CentralCards::clear()
{
	mCards->clear();
}

/*int CentralCards::getSize()
{
	return mCards->size();
}*/

bool CentralCards::isEmpty()
{
	if( mCards->size() == 0 ){
		return true;
	}//else
	return false;
}

bool CentralCards::isFull()
{
	if( mCards->size() == MAX_CENTRALCARDS ){
		return true;
	}//else
	return false;
}

Card CentralCards::getCard( int pos )
{
	return mCards->at( pos );
}
