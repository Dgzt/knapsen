#include <QtGui/QGraphicsScene>
#include <QtGui/QResizeEvent>
#include <QtGui/QPushButton>
#include <QtGui/QGraphicsProxyWidget>
#include <QtGui/QGraphicsTextItem>
#include <QtSvg/QSvgRenderer>
#include <KDE/KStandardDirs>
#include <KDE/KGlobal>
#include <KDE/KLocalizedString>
#include <KDE/KDebug>
#include "table/svgcard.h"
#include "table/mytextitem.h"
#include "table/scoretable.h"
#include "engine/client.h"
#include "engine/card.h"
#include "table/cards.h"
#include "table/centralwidget.h"

//const int CENTRAL_CARDS_SIZE = 2;

const double GERMAN_CARDS_SCALE_VALUE = 0.6; //0.6
const double FRENCH_CARDS_SCALE_VALUE = 1.4; //1.4

const int NAME_CARDS_DISTANCE = 5; //5
const int SCORE_TABLE_CARDS_DISTANCE = 10; //10
const int CARDS_BUTTON_DISTANCE = 5; //5

CentralWidget::CentralWidget( QWidget* parent ): 
    QGraphicsView( parent ),
    mGameIsRunning( false ),
    mScale( 0.0 )
{
    mOpponentName = 0;
    mPlayerName = 0;
    mRenderer = 0;
    mOpponentCards = 0;
    mDeck = 0;
    mPlayerCards = 0;
    mTrumpCard = 0;
    mCentralCards = 0;
    
    //
    mPlayerArrow = 0;
    mOpponentArrow = 0;
    //
    
    mOpponentScoreTable = 0;
    mPlayerScoreTable = 0;
    
    mTwentyButton = 0;
    mFortyButton = 0;
    
    mSchnapsenButton = 0;
    
    //Set View's stuff
    setFrameStyle( QFrame::NoFrame );
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    
    //Set graphics scene
    QGraphicsScene* gScene = new QGraphicsScene;
    gScene->setItemIndexMethod( QGraphicsScene::NoIndex );
    setScene( gScene );
    
    QImage backgroundImage( KGlobal::dirs()->findResource( "appdata", "pics/background.png" ) );
    
    gScene->setBackgroundBrush( QBrush( backgroundImage ) );
}

CentralWidget::~CentralWidget()
{}

void CentralWidget::setClient( Client* client )
{
    mClient = client;
    
    connect( mClient, SIGNAL( signalInitialize( QString, QString, Knapsen::TypeOfCards ) ), 
             this,    SLOT( slotInitialize( QString, QString, Knapsen::TypeOfCards ) ) );
}

