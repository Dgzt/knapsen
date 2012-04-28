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

const double SCALE_VALUE = 0.6; //0.7

/*CentralWidget::CentralWidget( QWidget* parent ) : QGraphicsView( parent )
{	
	//Set View's stuff
	setFrameStyle( QFrame::NoFrame );
	setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	
	//Set graphics scene
	QGraphicsScene* gScene = new QGraphicsScene;
	gScene->setItemIndexMethod( QGraphicsScene::NoIndex );
	setScene( gScene );
	
	gameIsRunning = false;
	
	QImage backgroundImage( KGlobal::dirs()->findResource( "appdata", "pics/background.png" ) );
	
	gScene->setBackgroundBrush( QBrush( backgroundImage ) );
	
	opponentName = 0;
	playerName = 0;
	
	renderer = new QSvgRenderer( KGlobal::dirs()->findResource( "appdata", "pics/william-tell.svgz" ) );
	
	cardDeckSvgItem = new QGraphicsSvgItem;
	//cardDeckSvgItem = new MySvgItem;
	cardDeckSvgItem->setSharedRenderer( renderer );
	cardDeckSvgItem->setElementId( "back" );
	
	cardDeckSvgItem->setScale( SCALE_VALUE );
	
	//cardSize = cardDeckSvgItem->boundingRect()*SCALE_VALUE;
	cardSize = cardDeckSvgItem->boundingRect().size()*SCALE_VALUE;
	
	opponentCards = 0;
	playerCards = 0;
	
	opponentScoreTable = 0;
	playerScoreTable = 0;
	
	trumpCard = 0;
	
	centralCards = 0;
	
	closeButton = 0;
	twentyButton = 0;
	fortyButton = 0;
	
	gScene->addItem( cardDeckSvgItem );
}*/

CentralWidget::CentralWidget( QWidget* parent ): 
	QGraphicsView( parent )
{
	mGameIsRunning = false;
	mOpponentName = 0;
	mPlayerName = 0;
	mRenderer = 0;
	mOpponentCards = 0;
	mNumberOfCardsInHand = 0;
	mDeck = 0;
	mPlayerCards = 0;
	mTrumpCard = 0;
	mCentralCards = 0;
	
	mOpponentScoreTable = 0;
	mPlayerScoreTable = 0;
	
	mTwentyButton = 0;
	mFortyButton = 0;
	
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

/*CentralWidget::~CentralWidget()
{
	//delete cardDeck;
	delete renderer;
	delete cardDeckSvgItem;
	
	if( opponentCards )
		delete[] opponentCards;
	
	if( playerCards )
		delete[] playerCards;
	
}*/

CentralWidget::~CentralWidget()
{

}


/*void CentralWidget::clearWidget()
{
	kDebug() << "Clear widget.";
	
	gameIsRunning = false;
	
	//Remove items from the scene
	for( int i = 0; i < 5; ++i ){
		scene()->removeItem( &opponentCards[i] );
		scene()->removeItem( &playerCards[i] );
	}
	
	scene()->removeItem( opponentName );
	scene()->removeItem( playerName );
	scene()->removeItem( trumpCard );
	scene()->removeItem( centralCards );
	
	delete[] opponentCards;
	delete[] playerCards;
	delete opponentName;
	delete playerName;
	delete opponentScoreTable;
	delete playerScoreTable;
	delete trumpCard;
	delete[] centralCards;
	delete closeButton;
	delete twentyButton;
	delete fortyButton;
	
	opponentCards = 0;
	playerCards = 0;
	opponentName = 0;
	playerName = 0;
	opponentScoreTable = 0;
	playerScoreTable = 0;
	trumpCard = 0;
	centralCards = 0;
	twentyButton = 0;
	closeButton = 0;
	
	fortyButton = 0;
	
	if( !cardDeckSvgItem->isVisible() )
		cardDeckSvgItem->setVisible( true );
}*/

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
}

