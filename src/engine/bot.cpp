#include <QtCore/QTimer>
#include <KDE/KDebug>
#include "centralcards.h"
#include "bot.h"

Bot::Bot( QObject* parent ): 
	Client( parent ),
	deckIsClosed( false ) 
{
	kDebug() << "Initialize.";
	
	connect( this, SIGNAL( signalOpponentDisconnected() ),			this, SLOT( slotOpponentDisconnected() ) );
	connect( this, SIGNAL( signalPlayerInAction() ),				this, SLOT( slotPlayerInAction() ) );
	connect( this, SIGNAL( signalNewRound() ),						this, SLOT( slotNewRound() ) );
	//
	connect( this, SIGNAL( signalCloseDeck() ),						this, SLOT( slotCloseDeck() ) );
	//
	connect( this, SIGNAL( signalCentralCardChanged( int, Card ) ),	this, SLOT( slotCentralCardChanged( int, Card ) ) );
	connect( this, SIGNAL( signalEndRound( QString, int ) ),		this, SLOT( slotEndRound( QString, int ) ) );
	connect( this, SIGNAL( signalEndGame( QString ) ),				this, SLOT( slotendGame( QString ) ) );
	
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
	kDebug() << "Opponent disconnected. Disconnect me too.";
	
	disconnectFromHost();
}

