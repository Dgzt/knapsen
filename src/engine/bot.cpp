#include <QtCore/QTimer>
#include <KDE/KDebug>
#include "centralcards.h"
#include "trumpcard.h"
#include "bot.h"

Bot::Bot( QString name, Knapsen::GameDifficulty difficulty ):
	Client( name ),
	mDifficulty( difficulty ),
	mDeckIsClosed( false ) 
{
	kDebug() << "Initialize.";
	
	//connect( this, SIGNAL( signalOpponentDisconnected() ),			this, SLOT( slotOpponentDisconnected() ) );
	//connect( this, SIGNAL( signalGameError( Client::GameErrorType ) ),	this, SLOT( slotGameError( Client::GameErrorType ) ) );
	connect( this, SIGNAL( signalPlayerInAction() ),					this, SLOT( slotPlayerInAction() ) );
	connect( this, SIGNAL( signalNewRound() ),							this, SLOT( slotNewRound() ) );
	connect( this, SIGNAL( signalCloseDeck() ),							this, SLOT( slotCloseDeck() ) );
	connect( this, SIGNAL( signalNewCentralCard( int , Card* ) ),                  this, SLOT( slotNewCentralCard( int, Card* ) ) );
	//connect( this, SIGNAL( signalEndRound( QString, int ) ),			this, SLOT( slotEndRound( QString, int ) ) );
	//connect( this, SIGNAL( signalEndGame( QString ) ),					this, SLOT( slotendGame( QString ) ) );
	
	//
	pairOfQueenWasInCentralCards[0].first = pairOfKingWasInCentralCards[0].first = Card::Heart;
	pairOfQueenWasInCentralCards[1].first = pairOfKingWasInCentralCards[1].first = Card::Diamond;
	pairOfQueenWasInCentralCards[2].first = pairOfKingWasInCentralCards[2].first = Card::Spade;
	pairOfQueenWasInCentralCards[3].first = pairOfKingWasInCentralCards[3].first = Card::Club;
}

/*bool Bot::getPairOfKingWasInCentralCards( Card::CardSuit cardSuit )
{
	for( int i = 0; i < 4; ++i ){
		if( pairOfKingWasInCentralCards[i].first == cardSuit ){
			return pairOfKingWasInCentralCards[i].second;
		}
	}
	
	return false;
}*/

/*bool Bot::getPairOfQueenWasInCentralCards( Card::CardSuit cardSuit )
{
	for( int i = 0; i < 4; ++i ){
		if( pairOfQueenWasInCentralCards[i].first == cardSuit ){
			return pairOfQueenWasInCentralCards[i].second;
		}
	}
	
	return false;
}*/

bool Bot::getPairOfCardWasInCentral( Card::CardType cardType, Card::CardSuit cardSuit )
{
    for( int i = 0; i < 4; ++i ){
        if( cardType == Card::King && pairOfKingWasInCentralCards[i].first == cardSuit ){
            return pairOfKingWasInCentralCards[i].second;
        }
        if( cardType == Card::Queen && pairOfQueenWasInCentralCards[i].first == cardSuit ){
            return pairOfQueenWasInCentralCards[i].second;
        }
        
    }
    
    return false;
}

/*void Bot::slotGameError( Client::GameErrorType )
{
	kDebug() << getName() << "Game error.";
	
	disconnectFromHost();
}*/

void Bot::slotPlayerInAction()
{
	QTimer::singleShot( 1000, this, SLOT( slotSelectCard() ) );
}