/*void CentralWidget::setInGamePositions()
{
	const int halpOfWidth  = width()/2;
	const int cardDistance = 10;
	const int nameDistance = 5;
	const int scoreTableDistance = 5;
	//
	const int buttonDistance = 5;
	//
	
	//Set position of player's name
	QPoint playerNamePos;
	playerNamePos.setX( halpOfWidth - playerName->boundingRect().width()/2 );
	playerNamePos.setY( height() - nameDistance -playerName->boundingRect().height() );
	playerName->setPos( playerNamePos );
	
	//Set position of opponent's name
	playerNamePos.setX( halpOfWidth - opponentName->boundingRect().width()/2 );
	playerNamePos.setY( nameDistance );
	opponentName->setPos( playerNamePos );

	//Set position of opponent's cards and player's cards
	QPoint posOpponent( halpOfWidth-cardSize.width()/2-2*cardSize.width()-2*cardDistance, 
						nameDistance+opponentName->boundingRect().height()+nameDistance );
	QPoint posPlayer( posOpponent.x(), 
					  height()-nameDistance-playerName->boundingRect().height()-nameDistance-cardSize.height());
	
	opponentCards[0].setPos( posOpponent );
	playerCards[0].setPos( posPlayer );
	
	for(int i = 1; i < 5; ++i){
		posOpponent.rx() += cardDistance+cardSize.width();
		opponentCards[i].setPos( posOpponent );
		
		posPlayer.rx() += cardDistance+cardSize.width();
		playerCards[i].setPos( posPlayer );
	}
	
	//Set position of trump card
	QPoint trumpPos;
	trumpPos.setX( cardDeckSvgItem->scenePos().x()+cardSize.width() );
	trumpPos.setY( cardDeckSvgItem->scenePos().y()-(cardDeckSvgItem->boundingRect().height()-cardSize.height()) );
	
	trumpCard->setPos( trumpPos );	
	
	//Set position of centralcards
	QPoint cenralCard1Pos;
	cenralCard1Pos.setX( halpOfWidth - cardSize.width()/2 );
	cenralCard1Pos.setY( ( height()-cardSize.height() ) / 2 );
	
	centralCards[0].setPos( cenralCard1Pos );
	
	QPoint cenralCard2Pos;
	cenralCard2Pos.setX( halpOfWidth );
	cenralCard2Pos.setY( ( height()-cardSize.height() ) / 2 );

	centralCards[1].setPos( cenralCard2Pos );
	
	//Set position of score tables
	QPoint opponentScoreTablePos;
	opponentScoreTablePos.setX( opponentCards[4].scenePos().x()+cardSize.width()+scoreTableDistance );
	opponentScoreTablePos.setY( opponentCards[4].scenePos().y() );
	
	opponentScoreTable->setPos( opponentScoreTablePos );
	
	QPoint playerScoreTablePos;
	playerScoreTablePos.setX( playerCards[4].scenePos().x()+cardSize.width()+scoreTableDistance );
	playerScoreTablePos.setY( playerCards[4].scenePos().y()+cardSize.height()-playerScoreTable->boundingRect().height() );
	
	playerScoreTable->setPos( playerScoreTablePos );
	
	//
	//setTrumpCardPosition();
	QPoint trumpCardPos;
	trumpCardPos.setX( cardDeckSvgItem->scenePos().x()+cardSize.width() );
	trumpCardPos.setY( cardDeckSvgItem->scenePos().y()-(cardDeckSvgItem->boundingRect().height()-cardSize.height()) );
	
	trumpCard->setPos( trumpCardPos );
	
	//Set position of close button
	QPoint closeButtonPos;
	closeButtonPos.setX( cardDeckSvgItem->scenePos().x() );
	closeButtonPos.setY( cardDeckSvgItem->scenePos().y()+cardSize.height()+10 );
	
	closeButton->setGeometry( QRect( closeButtonPos, QSize( cardSize.width(), 20 ) ) );
	
	//Set position of twenty button
	QPoint twentyButtonPos;
	twentyButtonPos.setX( playerCards[2].scenePos().x()-cardSize.width()/2-buttonDistance );
	twentyButtonPos.setY( playerCards[2].scenePos().y()-buttonDistance-20 );
	
	twentyButton->setGeometry( QRect( twentyButtonPos, QSize( cardSize.width(), 20 ) ) );
	
	//Set position of forty button
	QPoint fortyButtonPos;
	fortyButtonPos.setX( playerCards[2].scenePos().x()+cardSize.width()/2+buttonDistance );
	fortyButtonPos.setY( playerCards[2].scenePos().y()-buttonDistance-20 );
	
	fortyButton->setGeometry( QRect( fortyButtonPos, QSize( cardSize.width(), 20 ) ) );
}*/

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
	QPoint posOpponent( halpOfWidth-mCardSize.width()/2-2*mCardSize.width()-2*cardDistance, 
						nameDistance+mOpponentName->boundingRect().height()+nameDistance );
	QPoint posPlayer( posOpponent.x(), 
					  height()-nameDistance-mPlayerName->boundingRect().height()-nameDistance-mCardSize.height());
	
	mOpponentCards[0].setPos( posOpponent );
	mPlayerCards[0].setPos( posPlayer );
	
	for(int i = 1; i < mNumberOfCardsInHand; ++i){
		posOpponent.rx() += cardDistance+mCardSize.width();
		mOpponentCards[i].setPos( posOpponent );
		
		posPlayer.rx() += cardDistance+mCardSize.width();
		mPlayerCards[i].setPos( posPlayer );
	}
	
	//Set position of score tables
	QPoint opponentScoreTablePos;
	opponentScoreTablePos.setX( mOpponentCards[4].scenePos().x()+mCardSize.width()+scoreTableDistance );
	opponentScoreTablePos.setY( mOpponentCards[4].scenePos().y() );
	mOpponentScoreTable->setPos( opponentScoreTablePos );
	
	QPoint playerScoreTablePos;
	playerScoreTablePos.setX( mPlayerCards[4].scenePos().x()+mCardSize.width()+scoreTableDistance );
	playerScoreTablePos.setY( mPlayerCards[4].scenePos().y()+mCardSize.height()-mPlayerScoreTable->boundingRect().height() );
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

