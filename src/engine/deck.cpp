#include <KDE/KDebug>
#include "deck.h"


const int CARD_NUMBERS = 24;

const QString GERGMAN_SUIT_CARD_NAME[ CARD_NUMBERS ] = {
	"1_heart", "king_heart", "queen_heart", "jack_heart", "10_heart", "9_heart", //heart
	"1_diamond", "king_diamond", "queen_diamond", "jack_diamond", "10_diamond", "9_diamond", //diamond
	"1_spade", "king_spade", "queen_spade", "jack_spade", "10_spade", "9_spade", //spade
	"1_club", "king_club", "queen_club", "jack_club" , "10_club", "9_club" //club
};

Deck::Deck( int deckSize, Knapsen::TypeOfCards typeOfCards )
{
	kDebug() << "Deck size:" << deckSize;
	if( typeOfCards == Knapsen::GermanSuits ){
		kDebug() << "Type of cards: German suits";
	}else{
		kDebug() << "Type of cards: French suits";
	}
	
	mDeckSize = deckSize;
	mTypeOfCards = typeOfCards;
}

void Deck::buildDeck()
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
	
	mCards[19] = Card( 1 );
	mCards[17] = Card( 2 );
	//mCards[13] = Card( 3 );
	
	kDebug() << "Generated deck:";
	for( int i = 0; i < mCards.size(); ++i ){
		kDebug() << i << ":" << mCards.at( i ).getValue();
	}
}