void CentralWidget::clearWidget()
{
    kDebug() << "Clear widget.";
    
    mGameIsRunning = false;
    
    disconnect( mClient, 0, 0, 0 );
    
    if( mDeck ){
        scene()->removeItem( mDeck );
        delete mDeck;
        mDeck = 0;
    }
    
    if( mTrumpCard ){
        disconnect( mTrumpCard, 0, 0, 0 );
        scene()->removeItem( mTrumpCard );
        delete mTrumpCard;
        mTrumpCard = 0;
    }
    
    if( mCloseButton ){
        disconnect( mCloseButton, 0, 0, 0 );
        scene()->removeItem( mCloseButton );
        delete mCloseButton;
        mCloseButton = 0;
    }
    
    if( mOpponentName ){
        scene()->removeItem( mOpponentName );
        delete mOpponentName;
        mOpponentName = 0;
    }
    
    if( mPlayerName ){
        scene()->removeItem( mPlayerName );
        delete mPlayerName;
        mPlayerName = 0;
    }
    
    if( mSchnapsenButton ){
        disconnect( mSchnapsenButton, 0, 0, 0 );
        scene()->removeItem( mSchnapsenButton );
        delete mSchnapsenButton;
        mSchnapsenButton = 0;
    }
    
    if( mFortyButton ){
        disconnect( mFortyButton, 0, 0, 0 );
        scene()->removeItem( mFortyButton );
        delete mFortyButton;
        mFortyButton = 0;
    }
    
    if( mTwentyButton ){
        disconnect( mTwentyButton, 0, 0, 0 );
        scene()->removeItem( mTwentyButton );
        delete mTwentyButton;
        mTwentyButton = 0;
    }
    
    if( mOpponentArrow ){
        scene()->removeItem( mOpponentArrow );
        delete mOpponentArrow;
        mOpponentArrow = 0;
    }
    
    if( mPlayerArrow ){
        scene()->removeItem( mPlayerArrow );
        delete mPlayerArrow;
        mPlayerArrow = 0;
    }
    
    if( mOpponentScoreTable ){
        scene()->removeItem( mOpponentScoreTable );
        delete mOpponentScoreTable;
        mOpponentScoreTable = 0;
    }
    
    if( mPlayerScoreTable ){
        scene()->removeItem( mPlayerScoreTable );
        delete mPlayerScoreTable;
        mPlayerScoreTable = 0;
    }
    
    if( mOpponentCards ){
        disconnect( mOpponentCards, 0, 0, 0 );
        scene()->removeItem( mOpponentCards );
        delete mOpponentCards;
        mOpponentCards = 0;
    }
    
    if( mPlayerCards ){
        disconnect( mPlayerCards, 0, 0, 0 );
        scene()->removeItem( mPlayerCards );
        delete mPlayerCards;
        mPlayerCards = 0;
    }
    
    if( mCentralCards ){
        scene()->removeItem( mCentralCards );
        delete mCentralCards;
        mCentralCards = 0;
    }
    
}

void CentralWidget::setInGamePositions()
{
    const int halpOfWidth  = width()/2;
    const int nameDistance = 5;

    //Set position of player's name
    QPoint playerNamePos;
    playerNamePos.setX( halpOfWidth - mPlayerName->boundingRect().width()/2 );
    playerNamePos.setY( height() - nameDistance -mPlayerName->boundingRect().height() );
    mPlayerName->setPos( playerNamePos );
    
    //Set position of opponent's name
    QPoint opponentNamePos;
    opponentNamePos.setX( halpOfWidth - mOpponentName->boundingRect().width()/2 );
    opponentNamePos.setY( nameDistance );
    mOpponentName->setPos( opponentNamePos );
    
    //Set position of deck
    mDeck->setPos( 20, height()/2-mCardSize.height()/2 );
    
    slotOpponentCardsSizeChanged();
    
    slotPlayerCardsSizeChanged();
    
    
    //Set position of trump card
    QPoint trumpPos ;
    trumpPos.setX( mDeck->scenePos().x() + 15 ); // +15 and about center of the deck the french and german suits cards 
    trumpPos.setY( height()/2 + 45 ); // +24 and about center of the deck the french and german suits cards
    
    mTrumpCard->setPos( trumpPos );
    
    //Set position of centralcards
    mCentralCards->setPos( ( width() - mCardSize.width() ) / 2,
                           ( height() - mCardSize.height() ) /2 );

    //Set position of close button
    QPoint closeButtonPos;
    closeButtonPos.setX( mDeck->scenePos().x() );
    closeButtonPos.setY( mDeck->scenePos().y()+mCardSize.height()+10 );
    
    mCloseButton->setGeometry( QRect( closeButtonPos, QSize( mCardSize.width(), 20 ) ) );
}

void CentralWidget::resizeEvent( QResizeEvent* re )
{
    QGraphicsView::resizeEvent( re );
    
    //The size of scene is equal with the size of view
    scene()->setSceneRect( 0, 0, width(), height() );
    
    if( mGameIsRunning ){
        setInGamePositions();
    }
}