void Bot::slotSelectCard()
{
	kDebug() << "In Action.";
	
	// TMP
	/*for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
		if( getCard( i ) != 0 && getCard( i )->isSelectable() ){
			slotSelectedCardId( i );
			return;
		}
	}*/
	// /TMP
	
	//if( getCentralCardsSize() == 0 ){
	/*( getCentralCards()->isEmpty() ){
		kDebug() << "Central cards size: 0";
		
		//If the bot can change card, then change card
		//if( getTrumpCard().isSelectable() ){
		//if( !getTrumpCard()->isEmpty() && getTrumpCard()->isSelectable() ){
		if( !getTrumpCard()->isEmpty() && getTrumpCard()->getCard()->isSelectable() ){
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
				
				//Card card = getCard( i );
				Card *card = getCard( i );
				
				//if( card.isSelectable() && card.getCardType() == Card::King ){
				if( card != 0 && card->isSelectable() && card->getCardType() == Card::King ){
					
					slotSelectedCardId( i );
				}
				
			}
			
			
		}else{ //clicktedToFortyOrTwentyButton == false
			kDebug() << "Don't clicked to forty or twenty button.";
			
			//Card::CardSuit trumpCardSuit = getTrumpCard().getCardSuit();
			Card::CardSuit trumpCardSuit = getTrumpCard()->getCardSuit();
			
			kDebug() << "If bot have ace, and the type is not equal with type of trump card then select ace.";
			for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
				
				//Card card = getCard( i );
				Card *card = getCard( i );
				
				//if( card.isSelectable() && ( card.getCardSuit() != trumpCardSuit ) && ( card.getCardType() == Card::Ace ) ){
				if( card != 0 && card->isSelectable() && ( card->getCardSuit() != trumpCardSuit ) && ( card->getCardType() == Card::Ace ) ){
					slotSelectedCardId( i );
					return;
				}
				
			}
			
			kDebug() << "If bot have ten, and the type is not equal type of trump card then select ten.";
			for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
				
				//Card card = getCard( i );
				Card *card = getCard( i );
				
				//if( card.isSelectable() && ( card.getCardSuit() != trumpCardSuit ) && ( card.getCardType() == Card::Ten ) ){
				if( card != 0 && card->isSelectable() && ( card->getCardSuit() != trumpCardSuit ) && ( card->getCardType() == Card::Ten ) ){
					slotSelectedCardId( i );
					return;
				}
				
			}
			
			kDebug() << "If bot have king, and the pair of king was in the central cards _or_ the deck of cards count 0 then select king.";
			for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
				
				//Card card = getCard( i );
				Card *card = getCard( i );
				
				//if( card.isSelectable() && ( card.getCardType() == Card::King ) && ( getSizeOfDeckNow() == 0 || getPairOfKingWasInCentralCards( card.getCardSuit() ) ) ){
				if( card != 0 && card->isSelectable() && ( card->getCardType() == Card::King ) && ( getSizeOfDeckNow() == 0 || getPairOfKingWasInCentralCards( card->getCardSuit() ) ) ){
					slotSelectedCardId( i );
					return;
				}
				
			}
			
			kDebug() << "If bot have queen, and the pair of queen was in the central cards _or_ the deck of cards count 0 then select queen.";
			for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
				
				//Card card = getCard( i );
				Card *card = getCard( i );
				
				if( card->isSelectable() && ( card->getCardType() == Card::Queen ) && ( getSizeOfDeckNow() == 0 || getPairOfQueenWasInCentralCards( card->getCardSuit() ) ) ){
					slotSelectedCardId( i );
					return;
				}
				
			}
			
			kDebug() << "If bot have ace, (and the type is equal the type of trump card) then select ace.";
			for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
				
				//Card card = getCard( i );
				Card *card = getCard( i );
				
				//if( card.isSelectable() && card.getCardType() == Card::Ace ){
				if( card != 0 && card->isSelectable() && card->getCardType() == Card::Ace ){
					slotSelectedCardId( i );
					return;
				}
				
			}
			
			kDebug() << "If bot have ten, (and the type is equal the type of trump card) then select ten.";
			for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
				
				//Card card = getCard( i );
				Card *card = getCard( i );
				
				//if( card.isSelectable() && card.getCardType() == Card::Ten ){
				if( card != 0 && card->isSelectable() && card->getCardType() == Card::Ten ){
					slotSelectedCardId( i );
					return;
				}
			}
			
			kDebug() << "If bot have king, and have not pair of king, then select king.";
			for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
				
				//Card card = getCard( i );
				Card *card = getCard( i );
				
				//if( card.isSelectable() && card.getCardType() == Card::King ){
				if( card != 0 && card->isSelectable() && card->getCardType() == Card::King ){
					
					bool ok = true;
					
					for( int j = 0; j < getNumberOfCardsInHand(); ++j ){
						
						//if( getCard( j ).isValid() && getCard( j ).getCardSuit() == card.getCardSuit() && getCard( j ).getCardType() == Card::Queen ){
						if( getCard( j ) != 0 && getCard( j )->getCardSuit() == card->getCardSuit() && getCard( j )->getCardType() == Card::Queen ){
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
				
				//Card card = getCard( i );
				Card *card = getCard( i );
				
				//if( card.isSelectable() && card.getCardType() == Card::Queen ){
				if( card != 0 && card->isSelectable() && card->getCardType() == Card::Queen ){
						
					bool ok = true;
					
					for( int j = 0; j < getNumberOfCardsInHand(); ++j ){
						
						//if( getCard( j ).isValid() && getCard( j ).getCardSuit() == card.getCardSuit() && getCard( j ).getCardType() == Card::King ){
						if( getCard( j ) != 0 && getCard( j )->getCardSuit() == card->getCardSuit() && getCard( j )->getCardType() == Card::King ){
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
				//Card card = getCard( i );
				Card *card = getCard( i );
				
				//if( card.isSelectable() && card.getCardType() == Card::Jack ){
				if( card != 0 && card->isSelectable() && card->getCardType() == Card::Jack ){
					slotSelectedCardId( i );
					return;
				}
			}
			
			if( getLowestCardType() == Card::Nine ){
				kDebug() << "If bot have nine, then select nine.";
				
				for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
					//Card card = getCard( i );
					Card *card = getCard( i );
					
					//if( card.isSelectable() && card.getCardType() == Card::Nine ){
					if( card != 0 && card->isSelectable() && card->getCardType() == Card::Nine ){
						slotSelectedCardId( i );
						return;
					}
				}
			}
			
			kDebug() << "Finally select anything.";
			for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
				
				//if( getCard( i ).isSelectable() ){
				if( getCard( i ) != 0 && getCard( i )->isSelectable() ){
					slotSelectedCardId( i );
					return;
				}
			}
			
		}
		
		
	}else{ // !getCentralCard()->isEmpty()
		kDebug() << "Central cards size: 1";
		
		//Card centralCard = getCentralCard( 0 );
		//Card centralCard0 = getCentralCards()->getCard( 0 );
		const Card* centralCard0 = getCentralCards()->getCard( 0 );
		//Card card;
		Card *card;
		
		
		kDebug() << "-Firstly try select the card, which points of card bigger then points of central card (and the suits of cards are equal).";
		for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
			
			card = getCard( i );
			
			//if( card.isSelectable() && card.getCardSuit() == centralCard0.getCardSuit() && card.getCardPoint() > centralCard0.getCardPoint() ){
			//if( card != 0 && card->isSelectable() && card->getCardSuit() == centralCard0.getCardSuit() && card->getCardPoint() > centralCard0.getCardPoint() ){
			if( card != 0 && card->isSelectable() && card->getCardSuit() == centralCard0->getCardSuit() && card->getCardPoint() > centralCard0->getCardPoint() ){
			
				if( getSizeOfDeckNow() == 0 || deckIsClosed ){
					//If size of deck now is 0 or closed, then can select king or queen card too
					
					slotSelectedCardId( i );
					return;
					
				}else{ //getSizeOfDeckNow() != 0 || deckIsClosed == false
					//If size of deck is not 0 or closed, then can't select king or queen if these' pairs wasn't in the central card.
					
					if( ( card->getCardType() == Card::King && getPairOfKingWasInCentralCards( card->getCardSuit() ) ) ||
						( card->getCardType() == Card::Queen && getPairOfQueenWasInCentralCards( card->getCardSuit() ) ) ||
						( card->getCardType() != Card::King && card->getCardType() != Card::Queen ) )
					{
						slotSelectedCardId( i );
						return;
					}

				}
				
				
				
			}
			
		}
		
		kDebug() << "-Secondly if the suit of central card is not equal with suit of trump card, then select the card, which suit is equal with suit of trump card.";
		//If suit of centralCard is not equal with suit of trump card
		//if( getTrumpCard().getCardSuit() != centralCard0.getCardSuit() ){
		//if( getTrumpCard()->getCardSuit() != centralCard0.getCardSuit() ){ 	
		if( getTrumpCard()->getCardSuit() != centralCard0->getCardSuit() ){ 	
			int selectedCardId = -1;
			
			for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
				card = getCard( i );
				
				//if( card.isSelectable() && card.getCardSuit() == getTrumpCard().getCardSuit() ){
				//if( card != 0 && card->isSelectable() && card->getCardSuit() == getTrumpCard().getCardSuit() ){
				if( card != 0 && card->isSelectable() && card->getCardSuit() == getTrumpCard()->getCardSuit() ){
					
					if( getSizeOfDeckNow() == 0 || deckIsClosed ){
						
						//if( !getCard( selectedCardId )->isValid() ){
						if( selectedCardId == -1 ){
							selectedCardId = i;
						}else{ // getCard( selectedCardId ).isValid()
							
							if( card->getCardType() < getCard( selectedCardId )->getCardType() ){
								selectedCardId = i;
							}
							
						}
						
					}else{
						
						if( ( card->getCardType() == Card::King && getPairOfKingWasInCentralCards( card->getCardSuit() ) ) ||
							( card->getCardType() == Card::Queen && getPairOfQueenWasInCentralCards( card->getCardSuit() ) ) ||
							( card->getCardType() != Card::King && card->getCardType() != Card::Queen ) )
						{
							//selectedCardId = i;
							//if( !getCard( selectedCardId )->isValid() ){
							if( selectedCardId == -1 ){
								selectedCardId = i;
							}else{ // getCard( selectedCardId ).isValid()
							
								if( card->getCardType() < getCard( selectedCardId )->getCardType() ){
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
			//Card card = getCard( i );
			card = getCard( i );
			
			//if( card.isSelectable() ){
			if( card != 0 && card->isSelectable() ){
					
				if( selectedCardId == -1 ){
					selectedCardId = i;
				}else{ // selectedCardId != -1
					
					if( getSizeOfDeckNow() == 0 || deckIsClosed ){
						
						if( card->getCardType() < getCard( selectedCardId )->getCardType() ){
							selectedCardId = i;
						} 
						
					}else{ // getSizeOfDeckNow() != 0 && deckIsClosed == false
						
						if( ( card->getCardType() == Card::King && getPairOfKingWasInCentralCards( card->getCardSuit() ) ) ||
							( card->getCardType() == Card::Queen && getPairOfQueenWasInCentralCards( card->getCardSuit() ) ) ||
							( card->getCardType() != Card::King && card->getCardType() != Card::Queen ) )
						{
							
							if( card->getCardType() < getCard( selectedCardId )->getCardType() ){
								selectedCardId = i;
							}
							
						}
						
					}
					
				}
				
			}
			
		}
		
		slotSelectedCardId( selectedCardId );
		
	}*/
    
    kDebug() << "Bot cards:";
    for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
        if( getCard( i ) != 0 ){
            kDebug() << getCard( i )->getCardText();
        }
    }
    kDebug() << "";

    
    if( mDifficulty == Knapsen::Easy ){
        easy();
    }else if( mDifficulty == Knapsen::Medium ){
        medium();
    }else{
        hard();
    }
    
}