/*void CentralWidget::initializeSlot()
{
	kDebug() << "InitializeSlot";

	const int cardNum = 5;
	
	opponentCards = new QGraphicsSvgItem[ cardNum ];
	playerCards = new MySvgItem[ cardNum ];
	
	for( int i = 0; i < cardNum; ++i ){
		//Set opponent's cards
		opponentCards[i].setSharedRenderer( renderer );
		opponentCards[i].setElementId( "back" );
		opponentCards[i].setScale( SCALE_VALUE );
		opponentCards[i].setVisible( false );
		scene()->addItem( &opponentCards[i] );
		
		//Set player's cards
		playerCards[i].setSharedRenderer( renderer );
		playerCards[i].setElementId( "back" );
		playerCards[i].setScale( SCALE_VALUE );
		playerCards[i].setVisible( false );
		//playerCards[i].setEnabled( false );
		playerCards[i].setSelectable( false );
		playerCards[i].setId( i );
		scene()->addItem( &playerCards[i] );

		connect( &playerCards[i], SIGNAL(click(int)), this, SLOT(clickSlot(int)));
	}
	
	//set the centralcards
	centralCards = new QGraphicsSvgItem[ CENTRALCARDS_SIZE ];
	for( int i = 0; i < CENTRALCARDS_SIZE; ++i ){
		centralCards[i].setSharedRenderer( renderer );
		centralCards[i].setScale( SCALE_VALUE );
		centralCards[i].setVisible( false );
		centralCards[i].setElementId( "back" );
	
		scene()->addItem( &centralCards[i] );
	}
	
	// TMP Set trump card
	trumpCard = new MySvgItem;
	trumpCard->setSharedRenderer( renderer );
	trumpCard->setElementId( "back" );
	trumpCard->setScale( SCALE_VALUE );
	trumpCard->setTransformOriginPoint( 0, trumpCard->boundingRect().height()/2 );
	trumpCard->setRotation( 90 );
	trumpCard->setVisible( false );
	trumpCard->setSelectable( false );
	
	connect( trumpCard, SIGNAL(click(int)),this,SLOT(selectedTrumpCardSlot()));

	scene()->addItem( trumpCard );
	trumpCard->setZValue(0);
	cardDeckSvgItem->setZValue( 1 );
	//
	
	//set the score tables
	opponentScoreTable = new ScoreTable;
	playerScoreTable = new ScoreTable;
	
	opponentScoreTable->setVisible( false );
	playerScoreTable->setVisible( false );
	
	scene()->addItem( opponentScoreTable );
	scene()->addItem( playerScoreTable );
	
	//Set close button
	QPushButton* closePushButton = new QPushButton( i18n( "Close" ) );
	closeButton = scene()->addWidget( closePushButton );
	closeButton->setVisible( false );
	connect( closePushButton, SIGNAL(clicked()), this, SLOT(closeButtonSlot()));
	
	//Set twenty button
	QPushButton* twentyPushButton = new QPushButton( i18n( "Twenty" ) );
	twentyButton = scene()->addWidget( twentyPushButton );
	twentyButton->setVisible( false );
	connect( twentyPushButton, SIGNAL(clicked()), this, SLOT(twentyButtonSlot()));
	
	//Set forty button
	QPushButton* fortyPushButton = new QPushButton( i18n( "Forty" ) );
	fortyButton = scene()->addWidget( fortyPushButton );
	fortyButton->setVisible( false );
	connect( fortyPushButton, SIGNAL(clicked()), this, SLOT( fortyButtonSlot()));
}*/