void CentralWidget::slotInitialize( QString playerName, QString opponentName, Knapsen::TypeOfCards typeOfCards )
{
    kDebug() << "Initialize.";
    
    //Set player's name
    mPlayerName = new MyTextItem( playerName );
    mPlayerName->setVisible( false );
    scene()->addItem( mPlayerName );
    
    //Set opponent's name
    mOpponentName = new MyTextItem( opponentName );
    mOpponentName->setVisible( false );
    scene()->addItem( mOpponentName );
    
    //Set renderer
    if( typeOfCards == Knapsen::GermanSuits ){
        kDebug() << "Game with german suits cards.";
        mRenderer = new QSvgRenderer( KGlobal::dirs()->findResource( "appdata", "pics/william-tell.svgz" ) );
    }else{ // typeOfCards == Knapsen::FrenchSuits
        mRenderer = new QSvgRenderer( KGlobal::dirs()->findResource( "appdata", "pics/tigullio-bridge.svg" ) );
    }
    
    //
    if( typeOfCards == Knapsen::GermanSuits ){
        mScale = GERMAN_CARDS_SCALE_VALUE;
    }else{ //typeOfCards == Knapsen::GermanSuits
        mScale = FRENCH_CARDS_SCALE_VALUE;
    }
    //
    
    //Setup deck
    mDeck = new SvgCard( mRenderer, mScale );
    mDeck->setVisible( false );
    mDeck->setZValue( 1 );
    
    connect( mClient, SIGNAL( signalHideDeck() ), this, SLOT( slotHideDeck() ) );
    
    scene()->addItem( mDeck );
    
    //Set size of card
    mCardSize = mDeck->getSizeF();
    
    //Setup trump card
    mTrumpCard = new SvgCard( mRenderer, mScale );
    //mTrumpCard->setTransformOriginPoint( 0, mTrumpCard->boundingRect().height()/2 );
    mTrumpCard->setTransformOriginPoint( 0,0 );
    //mTrumpCard->setRotation( 90 );
    mTrumpCard->setRotation( -90 );
    mTrumpCard->setVisible( false );
    mTrumpCard->setSelectable( false );
    mTrumpCard->setZValue( 0 );
    
    connect( mClient, SIGNAL( signalNewTrumpCard( const Card* ) ),        this, SLOT( slotNewTrumpCard( const Card* ) ) );
    connect( mClient, SIGNAL( signalTrumpCardSelectableChanged( bool ) ), this, SLOT( slotTrumpCardSelectableChanged( bool ) ) );
    connect( mClient, SIGNAL( signalHideTrumpCard() ),                    this, SLOT( slotHideTrumpCard() ) );
    connect( mClient, SIGNAL( signalCloseDeck() ),                        this, SLOT( slotCloseDeck() ) ); 
    connect( mTrumpCard, SIGNAL( signalClick() ),                         mClient, SLOT( slotSelectTrumpCard() ) );
    
    scene()->addItem( mTrumpCard );
    
    //
    mOpponentCards = new Cards( mRenderer, mScale );
    
    connect( mClient, SIGNAL( signalNewOpponentCard() ),                         mOpponentCards, SLOT( slotNewCard() ) );
    connect( mClient, SIGNAL( signalOpponentSelectedCardId( int ) ),             mOpponentCards, SLOT( slotRemoveCard( int ) ) );
    connect( mClient, SIGNAL( signalShowOpponentCards( int, Card, int, Card ) ), mOpponentCards, SLOT( slotShowCards( int, Card, int, Card ) ) );
    connect( mOpponentCards, SIGNAL( signalSizeChanged() ),                      this, SLOT( slotOpponentCardsSizeChanged() ) );
    connect( mOpponentCards, SIGNAL( signalHiddenShowedCard() ),                 mClient, SLOT( slotProcessCommands() ) );
    
    scene()->addItem( mOpponentCards );
    
    mPlayerCards = new Cards( mRenderer, mScale );
    
    connect( mClient, SIGNAL( signalNewPlayerCard( const Card* ) ),              mPlayerCards, SLOT( slotNewCard( const Card* ) ) );
    connect( mClient, SIGNAL( signalChangePlayerCard( int, const Card* ) ),      mPlayerCards, SLOT( slotChangeCard( int, const Card* ) ) );
    connect( mClient, SIGNAL( signalPlayerCardSelectableChanged( int, bool ) ),  mPlayerCards, SLOT( slotSelectableChanged( int , bool ) ) );
    connect( mPlayerCards, SIGNAL( signalSizeChanged() ),                        this, SLOT( slotPlayerCardsSizeChanged() ) );
    connect( mPlayerCards, SIGNAL( signalSelectedCardId( int ) ),                mClient, SLOT( slotSelectCardId( int ) ) );
    
    scene()->addItem( mPlayerCards );
    //
    
    //Setup central cards
    mCentralCards = new Cards( mRenderer, mScale );
    
    connect( mClient, SIGNAL( signalNewCentralCard( const Card* ) ), mCentralCards, SLOT( slotNewCard( const Card* ) ) );
    connect( mClient, SIGNAL( signalClearCentralCards() ),           mCentralCards, SLOT( slotRemoveAllCards() ) );
    
    scene()->addItem( mCentralCards );
    
    //Arrows
    QPixmap arrowImage( KGlobal::dirs()->findResource( "appdata", "pics/arrow.png" ) );
    mPlayerArrow = scene()->addPixmap( arrowImage );
    mOpponentArrow = scene()->addPixmap( arrowImage );
    
    mPlayerArrow->setVisible( false );
    mOpponentArrow->setVisible( false );
    
    connect( mClient, SIGNAL( signalOpponentInAction() ),                 this, SLOT( slotShowOpponentArrow() ) );
    connect( mClient, SIGNAL( signalPlayerInAction() ),                   this, SLOT( slotShowPlayerArrow() ) );
    connect( mClient,      SIGNAL( signalOpponentSelectedCardId( int ) ), this, SLOT( slotHideOpponentArrow() ) );
    connect( mPlayerCards, SIGNAL( signalSelectedCardId( int ) ),         this, SLOT( slotHidePlayerArrow() ) );
    
    
    //Setup score tables
    mOpponentScoreTable = new ScoreTable;
    mPlayerScoreTable = new ScoreTable;
    
    mOpponentScoreTable->setVisible( false );
    mPlayerScoreTable->setVisible( false );
    
    connect( mClient, SIGNAL( signalOpponentTricksChanged( int ) ),     mOpponentScoreTable, SLOT( slotTricksChanged( int ) ) );
    connect( mClient, SIGNAL( signalOpponentScoresChanged( int ) ),     mOpponentScoreTable, SLOT( slotScoresChanged( int ) ) );
    connect( mClient, SIGNAL( signalPlayerTricksChanged( int ) ),       mPlayerScoreTable, SLOT( slotTricksChanged( int ) ) );
    connect( mClient, SIGNAL( signalPlayerScoresChanged( int ) ),       mPlayerScoreTable, SLOT( slotScoresChanged( int ) ) );
    
    scene()->addItem( mOpponentScoreTable );
    scene()->addItem( mPlayerScoreTable );
    
    //Set twenty button
    QPushButton *twentyButton = new QPushButton( i18n( "Twenty" ) );
    twentyButton->setAttribute( Qt::WA_NoSystemBackground );
    mTwentyButton = scene()->addWidget( twentyButton );
    mTwentyButton->setVisible( false );
    
    connect( mClient, SIGNAL( signalTwentyButtonVisible( bool ) ) , this, SLOT( slotTwentyButtonVisible( bool ) ) );
    connect( twentyButton, SIGNAL( clicked() ),                     mClient, SLOT( slotTwentyButtonClicked() ) );
    
    //Set forty button
    QPushButton *fortyButton = new QPushButton( i18n( "Forty" ) );
    fortyButton->setAttribute( Qt::WA_NoSystemBackground );
    mFortyButton = scene()->addWidget( fortyButton );
    mFortyButton->setVisible( false );
    
    connect( mClient, SIGNAL( signalFortyButtonVisible( bool ) ) , this, SLOT( slotFortyButtonVisible( bool ) ) );
    connect( fortyButton, SIGNAL( clicked() ),                     mClient, SLOT( slotFortyButtonClicked() ) );
    
    QPushButton *schnapsenButton = new QPushButton( i18n( "Schnapsen" ) );
    schnapsenButton->setAttribute( Qt::WA_NoSystemBackground );
    mSchnapsenButton = scene()->addWidget( schnapsenButton );
    mSchnapsenButton->setVisible( false );
    
    connect( mClient, SIGNAL( signalSchnapsenButtonVisible( bool ) ), this, SLOT( slotSchnapsenButtonVisible( bool ) ) );
    connect( schnapsenButton, SIGNAL( clicked() ),                    mClient, SLOT( slotSchnapsenButtonClicked() ) );
    
    //Set close button
    QPushButton *closeButton = new QPushButton( i18n( "Close" ) );
    closeButton->setAttribute( Qt::WA_NoSystemBackground );
    mCloseButton = scene()->addWidget( closeButton );
    mCloseButton->setVisible( false );
    
    connect( mClient, SIGNAL( signalCloseButtonVisible( bool ) ), this, SLOT( slotCloseButtonVisible( bool ) ) );
    connect( closeButton, SIGNAL( clicked() ),                    mClient, SLOT( slotCloseButtonClicked() ) );
    
    connect( mClient, SIGNAL( signalNewRound() ), this, SLOT( slotNewRound() ) );
    connect( mClient, SIGNAL( signalStartGame() ), this, SLOT( slotStartGame() ) );
}