void Bot::easy()
{
    kDebug() << "Easy";
    
    slotSelectCardId( getRandomCardId() );
}

void Bot::medium()
{
    kDebug() << "Medium";
    
    if( getCentralCards()->isEmpty() ){
        tryChangeTrumpCard();
        
        bool meld = tryMeldMarriage();
        
        if( meld ){
            trySelect( Card::King );
        }else{ //meld
            
            int rand = qrand()%7;
            kDebug() << "rand:" << rand;
            
            switch( rand ){
                case 0 : if( trySelectNotEqualTrump( Card::Ace ) ) break;
                case 1 : if( trySelectNotEqualTrump( Card::Ten ) ) break;
                case 2 : if( trySelectPairWasNotInCentral( Card::King ) ) break;
                case 3 : if( trySelectPairWasNotInCentral( Card::Queen ) ) break;
                case 4 : if( trySelectEqualTrump( Card::Ace ) ) break;
                case 5 : if( trySelectEqualTrump( Card::Ten ) ) break;
                case 6 : if( trySelect( Card::Jack ) ) break;
                case 7 : trySelect( Card::Nine );
            }
           
        }
        
    }else{ //getCentralCards()->isEmpty()
        
    }
    
}

void Bot::hard()
{
    kDebug() << "Hard";
    
    if( getCentralCards()->isEmpty() ){
        tryChangeTrumpCard();
        
        bool meld = tryMeldMarriage();
        
        if( meld ){
            trySelect( Card::King );
        }else{ // meld
            
            switch( 0 ){
                case 0 : if( trySelectNotEqualTrump( Card::Ace ) ) break;
                case 1 : if( trySelectNotEqualTrump( Card::Ten ) ) break;
                case 2 : if( trySelectPairWasNotInCentral( Card::King ) ) break;
                case 3 : if( trySelectPairWasNotInCentral( Card::Queen ) ) break;
                case 4 : if( trySelectEqualTrump( Card::Ace ) ) break;
                case 5 : if( trySelectEqualTrump( Card::Ten ) ) break;
                case 6 : if( trySelect( Card::Jack ) ) break;
                case 7 : trySelect( Card::Nine );
            }
            
        }
        
    }else{ // getCentralCards()->isEmpty()
        
        switch( 0 ){
            case 0 : if( trySelectEqualCentralNotEqualTrumpMinPoints() ) break;

        }
        
    }
    
}

