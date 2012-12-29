#ifndef DECK_H
#define DECK_H

#include <QtCore/QString>
#include <QtCore/QList>
#include "card.h"
#include "enums.h"

class Deck
{
	int mDeckSize;
	
	//QList<Card> mCards;
	QList<Card*> mCards;
	
	Knapsen::TypeOfCards mTypeOfCards;
		
public:
	//Deck( int, Knapsen::TypeOfCards );
	Deck( int );
	
	void buildDeck();
	
	int getDeckSize() const{ return mCards.size(); }
	
	//Card getCard(){ return mCards.takeLast(); }
	Card* getCard(){ return mCards.takeLast(); }
	
};

#endif //DECK_H
