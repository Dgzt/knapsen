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

void CentralCards::add( Card card )
{
	mCards->append( card );
}

void CentralCards::clear()
{
	mCards->clear();
}

bool CentralCards::isFull()
{
	if( mCards->size() == MAX_CENTRALCARDS ){
		return true;
	}//else
	return false;
}

Card CentralCards::at( int pos )
{
	return mCards->at( pos );
}