int Bot::getRandomCardId()
{
    kDebug() << "Return a random card id";
    
    QList<int> selectableCards;
    for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
        if( getCard( i ) != 0 && getCard( i )->isSelectable() ){
            selectableCards.append( i );
        }
    }
    
    return selectableCards.at( qrand()%selectableCards.size() );
}

void Bot::tryChangeTrumpCard()
{
    kDebug() << "Try change trump card.";
    
    if( !getTrumpCard()->isEmpty() && getTrumpCard()->getCard()->isSelectable() ){
        kDebug() << "Can change trump card. Change it.";
        
        slotSelectTrumpCard();
    }
}

bool Bot::tryMeldMarriage()
{
    kDebug() << "Try meld marriage.";

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
    
    return clicktedToFortyOrTwentyButton;
}

bool Bot::trySelectNotEqualTrump( Card::CardType cardType )
{
    kDebug() << "If have" << cardType << "and the type of card is not equal with type of trump card then select card.";
    Card* card;
    Card::CardSuit trumpCardSuit = getTrumpCard()->getCard()->getCardSuit();
    
    for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
        card = getCard( i );
        
        if( card != 0 && card->isSelectable() && ( card->getCardSuit() != trumpCardSuit ) && ( card->getCardType() == cardType ) ){
            slotSelectCardId( i );
            return true;
        }
                                
    }
    
    return false;
}

