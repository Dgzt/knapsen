#include <QtCore/QTimer>
#include <QtCore/QSignalMapper>
#include <QtGui/QPainter>
#include <QtGui/QGraphicsScene>
#include <QtGui/QResizeEvent>
#include <QtGui/QPushButton>
#include <QtGui/QGraphicsProxyWidget>
#include <QtGui/QGraphicsTextItem>
#include <QtSvg/QSvgRenderer>
#include <QtSvg/QGraphicsSvgItem>
#include <KDE/KStandardDirs>
#include <KDE/KGlobal>
#include <KDE/KLocalizedString>
#include <KDE/KDebug>
#include "table/svgcard.h"
#include "table/mytextitem.h"
#include "table/scoretable.h"
//
#include "engine/client.h"
#include "engine/card.h"
#include "table/hand.h"
//

#include "table/centralwidget.h"

const int CENTRAL_CARDS_SIZE = 2;

const double GERMAN_CARDS_SCALE_VALUE = 0.6; //0.6
const double FRENCH_CARDS_SCALE_VALUE = 1.4; //1.4

const int NAME_CARDS_DISTANCE = 5;

const int SCORE_TABLE_CARDS_DISTANCE = 10;

CentralWidget::CentralWidget( QWidget* parent ): 
    QGraphicsView( parent ),
    mGameIsRunning( false ),
    //mNumberOfCardsInHand( 0 ),
    mScale( 0.0 )
{
    mOpponentName = 0;
    mPlayerName = 0;
    mRenderer = 0;
    //
    //mNumberOfCardsInHand = 0;
    //
    mOpponentCards = 0;
    mDeck = 0;
    mPlayerCards = 0;
    mTrumpCard = 0;
    mCentralCards = 0;
    
    //
    mOpponentCardsShowTimer = 0;
    //
    
    //
    mPlayerArrow = 0;
    mOpponentArrow = 0;
    //
    
    mOpponentScoreTable = 0;
    mPlayerScoreTable = 0;
    
    mTwentyButton = 0;
    mFortyButton = 0;
    
    mShowOpponentCardsId = 0;
    
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

void CentralWidget::clearWidget()
{
    kDebug() << "Clear widget.";
    
    mGameIsRunning = false;
    
    scene()->removeItem( mOpponentName );
    scene()->removeItem( mPlayerName );
    scene()->removeItem( mDeck );
    /*for( int i = 0; i < mNumberOfCardsInHand; ++i ){
        scene()->removeItem( &mOpponentCards[i] );
        scene()->removeItem( &mPlayerCards[i] );
    }*/
    //
    scene()->removeItem( mOpponentCards );
    scene()->removeItem( mPlayerCards );
    //
    scene()->removeItem( mTrumpCard );
    //
    scene()->removeItem( mPlayerArrow );
    scene()->removeItem( mOpponentArrow );
    //
    scene()->removeItem( mOpponentScoreTable );
    scene()->removeItem( mPlayerScoreTable );
    for( int i = 0; i < CENTRAL_CARDS_SIZE; ++i ){
        scene()->removeItem( &mCentralCards[i] );
    }
    //scene()->removeItem( mTwentyButton );
    //scene()->removeItem( mFortyButton );
    scene()->removeItem( mCloseButton );
    
    //
    if( mOpponentCardsShowTimer ){
        mOpponentCardsShowTimer->stop();
        delete mOpponentCardsShowTimer;
        mOpponentCardsShowTimer = 0;
    }
    //
    
    delete mOpponentName;
    delete mPlayerName;
    //delete mRenderer;
    //delete mDeck;
    delete mOpponentCards; 
    delete mPlayerCards;
    //delete mTrumpCard;
    delete mPlayerArrow;
    delete mOpponentArrow;
    delete mOpponentScoreTable;
    delete mPlayerScoreTable;
    //delete[] mCentralCards;
    //delete mTwentyButton;
    //delete mFortyButton;
    //delete mCloseButton;
    
    mOpponentName = 0;
    mPlayerName = 0;
    mRenderer = 0;
    mDeck = 0;
    mOpponentCards = 0;
    mPlayerCards = 0;
    mTrumpCard = 0;
    mPlayerArrow = 0;
    mOpponentArrow = 0;
    mOpponentScoreTable = 0;
    mPlayerScoreTable = 0;
    mCentralCards = 0;
    mTwentyButton = 0;
    mFortyButton = 0;
    mCloseButton = 0;
}

void CentralWidget::setInGamePositions()
{
    const int halpOfWidth  = width()/2;
    const int nameDistance = 5;
    //const int cardDistance = 10;
    //const int scoreTableDistance = 5;
    const int buttonDistance = 5;
    
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
    
    //Set position of opponent's cards and player's cards
    float removeCardsDistance = 0.0;
    /*if( mNumberOfCardsInHand == 5 ){
        removeCardsDistance = 2 * cardDistance;
    }else{ //mNumberOfCardsInHand == 6
        removeCardsDistance = 2.5 * cardDistance;
    }*/
    
    /*QPoint opponentCard0Pos( halpOfWidth-((float)mNumberOfCardsInHand/2)*mCardSize.width()-removeCardsDistance,
                             nameDistance+mOpponentName->boundingRect().height()+nameDistance );
    QPoint playerCard0Pos( opponentCard0Pos.x(),
                           height()-nameDistance-mPlayerName->boundingRect().height()-nameDistance-mCardSize.height() );
    
    mOpponentCards[0].setPos( opponentCard0Pos );
    mPlayerCards[0].setPos( playerCard0Pos );
    */
    /*for(int i = 1; i < mNumberOfCardsInHand; ++i){
        opponentCard0Pos.rx() += cardDistance+mCardSize.width();
        mOpponentCards[i].setPos( opponentCard0Pos );
        
        playerCard0Pos.rx() += cardDistance+mCardSize.width();
        mPlayerCards[i].setPos( playerCard0Pos );
    }
    
    //Set position of arrows
    QPoint opponentArrowPoint;
    opponentArrowPoint.setX( mOpponentCards[ mNumberOfCardsInHand-1 ].scenePos().x()+mCardSize.width()+scoreTableDistance );
    opponentArrowPoint.setY( mOpponentCards[ mNumberOfCardsInHand-1 ].scenePos().y()+mCardSize.height()-mOpponentArrow->boundingRect().height() );
    mOpponentArrow->setPos( opponentArrowPoint );
    
    QPoint playerArrowPoint;
    playerArrowPoint.setX( mPlayerCards[ mNumberOfCardsInHand-1 ].scenePos().x()+mCardSize.width()+scoreTableDistance );
    playerArrowPoint.setY( mPlayerCards[ mNumberOfCardsInHand-1 ].scenePos().y() );
    mPlayerArrow->setPos( playerArrowPoint );
    
    //Set position of score tables
    QPoint opponentScoreTablePos;
    opponentScoreTablePos.setX( mOpponentCards[ mNumberOfCardsInHand-1 ].scenePos().x()+mCardSize.width()+scoreTableDistance );
    opponentScoreTablePos.setY( mOpponentCards[ mNumberOfCardsInHand-1 ].scenePos().y() );
    mOpponentScoreTable->setPos( opponentScoreTablePos );
    
    QPoint playerScoreTablePos;
    playerScoreTablePos.setX( mPlayerCards[ mNumberOfCardsInHand-1 ].scenePos().x()+mCardSize.width()+scoreTableDistance );
    playerScoreTablePos.setY( mPlayerCards[ mNumberOfCardsInHand-1 ].scenePos().y()+mCardSize.height()-mPlayerScoreTable->boundingRect().height() );
    mPlayerScoreTable->setPos( playerScoreTablePos );
    */
    
    //
    //mPlayerCards->setPos( width()/2, height()/2 );
    
    /*QPoint opponentHandPos;
    opponentHandPos.setX( ( width() - mOpponentCards->boundingRect().width() ) / 2 );
    opponentHandPos.setY( opponentNamePos.y() + mOpponentName->boundingRect().height() + nameDistance );
    
    mOpponentCards->setPos( opponentHandPos );
    */
    slotOpponentCardsSizeChanged();
    
    /*QPoint playerHandPos;
    playerHandPos.setX( ( width() - mPlayerCards->boundingRect().width() ) / 2 );
    playerHandPos.setY(  playerNamePos.y()-nameDistance-mCardSize.height() );
    
    mPlayerCards->setPos( playerHandPos );
    */
    slotPlayerCardsSizeChanged();
    
    //
    
    //Set position of trump card
    QPoint trumpPos ;
    trumpPos.setX( mDeck->scenePos().x() + 15 ); // +15 and about center of the deck the french and german suits cards 
    trumpPos.setY( height()/2 + 45 ); // +24 and about center of the deck the french and german suits cards
    
    mTrumpCard->setPos( trumpPos );
    
    //Set position of centralcards
    QPoint cenralCard1Pos;
    cenralCard1Pos.setX( halpOfWidth - mCardSize.width()/2 );
    cenralCard1Pos.setY( ( height()-mCardSize.height() ) / 2 );
    
    mCentralCards[0].setPos( cenralCard1Pos );
    
    QPoint cenralCard2Pos;
    cenralCard2Pos.setX( halpOfWidth );
    cenralCard2Pos.setY( cenralCard1Pos.y() );
    
    mCentralCards[1].setPos( cenralCard2Pos );
    
    //Set position of twenty button
    /*QPoint twentyButtonPos;
    twentyButtonPos.setX( halpOfWidth-cardDistance/2-mCardSize.width() );
    twentyButtonPos.setY( mPlayerCards[0].scenePos().y()-buttonDistance-20 );
    
    mTwentyButton->setGeometry( QRect( twentyButtonPos, QSize( mCardSize.width(), 20 ) ) );
    */
    //Set position of forty button
    /*QPoint fortyButtonPos;
    fortyButtonPos.setX( halpOfWidth+cardDistance/2 );
    fortyButtonPos.setY( mPlayerCards[0].scenePos().y()-buttonDistance-20 );
    
    mFortyButton->setGeometry( QRect( fortyButtonPos, QSize( mCardSize.width(), 20 ) ) );
    */
    //Set position of close button
    QPoint closeButtonPos;
    closeButtonPos.setX( mDeck->scenePos().x() );
    closeButtonPos.setY( mDeck->scenePos().y()+mCardSize.height()+10 );
    
    mCloseButton->setGeometry( QRect( closeButtonPos, QSize( mCardSize.width(), 20 ) ) );
}

/*void CentralWidget::setOpponentScoreTablePos()
{
    mOpponentScoreTable->setPos( mOpponentCards->pos().x() - SCORE_TABLE_CARDS_DISTANCE - mOpponentScoreTable->boundingRect().width(),
                                 mOpponentCards->y() );
}*/

/*void CentralWidget::setPlayerScoreTablePos()
{
    mPlayerScoreTable->setPos( mPlayerCards->pos().x() - SCORE_TABLE_CARDS_DISTANCE - mPlayerScoreTable->boundingRect().width(),
                          mPlayerCards->y() + mCardSize.height() - mPlayerScoreTable->boundingRect().height() );
}*/

void CentralWidget::resizeEvent( QResizeEvent* re )
{
    QGraphicsView::resizeEvent( re );
    
    //The size of scene is equal with the size of view
    scene()->setSceneRect( 0, 0, width(), height() );
    
    if( mGameIsRunning ){
        setInGamePositions();
    }
}

/*void CentralWidget::slotInitialize( QString playerName, QString opponentName, Knapsen::TypeOfCards typeOfCards , int numberOfCardsInHand )
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
    
    //Set opponent's and player's cards
    mNumberOfCardsInHand = numberOfCardsInHand;
    
    mOpponentCards = new QGraphicsSvgItem[ mNumberOfCardsInHand ];
    mPlayerCards = new MySvgItem[ mNumberOfCardsInHand ];
    
    for( int i = 0; i < mNumberOfCardsInHand; ++i ){
        mOpponentCards[i].setSharedRenderer( mRenderer );
        mOpponentCards[i].setElementId( "back" );
        //mOpponentCards[i].setScale( SCALE_VALUE );
        mOpponentCards[i].setScale( mScale );
        mOpponentCards[i].setVisible( false );
        
        scene()->addItem( &mOpponentCards[i] );
        
        mPlayerCards[i].setSharedRenderer( mRenderer );
        mPlayerCards[i].setElementId( "back" );
        //mPlayerCards[i].setScale( SCALE_VALUE );
        mPlayerCards[i].setScale( mScale );
        mPlayerCards[i].setVisible( false );
        mPlayerCards[i].setId( i );
        mPlayerCards[i].setSelectable( false );
        
        scene()->addItem( &mPlayerCards[i] );
        
        connect( &mPlayerCards[i], SIGNAL( click( int ) ), this, SLOT( slotClick( int ) ) );
    }
    
    //Setup deck
    mDeck = new QGraphicsSvgItem;
    mDeck->setSharedRenderer( mRenderer );
    mDeck->setElementId( "back" );
    //mDeck->setScale( SCALE_VALUE );
    mDeck->setScale( mScale );
    //
    mDeck->setZValue( 1 );
    //
            
    scene()->addItem( mDeck );
    
    //Setup trump card
    mTrumpCard = new MySvgItem;
    mTrumpCard->setSharedRenderer( mRenderer );
    mTrumpCard->setElementId( "back" );
    //mTrumpCard->setScale( SCALE_VALUE );
    mTrumpCard->setScale( mScale );
    //mTrumpCard->setTransformOriginPoint( 0, mTrumpCard->boundingRect().height()/2 );
    mTrumpCard->setTransformOriginPoint( 0,0 );
    //mTrumpCard->setRotation( 90 );
    mTrumpCard->setRotation( -90 );
    mTrumpCard->setVisible( false );
    mTrumpCard->setSelectable( false );
    mTrumpCard->setZValue( 0 );
    
    connect( mTrumpCard, SIGNAL( click() ), this, SLOT( slotSelectTrumpCardSlot() ) );
    
    scene()->addItem( mTrumpCard );
    
    //Setup central cards
    mCentralCards = new QGraphicsSvgItem[ CENTRAL_CARDS_SIZE ];
    for( int i = 0; i < CENTRAL_CARDS_SIZE; ++i ){
        mCentralCards[i].setSharedRenderer( mRenderer );
        //mCentralCards[i].setScale( SCALE_VALUE );
        mCentralCards[i].setScale( mScale );
        mCentralCards[i].setVisible( false );
        mCentralCards[i].setElementId( "" );
        
        scene()->addItem( &mCentralCards[i] );
    }
    
    //Arrows
    QPixmap arrowImage( KGlobal::dirs()->findResource( "appdata", "pics/arrow.png" ) );
    mPlayerArrow = scene()->addPixmap( arrowImage );
    mOpponentArrow = scene()->addPixmap( arrowImage );
    
    mPlayerArrow->setVisible( false );
    mOpponentArrow->setVisible( false );
    
    //Setup score tables
    mOpponentScoreTable = new ScoreTable;
    mPlayerScoreTable = new ScoreTable;
    
    mOpponentScoreTable->setVisible( false );
    mPlayerScoreTable->setVisible( false );
    
    scene()->addItem( mOpponentScoreTable );
    scene()->addItem( mPlayerScoreTable );
    
    //Set twenty button
    QPushButton *twentyButton = new QPushButton( i18n( "Twenty" ) );
    twentyButton->setAttribute( Qt::WA_NoSystemBackground );
    mTwentyButton = scene()->addWidget( twentyButton );
    mTwentyButton->setVisible( false );
    connect( twentyButton, SIGNAL( clicked() ), this, SLOT( slotTwentyButtonClicked() ) );
    
    //Set forty button
    QPushButton *fortyButton = new QPushButton( i18n( "Forty" ) );
    fortyButton->setAttribute( Qt::WA_NoSystemBackground );
    mFortyButton = scene()->addWidget( fortyButton );
    mFortyButton->setVisible( false );
    connect( fortyButton, SIGNAL(clicked()), this, SLOT( slotFortyButtonClicked() ) );
    
    //Set close button
    QPushButton *closeButton = new QPushButton( i18n( "Close" ) );
    closeButton->setAttribute( Qt::WA_NoSystemBackground );
    mCloseButton = scene()->addWidget( closeButton );
    mCloseButton->setVisible( false );
    connect( closeButton, SIGNAL( clicked() ), this, SLOT( slotCloseButtonClicked() ) );
}*/

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
    
    /*//Setup deck
    mDeck = new QGraphicsSvgItem;
    mDeck->setSharedRenderer( mRenderer );
    mDeck->setElementId( "back" );
    //mDeck->setScale( SCALE_VALUE );
    mDeck->setScale( mScale );
    //
    mDeck->setZValue( 1 );
    //
            
    scene()->addItem( mDeck );
    */
    
    mDeck = new SvgCard( mRenderer, mScale );
    mDeck->setElementId( "back" );
    mDeck->setVisible( false );
    mDeck->setZValue( 1 );
    
    scene()->addItem( mDeck );
    
    //Set size of card
    mCardSize = mDeck->getSizeF();
    
    //Setup trump card
    mTrumpCard = new SvgCard( mRenderer, mScale );
    //mTrumpCard->setSharedRenderer( mRenderer );
    mTrumpCard->setElementId( "back" );
    //mTrumpCard->setScale( SCALE_VALUE );
    //mTrumpCard->setScale( mScale );
    //mTrumpCard->setTransformOriginPoint( 0, mTrumpCard->boundingRect().height()/2 );
    mTrumpCard->setTransformOriginPoint( 0,0 );
    //mTrumpCard->setRotation( 90 );
    mTrumpCard->setRotation( -90 );
    mTrumpCard->setVisible( false );
    mTrumpCard->setSelectable( false );
    mTrumpCard->setZValue( 0 );
    
    connect( mTrumpCard, SIGNAL( click() ), this, SLOT( slotSelectTrumpCardSlot() ) );
    
    scene()->addItem( mTrumpCard );
    
    //Set opponent's and player's cards
    /*mNumberOfCardsInHand = numberOfCardsInHand;
    
    mOpponentCards = new QGraphicsSvgItem[ mNumberOfCardsInHand ];
    mPlayerCards = new MySvgItem[ mNumberOfCardsInHand ];
    
    for( int i = 0; i < mNumberOfCardsInHand; ++i ){
        mOpponentCards[i].setSharedRenderer( mRenderer );
        mOpponentCards[i].setElementId( "back" );
        //mOpponentCards[i].setScale( SCALE_VALUE );
        mOpponentCards[i].setScale( mScale );
        mOpponentCards[i].setVisible( false );
        
        scene()->addItem( &mOpponentCards[i] );
        
        mPlayerCards[i].setSharedRenderer( mRenderer );
        mPlayerCards[i].setElementId( "back" );
        //mPlayerCards[i].setScale( SCALE_VALUE );
        mPlayerCards[i].setScale( mScale );
        mPlayerCards[i].setVisible( false );
        mPlayerCards[i].setId( i );
        mPlayerCards[i].setSelectable( false );
        
        scene()->addItem( &mPlayerCards[i] );
        
        connect( &mPlayerCards[i], SIGNAL( click( int ) ), this, SLOT( slotClick( int ) ) );
    }*/
    
    //
    mOpponentCards = new Hand( mRenderer, mScale );
    connect( mClient, SIGNAL( signalNewOpponentCard() ),                        mOpponentCards, SLOT( slotNewCard() ) );
    connect( mClient, SIGNAL( signalOpponentSelectedCardId( int ) ),            mOpponentCards, SLOT( slotRemoveCard( int ) ) );
    
    connect( mOpponentCards, SIGNAL( signalSizeChanged() ),                     this, SLOT( slotOpponentCardsSizeChanged() ) );
    
    scene()->addItem( mOpponentCards );
    
    mPlayerCards = new Hand( mRenderer, mScale );
    connect( mClient, SIGNAL( signalNewPlayerCard( const Card* ) ),             mPlayerCards, SLOT( slotNewCard( const Card* ) ) );
    connect( mClient, SIGNAL( signalPlayerCardSelectableChanged( int, bool ) ), mPlayerCards, SLOT( slotSelectableChanged( int , bool ) ) );
    
    connect( mPlayerCards, SIGNAL( signalSizeChanged() ),                       this, SLOT( slotPlayerCardsSizeChanged() ) );
    
    connect( mPlayerCards, SIGNAL( signalSelectedCardId( int ) ),                 mClient, SLOT( slotSelectCardId( int ) ) );
    
    scene()->addItem( mPlayerCards );
    //
    
    //Setup central cards
    mCentralCards = new QGraphicsSvgItem[ CENTRAL_CARDS_SIZE ];
    for( int i = 0; i < CENTRAL_CARDS_SIZE; ++i ){
        mCentralCards[i].setSharedRenderer( mRenderer );
        //mCentralCards[i].setScale( SCALE_VALUE );
        mCentralCards[i].setScale( mScale );
        mCentralCards[i].setVisible( false );
        mCentralCards[i].setElementId( "" );
        
        scene()->addItem( &mCentralCards[i] );
    }
    
    //Arrows
    QPixmap arrowImage( KGlobal::dirs()->findResource( "appdata", "pics/arrow.png" ) );
    mPlayerArrow = scene()->addPixmap( arrowImage );
    mOpponentArrow = scene()->addPixmap( arrowImage );
    
    mPlayerArrow->setVisible( false );
    mOpponentArrow->setVisible( false );
    
    connect( mClient, SIGNAL( signalOpponentInAction() ),       this, SLOT( slotOpponentinAction() ) );
    connect( mClient, SIGNAL( signalPlayerInAction() ),         this, SLOT( slotPlayerInAction() ) );
    
    //Setup score tables
    mOpponentScoreTable = new ScoreTable;
    mPlayerScoreTable = new ScoreTable;
    
    mOpponentScoreTable->setVisible( false );
    mPlayerScoreTable->setVisible( false );
    
    scene()->addItem( mOpponentScoreTable );
    scene()->addItem( mPlayerScoreTable );
    
    //Set twenty button
    QPushButton *twentyButton = new QPushButton( i18n( "Twenty" ) );
    twentyButton->setAttribute( Qt::WA_NoSystemBackground );
    mTwentyButton = scene()->addWidget( twentyButton );
    mTwentyButton->setVisible( false );
    connect( twentyButton, SIGNAL( clicked() ), this, SLOT( slotTwentyButtonClicked() ) );
    
    //Set forty button
    QPushButton *fortyButton = new QPushButton( i18n( "Forty" ) );
    fortyButton->setAttribute( Qt::WA_NoSystemBackground );
    mFortyButton = scene()->addWidget( fortyButton );
    mFortyButton->setVisible( false );
    connect( fortyButton, SIGNAL(clicked()), this, SLOT( slotFortyButtonClicked() ) );
    
    //Set close button
    QPushButton *closeButton = new QPushButton( i18n( "Close" ) );
    closeButton->setAttribute( Qt::WA_NoSystemBackground );
    mCloseButton = scene()->addWidget( closeButton );
    mCloseButton->setVisible( false );
    connect( closeButton, SIGNAL( clicked() ), this, SLOT( slotCloseButtonClicked() ) );
}

