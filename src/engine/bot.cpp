#include <QtCore/QTimer>
#include <KDE/KDebug>
#include "bot.h"

Bot::Bot( QObject* parent ): Client( parent )
{
	kDebug() << "Initialize.";
	
	connect( this, SIGNAL( signalOpponentDisconnected() ),		this, SLOT( slotOpponentDisconnected() ) );
	connect( this, SIGNAL( signalInAction() ),					this, SLOT( slotInAction() ) );
	//
	connect( this, SIGNAL( signalNewRound() ),					this, SLOT( slotNewRound() ) );
	connect( this, SIGNAL( signalCentralCardChanged( int, Card ) ),	this, SLOT( slotCentralCardChanged( int, Card ) ) );
	//
	connect( this, SIGNAL( signalEndRound( QString, int ) ),	this, SLOT( slotEndRound( QString, int ) ) );
	connect( this, SIGNAL( signalEndGame( QString ) ),			this, SLOT( slotendGame( QString ) ) );
	
	//
	pairOfQueenWasInCentralCards[0].first = pairOfKingWasInCentralCards[0].first = Card::Heart;
	pairOfQueenWasInCentralCards[1].first = pairOfKingWasInCentralCards[1].first = Card::Diamond;
	pairOfQueenWasInCentralCards[2].first = pairOfKingWasInCentralCards[2].first = Card::Spade;
	pairOfQueenWasInCentralCards[3].first = pairOfKingWasInCentralCards[3].first = Card::Club;
}

bool Bot::getPairOfKingWasInCentralCards( Card::CardSuit cardSuit )
{
	for( int i = 0; i < 4; ++i ){
		if( pairOfKingWasInCentralCards[i].first == cardSuit ){
			return pairOfKingWasInCentralCards[i].second;
		}
	}
	
	return false;
}

bool Bot::getPairOfQueenWasInCentralCards( Card::CardSuit cardSuit )
{
	for( int i = 0; i < 4; ++i ){
		if( pairOfQueenWasInCentralCards[i].first == cardSuit ){
			return pairOfQueenWasInCentralCards[i].second;
		}
	}
	
	return false;
}

void Bot::slotOpponentDisconnected()
{
	kDebug() << "Opponent disconnected. Me too disconnect.";
	
	disconnectFromHost();
}

void Bot::slotInAction()
{
	QTimer::singleShot( 1000, this, SLOT( slotSelectCard() ) );
}

