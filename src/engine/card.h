#ifndef CARD_H
#define CARD_H

#include <QtCore/QString>
//#include "enums.h"

class Card
{	
public:
	enum CardSuit{ Heart, Diamond, Spade, Club };
	enum CardType{ Ace, Ten, King, Queen, Jack, Nine };
	
private:
	int mValue;
	
	bool mSelectable;
	
public:
	static const int CARD_NUMBERS_WITHOUT_9 = 20;
	static const int CARD_NUMBERS_WITH_9 = 24;
	
	Card();
	Card( int );
	
	int getValue() const{ return mValue; }
	
	//bool isValid() const;
	
	void setSelectable( bool selectable ){ mSelectable = selectable; }
	
	bool isSelectable() const{ return mSelectable; }
	
	QString getCardText(/* Knapsen::TypeOfCards */) const;
	
	int getCardPoint() const;
	
	CardSuit getCardSuit() const;
	CardType getCardType() const;

	bool operator==( const Card& );
};

#endif //CARD_H