bool Bot::trySelectPairWasNotInCentral( Card::CardType cardType )
{
    kDebug() << "If have card, what the pair of card was in the central cards _OR_ the deck of cards count 0 then select card.";
    
    Card::CardType pairCardType;
    if( cardType == Card::King ){
        pairCardType = Card::Queen;
    }else{ //cardType == Card::King
        pairCardType = Card::King;
    }
    
    Card* card;
    
    for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
        card = getCard( i );
        
        if( card != 0 && card->isSelectable() && ( card->getCardType() == cardType ) && ( mDeckIsClosed || getSizeOfDeckNow() == 0 || ( getPairOfCardWasInCentral( pairCardType, card->getCardSuit() ) ) ) ){
            slotSelectCardId( i );
            return true;
        }
        
    }
    
    return false;
}

bool Bot::trySelectEqualTrump( Card::CardType cardType )
{
    kDebug() << "If have card, what the type of card is equal with the type of trump card, then select card.";
    
    Card* card;
    Card::CardSuit trumpCardSuit = getTrumpCard()->getCard()->getCardSuit();
    
    for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
        card = getCard( i );
        
        if( card != 0 && card->isSelectable() && ( card->getCardSuit() == trumpCardSuit ) && ( card->getCardType() == cardType ) ){
            slotSelectCardId( i );
            return true;
        }
                                
    }
    
    return false;
}