/*void CentralWidget::slotNewPlayerCard( int id , const Card* card )
{
    mPlayerCards[ id ].setElementId( card->getCardText() );
    
    if( mGameIsRunning && !mPlayerCards[ id ].isVisible() ){
        mPlayerCards[ id ].setVisible( true );
    }
}*/

/*void CentralWidget::slotNewOpponentCardId( int id )
{
    if( mGameIsRunning ){
        mOpponentCards[ id ].setVisible( true );
    }
}*/

void CentralWidget::slotNewTrumpCard( const Card* card )
{
    mTrumpCard->setElementId( card->getCardText() );
    
    if( !mTrumpCard->isVisible() ){
        mTrumpCard->setVisible( true );
    }
}

void CentralWidget::slotTrumpCardHide()
{
    mTrumpCard->setVisible( false );
}

void CentralWidget::slotTrumpCardSelectableChanged( bool selectable )
{
    mTrumpCard->setSelectable( selectable );
}

/*void CentralWidget::slotPlayerCardSelectableChanged( int id , bool selectable )
{
    mPlayerCards[ id ].setSelectable( selectable );
}*/

void CentralWidget::slotNewCentralCard( int id, const Card* card )
{
    mCentralCards[ id ].setElementId( card->getCardText() );
    mCentralCards[ id ].setVisible( true );
}

