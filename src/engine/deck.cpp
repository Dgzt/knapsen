#include <KDE/KDebug>
#include "deck.h"


const int CARD_NUMBERS = 24;

Deck::Deck( int deckSize )
{
	kDebug() << "Deck size:" << deckSize;
	
	mDeckSize = deckSize;
}

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
		
		bool inList = false;
		for( int i = 0; i < mCards.size(); ++i ){
			if( mCards.at(i)->getValue() == card->getValue() ){
				inList = true;
			}
		}
		
		if( !inList ){
			mCards.append( card );
		}else{
			delete card;
		}
		
	}
	
	kDebug() << "Generated deck:";
	for( int i = 0; i < mCards.size(); ++i ){
		kDebug() << i << ":" << mCards.at( i )->getValue();
	}
}
