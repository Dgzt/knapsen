#ifndef CENTRALCARDS_H
#define CENTRALCARDS_H

#include <QtCore/QList>
#include "card.h"

class CentralCards
{
	QList<Card> *mCards;

public:
	CentralCards();
	~CentralCards();
	
	void add( Card );
	void clear();
	
	bool isFull();
	
	Card at( int );
	
};

#endif //CENTRALCARDS_H
