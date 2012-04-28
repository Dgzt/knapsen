#ifndef DECK_H
#define DECK_H

#include <QtCore/QString>
#include <QtCore/QList>
#include "card.h"
#include "enums.h"

class Deck
{
	int mDeckSize;
	
	//QList<QString> mCards;
	QList<Card> mCards;
	
	Knapsen::TypeOfCards mTypeOfCards;
	
	//int randomCardNumber();
	
public:
	Deck( int, Knapsen::TypeOfCards );
	
	void buildDeck();
	
	int getDeckSize() const{ return mCards.size(); }
	
	Card getCard(){ return mCards.takeLast(); }

};

#endif //DECK_H
