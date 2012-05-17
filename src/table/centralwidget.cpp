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
#include "table/mysvgitem.h"
#include "table/mytextitem.h"
#include "table/scoretable.h"
#include "table/centralwidget.h"

const int CENTRAL_CARDS_SIZE = 2;

const double GERMAN_CARDS_SCALE_VALUE = 0.6; //0.6
const double FRENCH_CARDS_SCALE_VALUE = 1.4; //1.4

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
	mNumberOfCardsInHand = 0;
	//
	mOpponentCards = 0;
	mDeck = 0;
	mPlayerCards = 0;
	mTrumpCard = 0;
	mCentralCards = 0;
	
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
{

}

void CentralWidget::clearWidget()
{
	kDebug() << "Clear widget.";
	
	mGameIsRunning = false;
	
	scene()->removeItem( mOpponentName );
	scene()->removeItem( mPlayerName );
	scene()->removeItem( mDeck );
	for( int i = 0; i < mNumberOfCardsInHand; ++i ){
		scene()->removeItem( &mOpponentCards[i] );
		scene()->removeItem( &mPlayerCards[i] );
	}
	scene()->removeItem( mTrumpCard );
	scene()->removeItem( mOpponentScoreTable );
	scene()->removeItem( mPlayerScoreTable );
	for( int i = 0; i < CENTRAL_CARDS_SIZE; ++i ){
		scene()->removeItem( &mCentralCards[i] );
	}
	scene()->removeItem( mTwentyButton );
	scene()->removeItem( mFortyButton );
	scene()->removeItem( mCloseButton );
	
	delete mOpponentName;
	delete mPlayerName;
	delete mRenderer;
	delete mDeck;
	delete[] mOpponentCards;
	delete[] mPlayerCards;
	delete mTrumpCard;
	delete mOpponentScoreTable;
	delete mPlayerScoreTable;
	delete[] mCentralCards;
	delete mTwentyButton;
	delete mFortyButton;
	delete mCloseButton;
	
	mOpponentName = 0;
	mPlayerName = 0;
	mRenderer = 0;
	mDeck = 0;
	mOpponentCards = 0;
	mPlayerCards = 0;
	mTrumpCard = 0;
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
	const int cardDistance = 10;
	const int scoreTableDistance = 5;
	const int buttonDistance = 5;
	
	//Set position of player's name
	QPoint playerNamePos;
	playerNamePos.setX( halpOfWidth - mPlayerName->boundingRect().width()/2 );
	playerNamePos.setY( height() - nameDistance -mPlayerName->boundingRect().height() );
	mPlayerName->setPos( playerNamePos );
	
	//Set position of opponent's name
	playerNamePos.setX( halpOfWidth - mOpponentName->boundingRect().width()/2 );
	playerNamePos.setY( nameDistance );
	mOpponentName->setPos( playerNamePos );
	
	//Set position of deck
	mDeck->setPos( 20, height()/2-mCardSize.height()/2 );
	
	//Set position of opponent's cards and player's cards
	/*QPoint opponentCard0Pos( halpOfWidth-mCardSize.width()/2-2*mCardSize.width()-2*cardDistance, 
						nameDistance+mOpponentName->boundingRect().height()+nameDistance );
	QPoint playerCard0Pos( opponentCard0Pos.x(), 
					  height()-nameDistance-mPlayerName->boundingRect().height()-nameDistance-mCardSize.height());*/
	//
	QPoint opponentCard0Pos( halpOfWidth-((float)mNumberOfCardsInHand/2)*mCardSize.width()-(mNumberOfCardsInHand/2)*cardDistance,
						nameDistance+mOpponentName->boundingRect().height()+nameDistance );
	QPoint playerCard0Pos( opponentCard0Pos.x(),
					  height()-nameDistance-mPlayerName->boundingRect().height()-nameDistance-mCardSize.height() );
	//
	
	
	mOpponentCards[0].setPos( opponentCard0Pos );
	mPlayerCards[0].setPos( playerCard0Pos );
	
	for(int i = 1; i < mNumberOfCardsInHand; ++i){
		opponentCard0Pos.rx() += cardDistance+mCardSize.width();
		mOpponentCards[i].setPos( opponentCard0Pos );
		
		playerCard0Pos.rx() += cardDistance+mCardSize.width();
		mPlayerCards[i].setPos( playerCard0Pos );
	}
	
	//Set position of score tables
	QPoint opponentScoreTablePos;
	opponentScoreTablePos.setX( mOpponentCards[ mNumberOfCardsInHand-1 ].scenePos().x()+mCardSize.width()+scoreTableDistance );
	opponentScoreTablePos.setY( mOpponentCards[ mNumberOfCardsInHand-1 ].scenePos().y() );
	mOpponentScoreTable->setPos( opponentScoreTablePos );
	
	QPoint playerScoreTablePos;
	playerScoreTablePos.setX( mPlayerCards[ mNumberOfCardsInHand-1 ].scenePos().x()+mCardSize.width()+scoreTableDistance );
	playerScoreTablePos.setY( mPlayerCards[ mNumberOfCardsInHand-1 ].scenePos().y()+mCardSize.height()-mPlayerScoreTable->boundingRect().height() );
	mPlayerScoreTable->setPos( playerScoreTablePos );
	
	//Set position of trump card
	QPoint trumpPos;
	trumpPos.setX( mDeck->scenePos().x()+mCardSize.width() );
	trumpPos.setY( mDeck->scenePos().y()-(mDeck->boundingRect().height()-mCardSize.height()) );
	
	mTrumpCard->setPos( trumpPos );
	
	//Set position of centralcards
	QPoint cenralCard1Pos;
	cenralCard1Pos.setX( halpOfWidth - mCardSize.width()/2 );
	cenralCard1Pos.setY( ( height()-mCardSize.height() ) / 2 );
	
	mCentralCards[0].setPos( cenralCard1Pos );
	
	QPoint cenralCard2Pos;
	cenralCard2Pos.setX( halpOfWidth );
	cenralCard2Pos.setY( ( height()-mCardSize.height() ) / 2 );

	mCentralCards[1].setPos( cenralCard2Pos );
	
	//Set position of twenty button
	QPoint twentyButtonPos;
	twentyButtonPos.setX( mPlayerCards[2].scenePos().x()-mCardSize.width()/2-buttonDistance );
	twentyButtonPos.setY( mPlayerCards[2].scenePos().y()-buttonDistance-20 );
	
	mTwentyButton->setGeometry( QRect( twentyButtonPos, QSize( mCardSize.width(), 20 ) ) );
	
	//Set position of forty button
	QPoint fortyButtonPos;
	fortyButtonPos.setX( mPlayerCards[2].scenePos().x()+mCardSize.width()/2+buttonDistance );
	fortyButtonPos.setY( mPlayerCards[2].scenePos().y()-buttonDistance-20 );
	
	mFortyButton->setGeometry( QRect( fortyButtonPos, QSize( mCardSize.width(), 20 ) ) );
	
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

void CentralWidget::slotInitialize( QString playerName, QString opponentName, Knapsen::TypeOfCards typeOfCards , int numberOfCardsInHand )
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
		//Under developing.
		mRenderer = new QSvgRenderer( KGlobal::dirs()->findResource( "appdata", "pics/tigullio-bridge.svg" ) );
		//
	}
	
	//
	if( typeOfCards == Knapsen::GermanSuits ){
		mScale = GERMAN_CARDS_SCALE_VALUE;
	}else{ //Knapsen::FrenchSuits
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
		
	scene()->addItem( mDeck );

	//Setup trump card
	mTrumpCard = new MySvgItem;
	mTrumpCard->setSharedRenderer( mRenderer );
	mTrumpCard->setElementId( "back" );
	//mTrumpCard->setScale( SCALE_VALUE );
	mTrumpCard->setScale( mScale );
	mTrumpCard->setTransformOriginPoint( 0, mTrumpCard->boundingRect().height()/2 );
	mTrumpCard->setRotation( 90 );
	mTrumpCard->setVisible( false );
	mTrumpCard->setSelectable( false );
	mTrumpCard->setZValue( -1 );
	
	connect( mTrumpCard, SIGNAL( click() ), this, SLOT( slotSelectedTrumpCardSlot() ) );

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
	
	//Setup score tables
	mOpponentScoreTable = new ScoreTable;
	mPlayerScoreTable = new ScoreTable;
	
	mOpponentScoreTable->setVisible( false );
	mPlayerScoreTable->setVisible( false );
	
	scene()->addItem( mOpponentScoreTable );
	scene()->addItem( mPlayerScoreTable );
	
	//Set twenty button
	QPushButton *twentyButton = new QPushButton( i18n( "Twenty" ) );
	mTwentyButton = scene()->addWidget( twentyButton );
	mTwentyButton->setVisible( false );
	connect( twentyButton, SIGNAL( clicked() ), this, SLOT( slotTwentyButtonClicked() ) );
	
	//Set forty button
	QPushButton *fortyButton = new QPushButton( i18n( "Forty" ) );
	mFortyButton = scene()->addWidget( fortyButton );
	mFortyButton->setVisible( false );
	connect( fortyButton, SIGNAL(clicked()), this, SLOT( slotFortyButtonClicked() ) );
	
	//Set close button
	QPushButton *closeButton = new QPushButton( i18n( "Close" ) );
	mCloseButton = scene()->addWidget( closeButton );
	mCloseButton->setVisible( false );
	connect( closeButton, SIGNAL( clicked() ), this, SLOT( slotCloseButtonClicked() ) ); 
}

