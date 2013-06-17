#include <QGraphicsObject>
//#include <KDE/KGlobal>
#include <KDE/KStandardDirs>
#include <KDE/KDebug>
//#include <KDE/KLocalizedString>
#include "engine/card.h"
#include "table/centralwidget.h"

CentralWidget::CentralWidget( QWidget* parent ): 
    QDeclarativeView( parent )
{
    //Set background
    //scene()->setBackgroundBrush( QBrush( QImage( KGlobal::dirs()->findResource( "appdata", "pics/background.png" ) ) ) );
    
    QString path = KStandardDirs::locate( "appdata", "qml/main.qml" );
    setSource( QUrl::fromLocalFile( path ) );
    
    setResizeMode( QDeclarativeView::SizeRootObjectToView );
    //setResizeMode( QDeclarativeView::SizeViewToRootObject );
    //setAlignment( Qt::AlignCenter | Qt::AlignCenter );
    
    connect( rootObject(), SIGNAL( signalAnimationEnd() ), this, SIGNAL( signalAnimationEnd() ) );
    connect( rootObject(), SIGNAL( signalSelectedCard( int, int ) ), this, SIGNAL( signalSelectedCard( int, int ) ) );
    connect( rootObject(), SIGNAL( signalCloseButtonClicked() ), this, SIGNAL( signalCloseButtonClicked() ) );
    connect( rootObject(), SIGNAL( signalFortyButtonClicked() ), this, SIGNAL( signalFortyButtonClicked() ) );
    connect( rootObject(), SIGNAL( signalTwentyButtonClicked() ), this, SIGNAL( signalTwentyButtonClicked() ) );
    connect( rootObject(), SIGNAL( signalSchnapsenButtonClicked() ), this, SIGNAL( signalSchnapsenButtonClicked() ) );
    //connect( rootObject(), SIGNAL( signalTrumpCardClicked() ), this, SIGNAL( signalTrumpCardClicked() ) );
}

void CentralWidget::clear()
{
    QMetaObject::invokeMethod( rootObject(), "clear" );
}

void CentralWidget::slotInitialize( QString playerName, QString opponentName, Knapsen::TypeOfCards typeOfCards )
{
    kDebug() << playerName << opponentName;
    
    //
    QString picPath;
    qreal scale;
    if( typeOfCards == Knapsen::GermanSuits ){
        picPath = KGlobal::dirs()->findResource( "appdata", "pics/william-tell.svgz" );
        scale = 0.9; //0.8
    }else{
        picPath = KGlobal::dirs()->findResource( "appdata", "pics/tigullio-bridge.svg" );
        scale = 2;
    }
    kDebug() << picPath;
    //
   
    QMetaObject::invokeMethod( rootObject(), 
                               "initialize", 
                               Q_ARG( QVariant, playerName ), 
                               Q_ARG( QVariant, opponentName ),
                               Q_ARG( QVariant, picPath ),
                               Q_ARG( QVariant, scale ) );
}

void CentralWidget::slotStartGame()
{
    kDebug() << "Start game.";
    
    QMetaObject::invokeMethod( rootObject(), "newGame" );
}

void CentralWidget::slotNewRound()
{
    kDebug() << "New round.";
    
    QMetaObject::invokeMethod( rootObject(), "newRound" );
}

void CentralWidget::slotNewPlayerCard( bool lastCard, Card* card )
{
    kDebug() << "New player card.";
    
    QMetaObject::invokeMethod( rootObject(), "newPlayerCard", Q_ARG( QVariant, lastCard ), Q_ARG( QVariant, card->getCardText() ) );
}

void CentralWidget::slotNewPlayerCardTrumpCard()
{
    kDebug() << "New player card trump card.";
    
    QMetaObject::invokeMethod( rootObject(), "newPlayerCardTrumpCard" );
}

void CentralWidget::slotNewOpponentCard( bool lastCard )
{
    kDebug() << "New opponent card.";
    
    QMetaObject::invokeMethod( rootObject(), "newOpponentCard", Q_ARG( QVariant, lastCard ) );
}

void CentralWidget::slotNewOpponentCardTrumpCard()
{
    kDebug() << "New opponent card trump card.";
    
    QMetaObject::invokeMethod( rootObject(), "newOpponentCardTrumpCard" );
}

void CentralWidget::slotNewTrumpCard( Card* trumpCard )
{
    kDebug() << "New trump card.";
    
    QMetaObject::invokeMethod( rootObject(), "newTrumpCard", Q_ARG( QVariant, trumpCard->getCardText() ) );
}

