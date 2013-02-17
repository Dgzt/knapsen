#include <KDE/KGlobal>
#include <KDE/KStandardDirs>
#include <KDE/KDebug>
#include <QtSvg/QSvgRenderer>
#include <QtCore/QPointF>
#include "engine/client.h"
#include "table/mytextitem.h"
#include "table/svgcard.h"
#include "table/animation.h"
#include "table/cardsgroup.h"
#include "table/centralwidget.h"

//
#include <QtCore/QTimer>
//

const int SCENE_WIDTH = 900; //1000
const int SCENE_HEIGHT = 850; //1000

const int NAME_DISTANCE = 10;

const int DECK_TRUMPCARD_DISTANCE = 30;

CentralWidget::CentralWidget( QWidget* parent ): 
    QGraphicsView( parent )
{
    mRenderer = 0;
    
    mOpponentName = 0;
    mPlayerName = 0;
    
    mDeck = 0;
    mTrumpCard = 0;
    mOpponentCards = 0;
    mPlayerCards = 0;
    mCentralCards = 0;
    
    //Set graphics scene
    QGraphicsScene* gScene = new QGraphicsScene( 0, 0, SCENE_WIDTH, SCENE_HEIGHT );
    setScene( gScene );
    
    QImage backgroundImage( KGlobal::dirs()->findResource( "appdata", "pics/background.png" ) );
    
    gScene->setBackgroundBrush( QBrush( backgroundImage ) );
    
    
    //Set View's stuff
    setFrameStyle( QFrame::NoFrame );
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    
    //setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setAlignment( Qt::AlignCenter | Qt::AlignCenter );
    
}

void CentralWidget::resizeEvent( QResizeEvent* ){
    fitInView( sceneRect(), Qt::KeepAspectRatio );
}

void CentralWidget::slotInitialize( QString playerNameStr, QString opponentNameStr, Knapsen::TypeOfCards typeOfCards )
{
    kDebug() << "Initialize.";
    
    //Setup opponent's name
    mOpponentName = new MyTextItem( opponentNameStr );
    mOpponentName->setVisible( false );
    
    scene()->addItem( mOpponentName );
    
    //Setup player's name
    mPlayerName = new MyTextItem( playerNameStr );
    mPlayerName->setVisible( false );
    
    scene()->addItem( mPlayerName );
    
    
    //Setup type of cards
    if( typeOfCards == Knapsen::GermanSuits ){
        kDebug() << "Game with german suits cards.";
        mRenderer = new QSvgRenderer( KGlobal::dirs()->findResource( "appdata", "pics/william-tell.svgz" ) );
    }else{ // typeOfCards == Knapsen::GermanSuits
        kDebug() << "Game with french suits cards.";
        mRenderer = new QSvgRenderer( KGlobal::dirs()->findResource( "appdata", "pics/tigullio-bridge.svg" ) );
    }
    
    //Setup Deck
    mDeck = new SvgCard( mRenderer );
    mDeck->setVisible( false );
    
    connect( mDeck->getAnimation(), SIGNAL( signalAnimationEnd() ), mClient, SLOT( slotProcessCommands() ) );
    
    scene()->addItem( mDeck );
    
    //Setup trump card
    mTrumpCard = new SvgCard( mRenderer );
    mTrumpCard->setVisible( false );
    
    scene()->addItem( mTrumpCard );
    
    connect( mClient, SIGNAL( signalNewTrumpCard( Card* ) ), this, SLOT( slotNewTrumpCard( Card* ) ) );
    connect( mTrumpCard->getAnimation(), SIGNAL( signalAnimationEnd() ), mClient, SLOT( slotProcessCommands() ) );
    
    //Setup Opponent's cards
    mOpponentCards = new CardsGroup( /*mDeck->boundingRect().size()*/ );
    
    //mOpponentCards->setPos( sceneRect().width() / 2,
    //                        opponentName->pos().y() + opponentName->boundingRect().height() + NAME_DISTANCE );
    
    scene()->addItem( mOpponentCards );
    
    connect( mClient, SIGNAL( signalNewOpponentCard() ), this, SLOT( slotNewOpponentCard() ) );
    //connect( mOpponentCards, SIGNAL( signalSizeChanged() ), this, SLOT( slotOpponentCardsSizeChanged() ) );
    connect( mOpponentCards, SIGNAL( signalCardArrived() ), mClient, SLOT( slotProcessCommands() ) );
    
    //Setup Player's cards
    mPlayerCards = new CardsGroup( /*mDeck->boundingRect().size()*/ );
    
    //mPlayerCards->setPos( sceneRect().width() / 2,
    //                      playerName->pos().y() - NAME_DISTANCE - mPlayerCards->boundingRect().height() );
    
    scene()->addItem( mPlayerCards );
    
    connect( mClient, SIGNAL( signalNewPlayerCard( Card* ) ), this, SLOT( slotNewPlayerCard( Card* ) ) );
    connect( mClient, SIGNAL( signalPlayerCardSelectableChanged( int , bool ) ), mPlayerCards, SLOT( slotSelectableChanged( int , bool ) ) );
    //connect( mPlayerCards, SIGNAL( signalSizeChanged() ), this, SLOT( slotPlayerCardsSizeChanged() ) );
    connect( mPlayerCards, SIGNAL( signalCardArrived() ), mClient, SLOT( slotProcessCommands() ) );
    connect( mPlayerCards, SIGNAL( signalSelectedCard( int, SvgCard* ) ), this, SLOT( slotPlayerSelectedCard( int , SvgCard* ) ) );

    //Setup central cards
    mCentralCards = new CardsGroup(/* mDeck->boundingRect().size() */);
    
    //mCentralCards->setPos( sceneRect().width() / 2, 
    //                       ( sceneRect().height() - mCentralCards->boundingRect().height() ) / 2 );
    mCentralCards->setPos( ( sceneRect().width() - ( mDeck->boundingRect().size().width() * 1.5 ) ) / 2, 
                           ( sceneRect().height() - mCentralCards->boundingRect().height() ) / 2 );
    
    //mCentralCards->setNewPos( mCentralCards->pos() );
    
    scene()->addItem( mCentralCards );
    
    connect( mClient, SIGNAL( signalNewGame() ), this, SLOT( slotNewGame() ) );
    //connect( mClient, SIGNAL( signalNewRound() ), this, SLOT( slotNewRound() ) );
}