void CentralWidget::slotNewTrumpCard( const Card* card )
{
    mTrumpCard->setElementId( card->getCardText() );
    
    if( !mTrumpCard->isVisible() ){
        mTrumpCard->setVisible( true );
    }
}

void CentralWidget::slotHideTrumpCard()
{
    mTrumpCard->setVisible( false );
}

void CentralWidget::slotTrumpCardSelectableChanged( bool selectable )
{
    mTrumpCard->setSelectable( selectable );
}

void CentralWidget::slotHideDeck()
{
    mDeck->setVisible( false );
}

void CentralWidget::slotCloseDeck()
{
    mTrumpCard->setElementId( "back" );
    
    mDeck->setZValue( 0 );
    mTrumpCard->setZValue( 1 );
}

void CentralWidget::slotTwentyButtonVisible( bool visible )
{
    mTwentyButton->setVisible( visible );
}

void CentralWidget::slotFortyButtonVisible( bool visible )
{
    mFortyButton->setVisible( visible );
}

void CentralWidget::slotSchnapsenButtonVisible( bool visible )
{
    mSchnapsenButton->setVisible( visible );
}

void CentralWidget::slotCloseButtonVisible( bool visible )
{
    mCloseButton->setVisible( visible );
}

void CentralWidget::slotNewRound()
{
    mOpponentCards->slotRemoveAllCards();
    mPlayerCards->slotRemoveAllCards();
    mCentralCards->slotRemoveAllCards();
    
    if( mTrumpCard->zValue() == 1 ){
        mTrumpCard->setZValue( 0 );
        mDeck->setZValue( 1 );
    }
    
    if( !mDeck->isVisible() ){
        mDeck->setVisible( true );
    }
    
    mOpponentScoreTable->slotTricksChanged( 0 );
    mPlayerScoreTable->slotTricksChanged( 0 );
}