void Bot::slotPlayerInAction()
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
	
	//if( getCentralCardsSize() == 0 ){
	if( getCentralCards()->isEmpty() ){
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
				
				if( card.isSelectable() && card.getCardType() == Card::King ){
					
					slotSelectedCardId( i );
				}
				
			}
			
			
		}else{ //clicktedToFortyOrTwentyButton == false
			kDebug() << "Don't clicked to forty or twenty button.";
			
			Card::CardSuit trumpCardSuit = getTrumpCard().getCardSuit();
			
			kDebug() << "If bot have ace, and the type is not equal with type of trump card then select ace.";
			for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
				
				Card card = getCard( i );
				
				if( card.isSelectable() && ( card.getCardSuit() != trumpCardSuit ) && ( card.getCardType() == Card::Ace ) ){
					slotSelectedCardId( i );
					return;
				}
				
			}
			
			kDebug() << "If bot have ten, and the type is not equal type of trump card then select ten.";
			for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
				
				Card card = getCard( i );
				
				if( card.isSelectable() && ( card.getCardSuit() != trumpCardSuit ) && ( card.getCardType() == Card::Ten ) ){
					slotSelectedCardId( i );
					return;
				}
				
			}
			
			kDebug() << "If bot have king, and the pair of king was in the central cards _or_ the deck of cards count 0 then select king.";
			for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
				
				Card card = getCard( i );
				
				if( card.isSelectable() && ( card.getCardType() == Card::King ) && ( getSizeOfDeckNow() == 0 || getPairOfKingWasInCentralCards( card.getCardSuit() ) ) ){
					slotSelectedCardId( i );
					return;
				}
				
			}
			
			kDebug() << "If bot have queen, and the pair of queen was in the central cards _or_ the deck of cards count 0 then select queen.";
			for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
				
				Card card = getCard( i );
				
				if( card.isSelectable() && ( card.getCardType() == Card::Queen ) && ( getSizeOfDeckNow() == 0 || getPairOfQueenWasInCentralCards( card.getCardSuit() ) ) ){
					slotSelectedCardId( i );
					return;
				}
				
			}
			
			kDebug() << "If bot have ace, (and the type is equal the type of trump card) then select ace.";
			for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
				
				Card card = getCard( i );
				
				if( card.isSelectable() && card.getCardType() == Card::Ace ){
					slotSelectedCardId( i );
					return;
				}
				
			}
			
			kDebug() << "If bot have ten, (and the type is equal the type of trump card) then select ten.";
			for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
				
				Card card = getCard( i );
				
				if( card.isSelectable() && card.getCardType() == Card::Ten ){
					slotSelectedCardId( i );
					return;
				}
			}
			
			kDebug() << "If bot have king, and have not pair of king, then select king.";
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
			
			kDebug() << "If bot have jack, then select jack.";
			for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
				Card card = getCard( i );
				
				if( card.isSelectable() && card.getCardType() == Card::Jack ){
					slotSelectedCardId( i );
					return;
				}
			}
			
			if( getLowestCardType() == Card::Nine ){
				kDebug() << "If bot have nine, then select nine.";
				
				for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
					Card card = getCard( i );
					
					if( card.isSelectable() && card.getCardType() == Card::Nine ){
						slotSelectedCardId( i );
						return;
					}
				}
			}
			
			kDebug() << "Finally select anything.";
			for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
				
				if( getCard( i ).isSelectable() ){
					slotSelectedCardId( i );
					return;
				}
			}
			
		}
		
		
	//}else{ // getCentralCardsSize == 1
	}else{ // !getCentralCard()->isEmpty()
		kDebug() << "Central cards size: 1";
		
		//Card centralCard = getCentralCard( 0 );
		Card centralCard0 = getCentralCards()->getCard( 0 );
		Card card;
		
		kDebug() << "-Firstly try select the card, which points of card bigger then points of central card (and the suits of cards are equal).";
		for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
			
			card = getCard( i );
			
			if( card.isSelectable() && card.getCardSuit() == centralCard0.getCardSuit() && card.getCardPoint() > centralCard0.getCardPoint() ){
				
				if( getSizeOfDeckNow() == 0 || deckIsClosed ){
					//If size of deck now is 0 or closed, then can select king or queen card too
					
					slotSelectedCardId( i );
					return;
					
				}else{ //getSizeOfDeckNow() != 0 || deckIsClosed == false
					//If size of deck is not 0 or closed, then can't select king or queen if these' pairs wasn't in the central card.
					
					if( ( card.getCardType() == Card::King && getPairOfKingWasInCentralCards( card.getCardSuit() ) ) ||
						( card.getCardType() == Card::Queen && getPairOfQueenWasInCentralCards( card.getCardSuit() ) ) ||
						( card.getCardType() != Card::King && card.getCardType() != Card::Queen ) )
					{
						slotSelectedCardId( i );
						return;
					}

				}
				
				
				
			}
			
		}
		
		kDebug() << "-Secondly if the suit of central card is not equal with suit of trump card, then select the card, which suit is equal with suit of trump card.";
		//If suit of centralCard is not equal with suit of trump card
		if( getTrumpCard().getCardSuit() != centralCard0.getCardSuit() ){
			
			int selectedCardId = -1;
			
			for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
				card = getCard( i );
				
				if( card.isSelectable() && card.getCardSuit() == getTrumpCard().getCardSuit() ){
					
					if( getSizeOfDeckNow() == 0 || deckIsClosed ){
						
						if( !getCard( selectedCardId ).isValid() ){
							selectedCardId = i;
						}else{ // getCard( selectedCardId ).isValid()
							
							if( card.getCardType() < getCard( selectedCardId ).getCardType() ){
								selectedCardId = i;
							}
							
						}
						
					}else{
						
						if( ( card.getCardType() == Card::King && getPairOfKingWasInCentralCards( card.getCardSuit() ) ) ||
							( card.getCardType() == Card::Queen && getPairOfQueenWasInCentralCards( card.getCardSuit() ) ) ||
							( card.getCardType() != Card::King && card.getCardType() != Card::Queen ) )
						{
							//selectedCardId = i;
							if( !getCard( selectedCardId ).isValid() ){
								selectedCardId = i;
							}else{ // getCard( selectedCardId ).isValid()
							
								if( card.getCardType() < getCard( selectedCardId ).getCardType() ){
									selectedCardId = i;
								}
							
							}
							
						}
						
					}
					
				}
				
			}
			
			if( selectedCardId != -1 ){
				slotSelectedCardId( selectedCardId );
				return;
			}
			
		}
		
		kDebug() << "Thirdly, I cannot win, i'm selecting a smallest points of card.";
		int selectedCardId = -1;
		
		for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
			Card card = getCard( i );
			
			if( card.isSelectable() ){
				
				if( selectedCardId == -1 ){
					selectedCardId = i;
				}else{ // selectedCardId != -1
					
					if( getSizeOfDeckNow() == 0 || deckIsClosed ){
						
						if( card.getCardType() < getCard( selectedCardId ).getCardType() ){
							selectedCardId = i;
						} 
						
					}else{ // getSizeOfDeckNow() != 0 && deckIsClosed == false
						
						if( ( card.getCardType() == Card::King && getPairOfKingWasInCentralCards( card.getCardSuit() ) ) ||
							( card.getCardType() == Card::Queen && getPairOfQueenWasInCentralCards( card.getCardSuit() ) ) ||
							( card.getCardType() != Card::King && card.getCardType() != Card::Queen ) )
						{
							
							if( card.getCardType() < getCard( selectedCardId ).getCardType() ){
								selectedCardId = i;
							}
							
						}
						
					}
					
				}
				
			}
			
		}
		
		slotSelectedCardId( selectedCardId );
		
	}
	
}

void Bot::slotNewRound()
{
	deckIsClosed = false;
	
	pairOfQueenWasInCentralCards[0].second = pairOfKingWasInCentralCards[0].second = false;
	pairOfQueenWasInCentralCards[1].second = pairOfKingWasInCentralCards[1].second = false;
	pairOfQueenWasInCentralCards[2].second = pairOfKingWasInCentralCards[2].second = false;
	pairOfQueenWasInCentralCards[3].second = pairOfKingWasInCentralCards[3].second = false;
}

void Bot::slotCloseDeck()
{
	deckIsClosed = true;
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