void CentralWidget::slotNewGame()
{
    kDebug();
    
    //Opponent's name
    mOpponentName->setPos( ( sceneRect().width() - mOpponentName->boundingRect().width() ) / 2,
                             mapToScene( 0, 0 ).y() - mOpponentName->boundingRect().height() );
    
    
    QPointF endOpponentNamePos( mOpponentName->x(), 
                                NAME_DISTANCE );
    
    mOpponentName->getAnimation()->setEndPosition( endOpponentNamePos );
    
    mOpponentName->setVisible( true );
    mOpponentName->getAnimation()->startAnimation();
    
    //Player's name
    mPlayerName->setPos( ( sceneRect().width() - mPlayerName->boundingRect().width() ) /2,
                         mapToScene( width(), height() ).y() + mPlayerName->boundingRect().height() );
    
    QPointF endPlayerNamePos( mPlayerName->x(),
                              sceneRect().height() - NAME_DISTANCE - mPlayerName->boundingRect().height()  );
    
    mPlayerName->getAnimation()->setEndPosition( endPlayerNamePos );
    mPlayerName->setVisible( true );
    
    connect( mPlayerName, SIGNAL( signalMyTextItemAnimationEnd() ), this, SLOT( slotNewRound() ) );
    
    mPlayerName->getAnimation()->startAnimation();
    
    //Setup opponent's cards pos
    mOpponentCards->setPos( sceneRect().width() / 2,
                            endOpponentNamePos.y() + mOpponentName->boundingRect().height() + NAME_DISTANCE );
        
    
    //Setup player's cards pos
    mPlayerCards->setPos( sceneRect().width() / 2,
                          endPlayerNamePos.y() - NAME_DISTANCE - mDeck->boundingRect().height() );
    
}

