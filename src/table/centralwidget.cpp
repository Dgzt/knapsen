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
    scene()->setBackgroundBrush( QBrush( QImage( KGlobal::dirs()->findResource( "appdata", "pics/background.png" ) ) ) );
    
    QString path = KStandardDirs::locate( "appdata", "qml/main.qml" );
    setSource( QUrl::fromLocalFile( path ) );
  
    setResizeMode( QDeclarativeView::SizeRootObjectToView );
    setAlignment( Qt::AlignCenter | Qt::AlignCenter );
    
    connect( rootObject(), SIGNAL( signalAnimationEnd() ), this, SIGNAL( signalAnimationEnd() ) );
    connect( rootObject(), SIGNAL( signalSelectedCard( int, int ) ), this, SIGNAL( signalSelectedCard( int, int ) ) );
}

void CentralWidget::clear()
{
    QMetaObject::invokeMethod( rootObject(), "clear" );
}

void CentralWidget::resizeEvent( QResizeEvent* ){
    fitInView( sceneRect(), Qt::KeepAspectRatio );
    //QMetaObject::invokeMethod( rootObject(), "resize" );
}

void CentralWidget::slotInitialize( QString playerName, QString opponentName, Knapsen::TypeOfCards /*typeOfCards*/ )
{
    kDebug() << playerName << opponentName;
   
    QMetaObject::invokeMethod( rootObject(), "initialize", Q_ARG(QVariant, playerName), Q_ARG(QVariant, opponentName) );
}

void CentralWidget::slotStartGame()
{
    kDebug() << "Start game.";
    
    QMetaObject::invokeMethod( rootObject(), "startGame" );
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

void CentralWidget::slotNewOpponentCard( bool lastCard )
{
    kDebug() << "New opponent card.";
    
    QMetaObject::invokeMethod( rootObject(), "newOpponentCard", Q_ARG( QVariant, lastCard ) );
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
