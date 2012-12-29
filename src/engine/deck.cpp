#include <KDE/KDebug>
#include "deck.h"


const int CARD_NUMBERS = 24;

/*Deck::Deck( int deckSize, Knapsen::TypeOfCards typeOfCards )
{
	kDebug() << "Deck size:" << deckSize;
	if( typeOfCards == Knapsen::GermanSuits ){
		kDebug() << "Type of cards: German suits";
	}else{
		kDebug() << "Type of cards: French suits";
	}
	
	mDeckSize = deckSize;
	mTypeOfCards = typeOfCards;
}*/

Deck::Deck( int deckSize )
{
	kDebug() << "Deck size:" << deckSize;
	
	mDeckSize = deckSize;
}

/*void Deck::buildDeck()
{
	kDebug() << "Build deck.";
	
	Card card;
	
	while( mCards.size() != mDeckSize ){
	
		if( mDeckSize == Card::CARD_NUMBERS_WITHOUT_9 ){
			card = Card( qrand()%Card::CARD_NUMBERS_WITHOUT_9 );
		}else{ //mDeckSize == Card::CARD_NUMBERS_WITH_9
			card = Card( qrand()%Card::CARD_NUMBERS_WITH_9 );
		}
		
		bool inList = false;
		for( int i = 0; i < mCards.size(); ++i ){
			if( card == mCards.at( i ) ){
				inList = true;
			}
		}
		
		if( !inList ){
			mCards.append( card );
		}
	}
	
	kDebug() << "Generated deck:";
	for( int i = 0; i < mCards.size(); ++i ){
		kDebug() << i << ":" << mCards.at( i ).getValue();
	}
}*/

void Deck::buildDeck()
{
	kDebug() << "Build deck.";
	
	
	while( mCards.size() != mDeckSize ){
		Card *card;
		
		if( mDeckSize == Card::CARD_NUMBERS_WITHOUT_9 ){
			card = new Card( qrand()%Card::CARD_NUMBERS_WITHOUT_9 );
		}else{ //mDeckSize == Card::CARD_NUMBERS_WITH_9
			card = new Card( qrand()%Card::CARD_NUMBERS_WITH_9 );
		}
		
		if( !mCards.contains( card ) ){
			mCards.append( card );
		}else{ //mCards.contains( card )
			delete card;
		}
		
		kDebug() << "Generated deck:";
		for( int i = 0; i < mCards.size(); ++i ){
			kDebug() << i << ":" << mCards.at( i )->getValue();
		}
	}
}
