#include <QtCore/QTimer>
#include <KDE/KDebug>
#include "bot.h"

Bot::Bot( QObject* parent ): Client( parent )
{
	kDebug() << "Initialize.";
	
	connect( this, SIGNAL( signalOpponentDisconnected() ),	this, SLOT( slotOpponentDisconnected() ) );
	connect( this, SIGNAL( signalInAction() ),				this, SLOT( slotInAction() ) );
	connect( this, SIGNAL( signalShowOpponentCards( int, QString, int, QString ) ), this, SLOT( slotShowOpponentCards( int, QString, int, QString ) ) );
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
	for( int i = 0; i < getNumberOfCardsInHand(); ++i ){
		if( getCard( i ).isValid() && getCard( i ).isSelectable() ){
			slotSelectedCardId( i );
			return;
		}
	}
}