void CentralWidget::slotPlayerCardSelectableChanged( int id, bool enabled )
{
    kDebug() << "Player card selectable changed";
    
    QMetaObject::invokeMethod( rootObject(), "playerCardSelectableChanged", Q_ARG( QVariant, id ), Q_ARG( QVariant, enabled ) );
}

void CentralWidget::slotOpponentSelectedCard( int id, Card* card )
{
    kDebug() << "Opponent selected a card.";
    
    QMetaObject::invokeMethod( rootObject(), "opponentSelectedCard", Q_ARG( QVariant, id ), Q_ARG( QVariant, card->getCardText() ) );
}

void CentralWidget::slotOpponentGetCentralCards()
{
    kDebug() << "Opponent get central cards.";
    
    QMetaObject::invokeMethod( rootObject(), "opponentGetCentralCards" );
}

void CentralWidget::slotPlayerGetCentralCards()
{
    kDebug() << "Player get central cards.";
    
    QMetaObject::invokeMethod( rootObject(), "playerGetCentralCards" );
}

void CentralWidget::slotCloseButtonVisibleChanged( bool visible )
{
    kDebug() << "Close button visible changed";
    
    QMetaObject::invokeMethod( rootObject(), "closeButtonVisibleChanged", Q_ARG( QVariant, visible ) );
}

void CentralWidget::slotCloseDeck()
{
    kDebug() << "Close deck.";
    
    QMetaObject::invokeMethod( rootObject(), "closeDeck" );
}

void CentralWidget::slotFortyButtonVisibleChanged( bool visible )
{
    kDebug() << "Forty button visible changed";
    
    QMetaObject::invokeMethod( rootObject(), "fortyButtonVisibleChanged", Q_ARG( QVariant, visible ) );
}

void CentralWidget::slotTwentyButtonVisibleChanged( bool visible )
{
    kDebug() << "Twenty button visible changed";
    
    QMetaObject::invokeMethod( rootObject(), "twentyButtonVisibleChanged", Q_ARG( QVariant, visible ) );
}

void CentralWidget::slotSchnapsenButtonVisibleChanged( bool visible )
{
    kDebug() << "Schnapsen button visible changed";
    
    QMetaObject::invokeMethod( rootObject(), "schnapsenButtonVisibleChanged", Q_ARG( QVariant, visible ) );
}

void CentralWidget::slotOpponentTricksChanged( int tricks )
{
    kDebug() << "Opponent's tricks changed";
    
    QMetaObject::invokeMethod( rootObject(), "opponentTricksChanged", Q_ARG( QVariant, tricks ) );
}

void CentralWidget::slotOpponentScoresChanged( int scores )
{
    kDebug() << "Opponent's scores changed";
    
    QMetaObject::invokeMethod( rootObject(), "opponentScoresChanged", Q_ARG( QVariant, scores ) );
}

void CentralWidget::slotPlayerTricksChanged( int tricks )
{
    kDebug() << "Player's tricks changed";
    
    QMetaObject::invokeMethod( rootObject(), "playerTricksChanged", Q_ARG( QVariant, tricks ) );
}

void CentralWidget::slotPlayerScoresChanged( int scores )
{
    kDebug() << "Player's scores changed";
    
    QMetaObject::invokeMethod( rootObject(), "playerScoresChanged", Q_ARG( QVariant, scores ) );
}

void CentralWidget::slotTrumpCardSelectableChanged( bool selectable )
{
    kDebug() << "Trump card selectable changed.";
    
    QMetaObject::invokeMethod( rootObject(), "trumpCardSelectableChanged", Q_ARG( QVariant, selectable ) );
}

void CentralWidget::slotPlayerChangeTrumpCard( int id )
{
    kDebug() << "Player change trump card.";
    
    QMetaObject::invokeMethod( rootObject(), "playerChangeTrumpCard", Q_ARG( QVariant, id ) );
}

void CentralWidget::slotOpponentChangeTrumpCard( int id, Card* newTrumpCard )
{
    kDebug() << "Opponent change trump card.";
    
    QMetaObject::invokeMethod( rootObject(), "opponentChangeTrumpCard", Q_ARG( QVariant, id ), Q_ARG( QVariant, newTrumpCard->getCardText() ) );
}

void CentralWidget::slotShowOpponentCards( int card1Id, Card card1, int card2Id, Card card2 )
{
    kDebug() << "Show opponent cards.";
    
    QMetaObject::invokeMethod( rootObject(), 
                               "showOpponentCards",
                               Q_ARG( QVariant, card1Id ),
                               Q_ARG( QVariant, card1.getCardText() ),
                               Q_ARG( QVariant, card2Id ),
                               Q_ARG( QVariant, card2.getCardText() )
                             );
}