void CentralWidget::slotStartGame()
{
    kDebug() << "Start game.";

    setInGamePositions();
    
    mOpponentName->setVisible( true );
    mPlayerName->setVisible( true );
    
    mDeck->setVisible( true );
    
    mOpponentScoreTable->setVisible( true );
    mPlayerScoreTable->setVisible( true );
    
    mTrumpCard->setVisible( true );
    
    mGameIsRunning = true;
}

void CentralWidget::slotOpponentCardsSizeChanged()
{
    QPoint opponentHandPos;
    opponentHandPos.setX( ( width() - mOpponentCards->boundingRect().width() ) / 2 );
    opponentHandPos.setY( mOpponentName->y() + mOpponentName->boundingRect().height() + NAME_CARDS_DISTANCE );
    
    mOpponentCards->setPos( opponentHandPos );
    
    //Set Opponent' score table's size
    mOpponentScoreTable->setPos( mOpponentCards->pos().x() + mOpponentCards->boundingRect().width() + SCORE_TABLE_CARDS_DISTANCE/* + mCardSize.width()/2*/,
                                 mOpponentCards->y() );
    
    
    //Set Opponent's arrow's size
    mOpponentArrow->setPos( mOpponentCards->pos().x() + mOpponentCards->boundingRect().width() + SCORE_TABLE_CARDS_DISTANCE/* + mCardSize.width()/2*/,
                            mOpponentCards->pos().y() + mCardSize.height() - mOpponentArrow->boundingRect().height() );

    
}

