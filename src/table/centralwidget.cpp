#include <QGraphicsObject>
//#include <KDE/KGlobal>
#include <KDE/KStandardDirs>
#include <KDE/KDebug>
//#include <KDE/KLocalizedString>
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