void Bot::slotSelectCard()
{
	kDebug() << "In Action.";
	
	// TMP
	/*for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
		if( getCard( i ).isValid() && getCard( i ).isSelectable() ){
			slotSelectedCardId( i );
			return;
		}
	}*/
	// /TMP
	
	if( getCentralCardsSize() == 0 ){
		kDebug() << "Central cards size: 0";
		
		//If the bot can change card, then change card
		if( getTrumpCard().isSelectable() ){
			kDebug() << "Can change trump card. Change it.";
			
			slotSelectedTrumpCard();
		}
		
		bool clicktedToFortyOrTwentyButton = false;
		
		//If have forty marriage, meld this
		if( isFortyButtonVisible() ){
			kDebug() << "Click to forty button.";
			
			slotFortyButtonClicked();
			clicktedToFortyOrTwentyButton = true;
		}
		
		//If have twenty marriage, meld this
		if( isTwentyButtonVisible() ){
			kDebug() << "Click to twenty button.";
			
			slotTwentyButtonClicked();
			clicktedToFortyOrTwentyButton = true;
		}
		
		//If clicked to forty or twenty button then add the king to the central cards
		if( clicktedToFortyOrTwentyButton ){
			kDebug() << "Select the king.";
			
			for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
				
				Card card = getCard( i );
				
				if( card.isValid() && card.isSelectable() && card.getCardType() == Card::King ){
					
					slotSelectedCardId( i );
				}
				
			}
			
			
		}else{ //clicktedToFortyOrTwentyButton == false
			kDebug() << "Don't clicked to forty or twenty button.";
			
			Card::CardSuit trumpCardSuit = getTrumpCard().getCardSuit();
			
			kDebug() << "If bot have ace, and the type is not equal with type of trump card.";
			for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
				
				Card card = getCard( i );
				
				if( card.isSelectable() && ( card.getCardSuit() != trumpCardSuit ) && ( card.getCardType() == Card::Ace ) ){
					slotSelectedCardId( i );
					return;
				}
				
			}
			
			kDebug() << "If bot have ten, and the type is not equal type of trump card.";
			for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
				
				Card card = getCard( i );
				
				if( card.isSelectable() && ( card.getCardSuit() != trumpCardSuit ) && ( card.getCardType() == Card::Ten ) ){
					slotSelectedCardId( i );
					return;
				}
				
			}
			
			kDebug() << "If bot have king, and the pair of king was in the central cards _or_ the deck of cards count 0, put the king.";
			for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
				
				Card card = getCard( i );
				
				if( card.isSelectable() && ( card.getCardType() == Card::King ) && ( getSizeOfDeckNow() == 0 || getPairOfKingWasInCentralCards( card.getCardSuit() ) ) ){
					slotSelectedCardId( i );
					return;
				}
				
			}
			
			kDebug() << "If bot have queen, and the pair of queen was in the central cards _or_ the deck of cards count 0, put the over.";
			for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
				
				Card card = getCard( i );
				
				if( card.isSelectable() && ( card.getCardType() == Card::Queen ) && ( getSizeOfDeckNow() == 0 || getPairOfQueenWasInCentralCards( card.getCardSuit() ) ) ){
					slotSelectedCardId( i );
					return;
				}
				
			}
			
			kDebug() << "If bot have ace, (and the type is equal the type of trump card) put ace.";
			for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
				
				Card card = getCard( i );
				
				if( card.isSelectable() && card.getCardType() == Card::Ace ){
					slotSelectedCardId( i );
					return;
				}
				
			}
			
			kDebug() << "If bot have ten, (and the type is equal the type of trump card) put ten.";
			for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
				
				Card card = getCard( i );
				
				if( card.isSelectable() && card.getCardType() == Card::Ten ){
					slotSelectedCardId( i );
					return;
				}
			}
			
			kDebug() << "If bot have king, and have not pair of king, then put the king.";
			for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
				
				Card card = getCard( i );
				
				if( card.isSelectable() && card.getCardType() == Card::King ){
					
					bool ok = true;
					
					for( int j = 0; j < getNumberOfCardsInHand(); ++j ){
						
						if( getCard( j ).isValid() && getCard( j ).getCardSuit() == card.getCardSuit() && getCard( j ).getCardType() == Card::Queen ){
							ok = false;
						}
						
					}
					
					if( ok ){
						slotSelectedCardId( i );
						return;
					}
					
				}
				
			}
			
			kDebug() << "If bot have queen, and have not pair of queen, then put the queen.";
			for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
				
				Card card = getCard( i );
				
				if( card.isSelectable() && card.getCardType() == Card::Queen ){
					
					bool ok = true;
					
					for( int j = 0; j < getNumberOfCardsInHand(); ++j ){
						
						if( getCard( j ).isValid() && getCard( j ).getCardSuit() == card.getCardSuit() && getCard( j ).getCardType() == Card::King ){
							ok = false;
						}
						
					}
					
					if( ok ){
						slotSelectedCardId( i );
						return;
					}
					
				}
				
			}
			
			kDebug() << "Finally put everithing.";
			for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
				
				if( getCard( i ).isSelectable() ){
					slotSelectedCardId( i );
					return;
				}
			}
			
			
		}
		
		
	}else{ // getCentralCardsSize == 1
		kDebug() << "Central cards size: 1";
		
	}
	
}

void Bot::slotNewRound()
{
	pairOfQueenWasInCentralCards[0].second = pairOfKingWasInCentralCards[0].second = false;
	pairOfQueenWasInCentralCards[1].second = pairOfKingWasInCentralCards[1].second = false;
	pairOfQueenWasInCentralCards[2].second = pairOfKingWasInCentralCards[2].second = false;
	pairOfQueenWasInCentralCards[3].second = pairOfKingWasInCentralCards[3].second = false;
}

void Bot::slotCentralCardChanged( int , Card card )
{
	if( card.getCardType() == Card::Queen ){
		
		for( int i = 0; i < 4; ++i ){
			
			if( pairOfKingWasInCentralCards[i].first == card.getCardSuit() ){
				pairOfKingWasInCentralCards[i].second = true;
			}
			
		}
		
	}else if( card.getCardType() == Card::King ){
		
		for( int i = 0; i < 4; ++i ){
			
			if( pairOfQueenWasInCentralCards[i].first == card.getCardSuit() ){
				pairOfQueenWasInCentralCards[i].second = true;
			}
			
		}
		
	}
}

void Bot::slotEndRound( QString /*roundWinnerName*/ , int /*scores*/ )
{
	startNextRound();
}

void Bot::slotendGame( QString /*gameWinnerName*/ )
{
	startNextGame();
}