void CentralWidget::slotPlayerCardsSizeChanged()
{
    QPoint playerHandPos;
    playerHandPos.setX( ( width() - mPlayerCards->boundingRect().width() ) / 2 );
    playerHandPos.setY(  mPlayerName->y() - NAME_CARDS_DISTANCE - mCardSize.height() );
    
    mPlayerCards->setPos( playerHandPos );
    
    //Set Player's score table's position
    mPlayerScoreTable->setPos( mPlayerCards->x() + mPlayerCards->boundingRect().width() + SCORE_TABLE_CARDS_DISTANCE/* + mCardSize.width()/2*/,
                               mPlayerCards->y() + mCardSize.height() - mPlayerScoreTable->boundingRect().height() );
    
    
    //Set Player's arrow's position
    mPlayerArrow->setPos( mPlayerCards->pos().x() + mPlayerCards->boundingRect().width() + SCORE_TABLE_CARDS_DISTANCE/* + mCardSize.width()/2*/,
                          mPlayerCards->pos().y() );
    
    //Set schnapsen button position
    mSchnapsenButton->setPos( mPlayerCards->x() - SCORE_TABLE_CARDS_DISTANCE - mSchnapsenButton->boundingRect().width(),
                              mPlayerCards->y() + mPlayerCards->boundingRect().height() - mCardSize.height() );
    
    //Set forty button position
    mFortyButton->setPos( mPlayerCards->x() - SCORE_TABLE_CARDS_DISTANCE - mFortyButton->boundingRect().width(),
                          mPlayerCards->y() + mPlayerCards->boundingRect().height() - ( mCardSize.height() + mFortyButton->boundingRect().height() ) /2  );
    
    //Set twenty button position
    mTwentyButton->setPos( mPlayerCards->x() - SCORE_TABLE_CARDS_DISTANCE - mTwentyButton->boundingRect().width(),
                           mPlayerCards->y() + mPlayerCards->boundingRect().height() - mTwentyButton->boundingRect().height() );
    
}

void CentralWidget::slotShowOpponentArrow()
{
    mOpponentArrow->setVisible( true );
}

void CentralWidget::slotShowPlayerArrow()
{
    mPlayerArrow->setVisible( true );
}

void CentralWidget::slotHideOpponentArrow()
{
    mOpponentArrow->setVisible( false );
}

void CentralWidget::slotHidePlayerArrow()
{
    mPlayerArrow->setVisible( false );
}