bool Bot::trySelect( Card::CardType cardType )
{
    kDebug() << "If have " << cardType << " then I will select this.";
    
    for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
        if( getCard( i ) != 0 && getCard( i )->isSelectable() && getCard( i )->getCardType() == cardType ){
            slotSelectCardId( i );
            return true;
        }
    }
    
    return false;
}

bool Bot::trySelectEqualCentralNotEqualTrumpMinPoints()
{
    Card* card;
    int selectCardId = -1;
    Card* centralCard0 = getCentralCards()->getCard(0);
                
    kDebug() << "Try select the smallest points card, what's suit is equal whith central card's suit and NOT equal trump card's suit and the card's point is more then central card's point.";
    for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
        
        card = getCard( i );
        
        if( card != 0 &&
            card->isSelectable() &&
            card->getCardSuit() == centralCard0->getCardSuit() &&
            card->getCardPoint() > centralCard0->getCardPoint() )
        {
            if( ( getSizeOfDeckNow() == 0 || mDeckIsClosed ) ||
                ( ( card->getCardType() == Card::King && getPairOfCardWasInCentral( Card::King, card->getCardSuit() ) ) ||
                  ( card->getCardType() == Card::Queen && getPairOfCardWasInCentral( Card::Queen, card->getCardSuit() ) ) ||
                  ( card->getCardType() != Card::King && card->getCardType() != Card::Queen ) )   
            ){
                if( ( selectCardId == -1 ) ||
                    ( card->getCardPoint() < getCard( selectCardId )->getCardPoint()  )
                ){
                    selectCardId = i;
                }
            }
            
            
        }
        
    }
    
    if( selectCardId != -1 ){
       slotSelectCardId( selectCardId );
       return true;
    }
    
    return false;
}

void Bot::slotNewRound()
{
	mDeckIsClosed = false;
        
        for( int i = 0; i < 4; ++i ){
            pairOfQueenWasInCentralCards[i].second = pairOfKingWasInCentralCards[i].second = false;
        }
}

void Bot::slotNewCentralCard( int , Card* card )
{
    if( card->getCardType() == Card::Queen ){
    
        for( int i = 0; i < 4; ++i ){
        
            if( pairOfKingWasInCentralCards[i].first == card->getCardSuit() ){
                pairOfKingWasInCentralCards[i].second = true;
            }
                        
        }
                
    }else if( card->getCardType() == Card::King ){
    
        for( int i = 0; i < 4; ++i ){
        
            if( pairOfQueenWasInCentralCards[i].first == card->getCardSuit() ){
                pairOfQueenWasInCentralCards[i].second = true;
            }
                        
        }
                
    }
}

/*void Bot::slotEndRound( QString /roundWinnerName/ , int /scores/ )
{
	startNextRound();
}
*/

/*void Bot::slotendGame( QString /gameWinnerName/ )
{
	startNextGame();
}
*/