void CentralWidget::slotNewRound()
{
    kDebug();
    
    QPointF startDeckPos( mapToScene( 0, 0 ).x() - mDeck->boundingRect().width(),
                          ( sceneRect().height() - mDeck->boundingRect().height() ) / 2 );
    
    QPointF endDeckPos( 50 , 
                        ( sceneRect().height() - mDeck->boundingRect().height() ) / 2 );
    
    mDeck->setPos( startDeckPos );
    mDeck->getAnimation()->setEndPosition( endDeckPos );
    
    //connect( mDeck->getAnimation(), SIGNAL( signalAnimationEnd() ), mClient, SLOT( slotProcessCommands() ) );
    
    mDeck->setVisible( true );
    
    mDeck->getAnimation()->startAnimation();
}

void CentralWidget::slotNewOpponentCard()
{
    kDebug();
    
    SvgCard* svgCard = new SvgCard( mRenderer );
    svgCard->setPos( mDeck->pos() );
    
    scene()->addItem( svgCard );
    
    mOpponentCards->addNewCard( svgCard, mDeck );
}

void CentralWidget::slotNewPlayerCard( Card* card )
{
    kDebug();
    
    SvgCard* svgCard = new SvgCard( mRenderer );
    svgCard->getAnimation()->setCard( card );
    
    mPlayerCards->addNewCard( svgCard, mDeck );
}

void CentralWidget::slotNewTrumpCard( Card* card )
{
    kDebug();
    
    /*mTrumpCard->setPos( mDeck->pos() );
    
    
    mTrumpCard->setVisible( true );
    
    Animation* animation = new Animation( mTrumpCard, endTrumpCardPos, card );
    connect( animation, SIGNAL( signalCardArrived() ), mClient, SLOT( slotProcessCommands() ) );
    
    //animation->startAnimation();
    animation->slotStartAnimation();*/

    mTrumpCard->setPos( mDeck->pos() );
    
    QPointF endTrumpCardPos( mDeck->pos().x() + mDeck->boundingRect().width() + DECK_TRUMPCARD_DISTANCE, mDeck->pos().y() );
    
    mTrumpCard->getAnimation()->setEndPosition( endTrumpCardPos );
    mTrumpCard->getAnimation()->setCard( card );
    
    mTrumpCard->setVisible( true );
    
    mTrumpCard->getAnimation()->startAnimation();
}

/*void CentralWidget::slotOpponentCardsSizeChanged()
{
    kDebug() << "a";
    
    //mOpponentCards->setPos( ( sceneRect().width() - mOpponentCards->boundingRect().width() ) /2, mOpponentCards->y() );

    QPointF endPos( ( sceneRect().width() - mOpponentCards->boundingRect().width() ) /2, mOpponentCards->y() );
    kDebug() << "b";
    mOpponentCards->setNewPos( endPos );
    kDebug() << "c";
    Animation* animation = new Animation( mOpponentCards, endPos );
    kDebug() << "d";
    animation->startAnimation();
    kDebug() << "e";
}*/

/*void CentralWidget::slotPlayerCardsSizeChanged()
{
    kDebug() << "a";
    
    //mPlayerCards->setPos( ( sceneRect().width() - mPlayerCards->boundingRect().width() ) /2, mPlayerCards->y() );
    
    QPointF endPos( ( sceneRect().width() - mPlayerCards->boundingRect().width() ) /2, mPlayerCards->y() );
    kDebug() << "b";
    mPlayerCards->setNewPos( endPos );
    kDebug() << "c";
    Animation* animation = new Animation( mPlayerCards, endPos );
    kDebug() << "d";
    animation->startAnimation();
    kDebug() << "e";
}*/

void CentralWidget::slotPlayerSelectedCard( int id, SvgCard* svgCard )
{
    kDebug() << id << svgCard->elementId();
    
    /*svgCard->setParentItem( 0 ); //Set to top-level item
    svgCard->setPos( mPlayerCards->pos() + svgCard->pos() );
    
    mCentralCards->newCardArrive();
    
    Animation* animation = new Animation( svgCard, mCentralCards->pos() + mCentralCards->getNewCardPos() );
    //animation->startAnimation();
    animation->slotStartAnimation();*/
}

void CentralWidget::setClient( Client* client )
{
    mClient = client;
    
    connect( mClient, SIGNAL( signalInitialize( QString, QString, Knapsen::TypeOfCards ) ), this, SLOT( slotInitialize( QString, QString, Knapsen::TypeOfCards ) ) );
}

void CentralWidget::clearWidget()
{
    scene()->clear();
    
    delete mRenderer;
    mRenderer = 0;
}