void CentralWidget::slotClearCentralCards()
{
    for( int i = 0; i < CENTRAL_CARDS_SIZE; ++i ){
        mCentralCards[ i ].setElementId( "back" );
        mCentralCards[ i ].setVisible( false );
    }
}

void CentralWidget::slotOpponentSelectedCardId( int id )
{
    //mOpponentCards[ id ].setVisible( false );
    
    //
    mOpponentArrow->setVisible( false );
}

void CentralWidget::slotOpponentTricksChanged( int tricks )
{
    mOpponentScoreTable->setTricks( tricks );
}

void CentralWidget::slotOpponentScoresChanged( int scores )
{
    mOpponentScoreTable->setScores( scores );
}

void CentralWidget::slotPlayerTricksChanged( int tricks )
{
    mPlayerScoreTable->setTricks( tricks );
}

void CentralWidget::slotPlayerScoresChanged( int scores )
{
    mPlayerScoreTable->setScores( scores );
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

void CentralWidget::slotCloseButtonVisible( bool visible )
{
    mCloseButton->setVisible( visible );
}

void CentralWidget::slotShowOpponentCards( int card1Pos, Card card1, int card2Pos, Card card2 )
{
    /*mOpponentCards[ card1Pos ].setElementId( card1.getCardText() );
    mOpponentCards[ card2Pos ].setElementId( card2.getCardText() );
    
    mShowOpponentCardsId = new QPair< int, int >;
    mShowOpponentCardsId->first = card1Pos;
    mShowOpponentCardsId->second = card2Pos;
    
    mOpponentCardsShowTimer = new QTimer( this );
    mOpponentCardsShowTimer->setSingleShot( true );
    mOpponentCardsShowTimer->setInterval( 1000 );
    
    connect( mOpponentCardsShowTimer, SIGNAL( timeout() ), this, SLOT( slotCoverOpponentCards() ) );
    
    mOpponentCardsShowTimer->start();*/
}

void CentralWidget::slotPlayerInAction()
{
    kDebug() << "Player in action.";
    
    mPlayerArrow->setVisible( true );
}

void CentralWidget::slotOpponentinAction()
{
    kDebug() << "Opponent in action.";
    
    mOpponentArrow->setVisible( true );
}

void CentralWidget::slotNewRound()
{
    /*for( int i = 0; i < mNumberOfCardsInHand; ++i ){
        mOpponentCards[ i ].setVisible( false );
        mPlayerCards[ i ].setVisible( false );
    }*/
    
    if( mTrumpCard->zValue() == 1 ){
        mTrumpCard->setZValue( 0 );
        mDeck->setZValue( 1 );
    }
    
    if( !mDeck->isVisible() ){
        mDeck->setVisible( true );
    }
    
    mCentralCards[ 0 ].setVisible( false );
    mCentralCards[ 1 ].setVisible( false );
    
    mOpponentScoreTable->setTricks( 0 );
    mPlayerScoreTable->setTricks( 0 );
}

void CentralWidget::slotStartGame()
{
    kDebug() << "Start game.";
    
    //mCardSize = mDeck->boundingRect().size()*SCALE_VALUE;
    mCardSize = mDeck->boundingRect().size()*mScale;
    
    setInGamePositions();
    
    mOpponentName->setVisible( true );
    mPlayerName->setVisible( true );
    
    mDeck->setVisible( true );
    
    /*for( int i = 0; i < mNumberOfCardsInHand; ++i ){
        mPlayerCards[ i ].setVisible( true );
        mOpponentCards[ i ].setVisible( true );
    }*/
    
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
    //mOpponentScoreTable->setPos( mOpponentCards->pos().x() - SCORE_TABLE_CARDS_DISTANCE - mOpponentScoreTable->boundingRect().width(),
    //                             mOpponentCards->y() );
    mOpponentScoreTable->setPos( mOpponentCards->pos().x() + mOpponentCards->boundingRect().width() + SCORE_TABLE_CARDS_DISTANCE + mCardSize.width()/2,
                                 mOpponentCards->y() );
    
    //Set Opponent's arrow's size
    //mOpponentArrow->setPos( mOpponentCards->pos().x() - SCORE_TABLE_CARDS_DISTANCE - mOpponentArrow->pos().x(),
    //                        mOpponentCards->pos().y() );
    mOpponentArrow->setPos( mOpponentCards->pos().x() + mOpponentCards->boundingRect().width() + SCORE_TABLE_CARDS_DISTANCE + mCardSize.width()/2,
                            mOpponentCards->pos().y() );

}

void CentralWidget::slotPlayerCardsSizeChanged()
{
    QPoint playerHandPos;
    playerHandPos.setX( ( width() - mPlayerCards->boundingRect().width() ) / 2 );
    playerHandPos.setY(  mPlayerName->y() - NAME_CARDS_DISTANCE - mCardSize.height() );
    
    mPlayerCards->setPos( playerHandPos );
    
    //Set Player's score table's size
    //mPlayerScoreTable->setPos( mPlayerCards->pos().x() - SCORE_TABLE_CARDS_DISTANCE - mPlayerScoreTable->boundingRect().width(),
    //                           mPlayerCards->y() + mCardSize.height() - mPlayerScoreTable->boundingRect().height() );
    mPlayerScoreTable->setPos( mPlayerCards->pos().x() + mPlayerCards->boundingRect().width() + SCORE_TABLE_CARDS_DISTANCE + mCardSize.width()/2,
                               mPlayerCards->y() + mCardSize.height() - mPlayerScoreTable->boundingRect().height() );
    
    //Set Player's arrow's size
    //mPlayerArrow->setPos( mPlayerCards->pos().x() - SCORE_TABLE_CARDS_DISTANCE - mPlayerArrow->boundingRect().width(),
    //                      mPlayerCards->pos().y() );
    mPlayerArrow->setPos( mPlayerCards->pos().x() + mPlayerCards->boundingRect().width() + SCORE_TABLE_CARDS_DISTANCE + mCardSize.width()/2,
                          mPlayerCards->pos().y() );
    
}

void CentralWidget::slotClick( int id )
{
    emit signalSelectCardId( id );
    mPlayerCards[ id ].setVisible( false );
    
    //
    mPlayerArrow->setVisible( false );
}

void CentralWidget::slotCoverOpponentCards()
{
    /*mOpponentCards[ mShowOpponentCardsId->first ].setElementId( "back" );
    mOpponentCards[ mShowOpponentCardsId->second ].setElementId( "back" );
    
    delete mShowOpponentCardsId;
    mShowOpponentCardsId = 0;
    
    delete mOpponentCardsShowTimer;
    mOpponentCardsShowTimer = 0;
    
    emit signalHideShowedOpponentCards();*/
}
