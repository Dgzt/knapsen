#include <QtCore/QTimer>
#include <KDE/KDebug>
#include "bot.h"

Bot::Bot( QObject* parent ): Client( parent )
{
	kDebug() << "Initialize.";
	
	connect( this, SIGNAL( signalOpponentDisconnected() ),		this, SLOT( slotOpponentDisconnected() ) );
	connect( this, SIGNAL( signalInAction() ),					this, SLOT( slotInAction() ) );
	connect( this, SIGNAL( signalEndRound( QString, int ) ),	this, SLOT( slotEndRound( QString, int ) ) );
	connect( this, SIGNAL( signalEndGame( QString ) ),			this, SLOT( slotendGame( QString ) ) );
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
			
			
			

		}
		
		
	}else{ // getCentralCardsSize == 1
		kDebug() << "Central cards size: 1";
		
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