/*void CentralWidget::trumpCardSlot( QString cardText )
{
	if( !cardText.isEmpty() ){
		trumpCard->setElementId( cardText );
	}else{
		trumpCard->setVisible( false );
	}
	
	//If the trump card is changed, then put under the deck
	cardDeckSvgItem->setZValue( 1 );
	trumpCard->setZValue( 0 );
}*/

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
	}
	
	//Set opponent's and player's cards
	mNumberOfCardsInHand = numberOfCardsInHand;
	
	mOpponentCards = new QGraphicsSvgItem[ mNumberOfCardsInHand ];
	mPlayerCards = new MySvgItem[ mNumberOfCardsInHand ];
	
	for( int i = 0; i < mNumberOfCardsInHand; ++i ){
		mOpponentCards[i].setSharedRenderer( mRenderer );
		mOpponentCards[i].setElementId( "back" );
		mOpponentCards[i].setScale( SCALE_VALUE );
		mOpponentCards[i].setVisible( false );
		
		scene()->addItem( &mOpponentCards[i] );
		
		mPlayerCards[i].setSharedRenderer( mRenderer );
		mPlayerCards[i].setElementId( "back" );
		mPlayerCards[i].setScale( SCALE_VALUE );
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
	mDeck->setScale( SCALE_VALUE );
		
	scene()->addItem( mDeck );

	//Setup trump card
	mTrumpCard = new MySvgItem;
	mTrumpCard->setSharedRenderer( mRenderer );
	mTrumpCard->setElementId( "back" );
	mTrumpCard->setScale( SCALE_VALUE );
	mTrumpCard->setTransformOriginPoint( 0, mTrumpCard->boundingRect().height()/2 );
	mTrumpCard->setRotation( 90 );
	mTrumpCard->setVisible( false );
	mTrumpCard->setSelectable( false );
	mTrumpCard->setZValue( -1 );

	scene()->addItem( mTrumpCard );
	
	//Setup central cards
	mCentralCards = new QGraphicsSvgItem[ CENTRAL_CARDS_SIZE ];
	for( int i = 0; i < CENTRAL_CARDS_SIZE; ++i ){
		mCentralCards[i].setSharedRenderer( mRenderer );
		mCentralCards[i].setScale( SCALE_VALUE );
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
	QPushButton* twentyPushButton = new QPushButton( i18n( "Twenty" ) );
	mTwentyButton = scene()->addWidget( twentyPushButton );
	mTwentyButton->setVisible( false );
	connect( twentyPushButton, SIGNAL( clicked() ), this, SLOT( slotTwentyButtonClicked() ) );
	
	//Set forty button
	QPushButton* fortyPushButton = new QPushButton( i18n( "Forty" ) );
	mFortyButton = scene()->addWidget( fortyPushButton );
	mFortyButton->setVisible( false );
	connect( fortyPushButton, SIGNAL(clicked()), this, SLOT( slotFortyButtonClicked() ) );
	
}

void CentralWidget::slotNewPlayerCard( int id , QString card )
{
	mPlayerCards[ id ].setElementId( card );
	
	if( mGameIsRunning ){
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
	kDebug() << card;
	
	/*if( !card.isEmpty() ){
		mTrumpCard->setElementId( card );
	}else{
		mTrumpCard->setVisible( false );
		mTrumpCard->setElementId( card );
	}*/
	mTrumpCard->setElementId( card );
	
	if( !mTrumpCard->isVisible() ){
		mTrumpCard->setVisible( true );
	}
}

void CentralWidget::slotTrumpCardHide()
{
	mTrumpCard->setVisible( false );
}

void CentralWidget::slotPlayerCardSelectableChanged( int id , bool selectable )
{
	mPlayerCards[ id ].setSelectable( selectable );
}

/*void CentralWidget::slotNewCentralCard( int id, QString cardText )
{
	mCentralCards[ id ].setElementId( cardText );
	mCentralCards[ id ].setVisible( true );
}*/

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

void CentralWidget::slotPlayerTricksChanged( int tricks )
{
	mPlayerScoreTable->setTricks( tricks );
}

void CentralWidget::slotDeckVisible( bool visible )
{
	mDeck->setVisible( visible );
}

void CentralWidget::slotTwentyButtonVisible( bool visible )
{
	mTwentyButton->setVisible( visible );
}

void CentralWidget::slotFortyButtonVisible( bool visible )
{
	mFortyButton->setVisible( visible );
}

void CentralWidget::slotStartGame()
{
	kDebug() << "Start game.";
	
	mCardSize = mDeck->boundingRect().size()*SCALE_VALUE;
	
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
	
	//
	//mTwentyButton->setVisible( true );
	//mFortyButton->setVisible( true );
	//
	
	mGameIsRunning = true;
}

void CentralWidget::slotClick( int id )
{
	emit signalSelectedCardId( id );
	mPlayerCards[ id ].setVisible( false );
}

/*void CentralWidget::closeButtonSlot()
{
	trumpCard->setElementId( "back" );
	
	cardDeckSvgItem->setZValue( 0 );
	trumpCard->setZValue( 1 );
	
	emit signalCloseDeck();
}*/

/*void CentralWidget::opponentScoresChanged( int scores )
{
	opponentScoreTable->setScores( scores );
}*/

/*void CentralWidget::playerScoresChanged( int scores )
{
	playerScoreTable->setScores( scores );
}*/

/*void CentralWidget::enabledCloseButtonSlot( bool enabled )
{
	closeButton->setVisible( enabled );
}*/

/*void CentralWidget::enabledTwentyButtonSlot( bool enabled )
{
	twentyButton->setVisible( enabled );
}*/

/*void CentralWidget::enabledFortyButtonSlot(bool enabled )
{
	fortyButton->setVisible( enabled );
}*/

/*void CentralWidget::selectableTrumpCard( bool selectable )
{
	trumpCard->setSelectable( selectable );
}*/