void CentralWidget::slotNewPlayerCard( int id , QString card )
{
	mPlayerCards[ id ].setElementId( card );
	
	if( mGameIsRunning && !mPlayerCards[ id ].isVisible() ){
		mPlayerCards[ id ].setVisible( true );
	}
}

void CentralWidget::slotNewOpponentCardId( int id )
{
	if( mGameIsRunning ){
		mOpponentCards[ id ].setVisible( true );
	}
}

void CentralWidget::slotNewTrumpCard( QString card )
{
	mTrumpCard->setElementId( card );
	
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

void CentralWidget::slotPlayerCardSelectableChanged( int id , bool selectable )
{
	mPlayerCards[ id ].setSelectable( selectable );
}

void CentralWidget::slotCentralCardChanged( int id, QString cardText )
{
	if( cardText.isEmpty() ){
		mCentralCards[ id ].setElementId( cardText );
		mCentralCards[ id ].setVisible( false );
	}else{
		mCentralCards[ id ].setElementId( cardText );
		mCentralCards[ id ].setVisible( true );
	}
}

void CentralWidget::slotOpponentSelectedCardId( int id )
{
	mOpponentCards[ id ].setVisible( false );
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

void CentralWidget::slotShowOpponentCards( int card1Pos, QString card1Text, int card2Pos, QString card2Text )
{
	mOpponentCards[ card1Pos ].setElementId( card1Text );
	mOpponentCards[ card2Pos ].setElementId( card2Text );
	
	//mShowOpponentCardsId[0] = card1Pos;
	//mShowOpponentCardsId[1] = card2Pos;
	mShowOpponentCardsId = new QPair< int, int >;
	mShowOpponentCardsId->first = card1Pos;
	mShowOpponentCardsId->second = card2Pos;
	
	QTimer::singleShot( 1000, this, SLOT( slotCoverOpponentCards() ) );
}

void CentralWidget::slotNewRound()
{
	for( int i = 0; i < mNumberOfCardsInHand; ++i ){
		mOpponentCards[ i ].setVisible( false );
		mPlayerCards[ i ].setVisible( false );
	}
	
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
	
	for( int i = 0; i < mNumberOfCardsInHand; ++i ){
		mPlayerCards[ i ].setVisible( true );
		mOpponentCards[ i ].setVisible( true );
	}
	
	mOpponentScoreTable->setVisible( true );
	mPlayerScoreTable->setVisible( true );
	
	mTrumpCard->setVisible( true );

	mGameIsRunning = true;
}

void CentralWidget::slotClick( int id )
{
	emit signalSelectedCardId( id );
	mPlayerCards[ id ].setVisible( false );
}

void CentralWidget::slotCoverOpponentCards()
{
	//mOpponentCards[ mShowOpponentCardsId[0] ].setElementId( "back" );
	//mOpponentCards[ mShowOpponentCardsId[1] ].setElementId( "back" );
	mOpponentCards[ mShowOpponentCardsId->first ].setElementId( "back" );
	mOpponentCards[ mShowOpponentCardsId->second ].setElementId( "back" );
	
	delete mShowOpponentCardsId;
	mShowOpponentCardsId = 0;
	
	emit signalHideShowedOpponentCards();
}
