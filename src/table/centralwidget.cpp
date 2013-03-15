#include <KDE/KGlobal>
#include <KDE/KStandardDirs>
#include <KDE/KDebug>
#include <KDE/KLocalizedString>
#include <QtSvg/QSvgRenderer>
#include <QtCore/QPointF>
#include <QtCore/QRectF>
//
#include <QtCore/QTimeLine>
//
#include <QtGui/QPushButton>
#include <QtGui/QGraphicsProxyWidget>
#include "engine/client.h"
#include "table/mytextitem.h"
#include "table/scoretable.h"
#include "table/svgcard.h"
#include "table/animation.h"
#include "table/cardsgroup.h"
#include "table/centralwidget.h"

//
#include <QtCore/QTimer>
//

const int SCENE_WIDTH = 1000; //1000
const int SCENE_HEIGHT = 850; //1000

const int NAME_DISTANCE = 10;
const int CARDS_SCORE_TABLE_DISTANCE = 10;

const int DECK_TRUMPCARD_DISTANCE = 30;

const int BUTTON_Y_DISTANCE = 10;
const int BUTTON_WIDTH = 100;
const int BUTTON_HEIGHT = 30;

//
const int NAME_ANIMATION_TIME = 2000; //2000
const int SCORE_TABLE_ANIMATION_TIME = 2000; //2000
const int DECK_ANIMATION_TIME = 2000; //2000
//
const int DECK_DELAY_TIME = 500; //500
//
//const int TRUMP_ANIMATION_TIME = 1000; //2000
const int CARD_ANIMATION_TIME = 1000; //100
//

//
const QString SCHNAPSEN_TEXT = "Schnapsen";
const QString FORTY_TEXT = "Forty";
const QString TWENTY_TEXT = "Twenty";
//

CentralWidget::CentralWidget( QWidget* parent ): 
    QGraphicsView( parent )
{
    //
    mTrumpCard = 0;
    
    mRemoveCards = 0;
    //
    
    //
    mOpponentSchnapsenText = 0;
    mOpponentFortyText = 0;
    mOpponentTwentyText = 0;
    //
    
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
    mOpponentName = new MyTextItem( opponentNameStr, NAME_ANIMATION_TIME );
    mOpponentName->setVisible( false );
    
    scene()->addItem( mOpponentName );
    
    //Setup opponent's score table
    mOpponentScoreTable = new ScoreTable( SCORE_TABLE_ANIMATION_TIME );
    mOpponentScoreTable->setVisible( false );
    
    scene()->addItem( mOpponentScoreTable );
    
    //Setup player's name
    mPlayerName = new MyTextItem( playerNameStr, NAME_ANIMATION_TIME );
    mPlayerName->setVisible( false );
    
    connect( mClient, SIGNAL( signalOpponentTricksChanged( int ) ), mOpponentScoreTable, SLOT( slotTricksChanged( int ) ) );
    connect( mClient, SIGNAL( signalOpponentScoresChanged( int ) ), mOpponentScoreTable, SLOT( slotScoresChanged( int ) ) );
    
    scene()->addItem( mPlayerName );
    
    //Setup player's score table
    mPlayerScoreTable = new ScoreTable( SCORE_TABLE_ANIMATION_TIME );
    mPlayerScoreTable->setVisible( false );
    
    connect( mClient, SIGNAL( signalPlayerTricksChanged( int ) ), mPlayerScoreTable, SLOT( slotTricksChanged( int ) ) );
    connect( mClient, SIGNAL( signalPlayerScoresChanged( int ) ), mPlayerScoreTable, SLOT( slotScoresChanged( int ) ) );
    
    scene()->addItem( mPlayerScoreTable );
    
    //Setup type of cards
    if( typeOfCards == Knapsen::GermanSuits ){
        kDebug() << "Game with german suits cards.";
        mRenderer = new QSvgRenderer( KGlobal::dirs()->findResource( "appdata", "pics/william-tell.svgz" ) );
    }else{ // typeOfCards == Knapsen::GermanSuits
        kDebug() << "Game with french suits cards.";
        mRenderer = new QSvgRenderer( KGlobal::dirs()->findResource( "appdata", "pics/tigullio-bridge.svg" ) );
    }
    
    //Setup Deck
    mDeck = new SvgCard( mRenderer, DECK_ANIMATION_TIME );
    mDeck->setVisible( false );
    
    scene()->addItem( mDeck );
    
    //Setup trump card
    connect( mClient, SIGNAL( signalNewTrumpCard( Card* ) ), this, SLOT( slotNewTrumpCard( Card* ) ) );
    connect( mClient, SIGNAL( signalTrumpCardSelectableChanged( bool ) ), this, SLOT( slotTrumpCardSelectableChanged( bool ) ) );
    connect( mClient, SIGNAL( signalPlayerChangeTrumpCard( int ) ), this, SLOT( slotPlayerChangeTrumpCard( int ) ) );
    
    //Setup Opponent's cards
    mOpponentCards = new CardsGroup;
    
    connect( mClient, SIGNAL( signalNewOpponentCard( bool ) ), this, SLOT( slotNewOpponentCard( bool ) ) );
    connect( mClient, SIGNAL( signalNewOpponentCardTrumpCard() ), this, SLOT( slotNewOpponentCardTrumpCard() ) );
    connect( mClient, SIGNAL( signalOpponentSelectedCard( int, Card* ) ), mOpponentCards, SLOT( slotSelectedCard( int, Card* ) ) );
    connect( mClient, SIGNAL( signalShowOpponentCards( int, Card, int, Card ) ), mOpponentCards, SLOT( slotShowCards( int, Card, int, Card ) ) );
    connect( mClient, SIGNAL( signalOpponentChangeTrumpCard( int, Card* ) ), this, SLOT( slotOpponentChangeTrumpCard( int, Card* ) ) );
    connect( mOpponentCards, SIGNAL( signalSizeChanged() ), this, SLOT( slotOpponentCardsSizeChanged() ) );
    connect( mOpponentCards, SIGNAL( signalCardArrived() ), mClient, SLOT( slotProcessCommands() ) );
    connect( mOpponentCards, SIGNAL( signalHideCards() ), mClient, SLOT( slotProcessCommands() ) );
    
    //Setup Player's cards
    mPlayerCards = new CardsGroup;
    
    connect( mClient, SIGNAL( signalNewPlayerCard( bool, Card* ) ), this, SLOT( slotNewPlayerCard( bool, Card* ) ) );
    connect( mClient, SIGNAL( signalNewPlayerCardTrumpCard() ), this, SLOT( slotNewPlayerCardTrumpCard() ) );
    connect( mClient, SIGNAL( signalPlayerCardSelectableChanged( int , bool ) ), mPlayerCards, SLOT( slotSelectableChanged( int , bool ) ) );
    connect( mPlayerCards, SIGNAL( signalSizeChanged() ), this, SLOT( slotPlayerCardsSizeChanged() ) );
    connect( mPlayerCards, SIGNAL( signalCardArrived() ), mClient, SLOT( slotProcessCommands() ) );
    connect( mPlayerCards, SIGNAL( signalSelectedCardId( int ) ), mClient, SLOT( slotSelectCardId( int ) ) );
    connect( mPlayerCards, SIGNAL( signalSelectedCardId( int ) ), this, SLOT( slotPlayerSelectedCard() ) );
    
    //Setup central cards
    mCentralCards = new CardsGroup;
    
    connect( mCentralCards, SIGNAL( signalSizeChanged() ), this, SLOT( slotCentralCardsSizeChanged() ) ); 
    connect( mPlayerCards, SIGNAL( signalSelectedCard( SvgCard* ) ), mCentralCards, SLOT( slotAddNewCard( SvgCard* ) ) );
    connect( mOpponentCards, SIGNAL( signalSelectedCard( SvgCard* ) ), mCentralCards, SLOT( slotAddNewCard( SvgCard* ) ) );
    connect( mCentralCards, SIGNAL( signalCardArrived() ), mClient, SLOT( slotProcessCommands() ) );
    connect( mClient, SIGNAL( signalPlayerGetCentralCards() ), this, SLOT( slotPlayerGetCentralCards() ) );
    connect( mClient, SIGNAL( signalOpponentGetCentralCards() ), this, SLOT( slotOpponentGetCentralCards() ) );
    
    //Close button
    QPushButton *closeButton = new QPushButton( i18n( "Close" ) );
    closeButton->setAttribute( Qt::WA_NoSystemBackground );
    mCloseButton = scene()->addWidget( closeButton );
    mCloseButton->setVisible( false );
    
    connect( mClient, SIGNAL( signalCloseButtonVisible( bool ) ), this, SLOT( slotCloseButtonVisible( bool ) ) );
    connect( mClient, SIGNAL( signalCloseDeck() ), this, SLOT( slotCloseDeck() ) );
    connect( closeButton, SIGNAL( clicked() ), mClient, SLOT( slotCloseButtonClicked() ) );
    
    //Schnapsen button
    QPushButton* schnapsenButton = new QPushButton( i18n( "Schnapsen" ) );
    schnapsenButton->setAttribute( Qt::WA_NoSystemBackground );
    mSchnapsenButton = scene()->addWidget( schnapsenButton );
    mSchnapsenButton->setVisible( false );
    
    connect( mClient, SIGNAL( signalSchnapsenButtonVisible( bool ) ), this, SLOT( slotSchnapsenButtonVisible( bool ) ) );
    connect( schnapsenButton, SIGNAL( clicked() ), mClient, SLOT( slotSchnapsenButtonClicked() ) );
    
    //Twenty button
    QPushButton* twentyButton = new QPushButton( i18n( "Twenty" ) );
    twentyButton->setAttribute( Qt::WA_NoSystemBackground );
    mTwentyButton = scene()->addWidget( twentyButton );
    mTwentyButton->setVisible( false );
    
    connect( mClient, SIGNAL( signalTwentyButtonVisible( bool ) ), this, SLOT( slotTwentyButtonVisible( bool ) ) );
    connect( twentyButton, SIGNAL( clicked() ), mClient, SLOT( slotTwentyButtonClicked() ) );
    
    //Forty button
    QPushButton* fortyButton = new QPushButton( i18n( "Forty" ) );
    fortyButton->setAttribute( Qt::WA_NoSystemBackground );
    mFortyButton = scene()->addWidget( fortyButton );
    mFortyButton->setVisible( false );
    
    connect( mClient, SIGNAL( signalFortyButtonVisible( bool ) ), this, SLOT( slotFortyButtonVisible( bool ) ) );
    connect( fortyButton, SIGNAL( clicked() ), mClient, SLOT( slotFortyButtonClicked() ) );
    
    //Arrows
    QPixmap arrowImage( KGlobal::dirs()->findResource( "appdata", "pics/arrow.png" ) );
    mPlayerArrow = scene()->addPixmap( arrowImage );
    mOpponentArrow = scene()->addPixmap( arrowImage );
    
    mPlayerArrow->setVisible( false );
    mOpponentArrow->setVisible( false );
    
    connect( mClient, SIGNAL( signalPlayerInAction() ), this, SLOT( slotShowPlayerArrow() ) );
    connect( mPlayerCards, SIGNAL( signalSelectedCardId(int) ), this, SLOT( slotHidePlayerArrow() ) );
    connect( mClient, SIGNAL( signalOpponentInAction() ), this, SLOT( slotShowOpponentArrow() ) );
    connect( mOpponentCards, SIGNAL( signalSelectedCard( SvgCard* ) ), this, SLOT( slotHideOpponentArrow() ) );
    
    //Opponent's texts
    mOpponentSchnapsenText = new MyTextItem( SCHNAPSEN_TEXT );
    mOpponentSchnapsenText->setVisible( false );
    scene()->addItem( mOpponentSchnapsenText );
    
    connect( mClient, SIGNAL( signalOpponentSchnapsenButtonClicked() ), this, SLOT( slotOpponentSchnapsenButtonClicked() ) );

    mOpponentFortyText = new MyTextItem( FORTY_TEXT );
    mOpponentFortyText->setVisible( false );
    scene()->addItem( mOpponentFortyText );
    
    connect( mClient, SIGNAL( signalOpponentFortyButtonClicked() ), this, SLOT( slotOpponentFortyButtonClicked() ) );
    
    mOpponentTwentyText = new MyTextItem( TWENTY_TEXT );
    mOpponentTwentyText->setVisible( false );
    scene()->addItem( mOpponentTwentyText );
    
    connect( mClient, SIGNAL( signalOpponentTwentyButtonClicked() ), this, SLOT( slotOpponentTwentyButtonClicked() ) );
    
    //Set timer
    mTimer = new QTimer;
    mTimer->setSingleShot( true );
    
    connect( mTimer, SIGNAL( timeout() ), mClient, SLOT( slotProcessCommands() ) );
    
    //Start game signal-slot
    connect( mClient, SIGNAL( signalNewGame() ), this, SLOT( slotNewGame() ) );
    connect( mClient, SIGNAL( signalNewRound() ), this, SLOT( slotNewRound() ) );
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
    
    //Setup opponent's cards pos
    mOpponentCards->setPos( sceneRect().width() / 2,
                            endOpponentNamePos.y() + mOpponentName->boundingRect().height() + NAME_DISTANCE );
    
    //Opponent's score table
    mOpponentScoreTable->setPos( mapToScene( width(), height() ).x(),
                                 mOpponentCards->y() );
    
    QPointF endOpponentScoreTablePos( mOpponentCards->x() + CARDS_SCORE_TABLE_DISTANCE,
                                      mOpponentScoreTable->y() );
    
    mOpponentScoreTable->getAnimation()->setEndPosition( endOpponentScoreTablePos );
    mOpponentScoreTable->setVisible( true );
    mOpponentScoreTable->getAnimation()->startAnimation();
    
    
    //Player's name
    mPlayerName->setPos( ( sceneRect().width() - mPlayerName->boundingRect().width() ) /2,
                         mapToScene( width(), height() ).y() + mPlayerName->boundingRect().height() );
    
    QPointF endPlayerNamePos( mPlayerName->x(),
                              sceneRect().height() - NAME_DISTANCE - mPlayerName->boundingRect().height()  );
    
    mPlayerName->getAnimation()->setEndPosition( endPlayerNamePos );
    mPlayerName->setVisible( true );
    
    //connect( mPlayerName, SIGNAL( signalMyTextItemAnimationEnd() ), this, SLOT( slotNewRound() ) );
    
    mPlayerName->getAnimation()->startAnimation();
    
    //Setup player's cards pos
    mPlayerCards->setPos( sceneRect().width() / 2,
                          endPlayerNamePos.y() - NAME_DISTANCE - mDeck->boundingRect().height() );
    
    //Player's score table
    mPlayerScoreTable->setPos( mOpponentScoreTable->x(),
                               endPlayerNamePos.y() - NAME_DISTANCE - mPlayerScoreTable->boundingRect().height() );
    
    QPointF endPlayerScoreTablePos( mPlayerCards->x() + CARDS_SCORE_TABLE_DISTANCE,
                                    mPlayerScoreTable->y() );
    
    mPlayerScoreTable->getAnimation()->setEndPosition( endPlayerScoreTablePos );
    mPlayerScoreTable->setVisible( true );
    mPlayerScoreTable->getAnimation()->startAnimation();
    
    //
    mCentralCards->setPos( sceneRect().width() / 2,
                           ( sceneRect().height() - mDeck->boundingRect().height() ) / 2 );
    
    //QTimer::singleShot( mPlayerName->getAnimation()->timeLine()->duration(), mClient, SLOT( slotProcessCommands() ) );
    mTimer->start( NAME_ANIMATION_TIME );
}

void CentralWidget::slotNewRound()
{
    kDebug();
    
    //If this round isn't the first round then clear the table
    if( !mOpponentCards->isEmpty() ){
        mOpponentCards->clear();
    }
    
    if( !mPlayerCards->isEmpty() ){
        mPlayerCards->clear();
    }
    
    if( !mCentralCards->isEmpty() ){
        mCentralCards->clear();
    }
    
    if( mTrumpCard ){
        delete mTrumpCard;
        mTrumpCard = 0;
    }
    
    if( mOpponentSchnapsenText->isVisible() ){
        mOpponentSchnapsenText->hide();
    }
    
    QPointF startDeckPos( mapToScene( 0, 0 ).x() - mDeck->boundingRect().width(),
                          ( sceneRect().height() - mDeck->boundingRect().height() ) / 2 );
    
    QPointF endDeckPos( 50 , 
                        ( sceneRect().height() - mDeck->boundingRect().height() ) / 2 );
    
    mDeck->setPos( startDeckPos );
    mDeck->getAnimation()->setEndPosition( endDeckPos );
    
    //connect( mDeck->getAnimation(), SIGNAL( signalAnimationEnd() ), mClient, SLOT( slotProcessCommands() ) );
    
    mDeck->setVisible( true );
    
    mDeck->getAnimation()->startAnimation();
    
    //Set close button position
    mCloseButton->setGeometry( QRectF( QPointF( endDeckPos.x(), endDeckPos.y() + mDeck->boundingRect().height() + BUTTON_Y_DISTANCE ), 
                                       QSizeF( mDeck->boundingRect().width(), BUTTON_HEIGHT ) ) );

    //QTimer::singleShot( mDeck->getAnimation()->timeLine()->duration() + DECK_DELAY_TIME, mClient, SLOT( slotProcessCommands() ) );
    mTimer->start( DECK_ANIMATION_TIME + DECK_DELAY_TIME );
}

void CentralWidget::slotNewOpponentCard( bool lastCard )
{
    kDebug();
    
    SvgCard* svgCard = new SvgCard( mRenderer, CARD_ANIMATION_TIME );
    svgCard->setPos( mDeck->pos() );
    scene()->addItem( svgCard );
    
    //
    if( lastCard ){
        mDeck->setVisible( false );
    }
    //
    
    mOpponentCards->slotAddNewCard( svgCard );
}

void CentralWidget::slotNewOpponentCardTrumpCard()
{
    kDebug();
    
    //mTrumpCard->disconnect();
    
    mTrumpCard->setElementId( SvgCard::backCardText() );
    mOpponentCards->slotAddNewCard( mTrumpCard );
    
    mTrumpCard = 0;
}

void CentralWidget::slotNewPlayerCardTrumpCard()
{
    kDebug();
    
    //mTrumpCard->disconnect();
    
    mPlayerCards->slotAddNewCard( mTrumpCard );
    
    mTrumpCard = 0;
}

void CentralWidget::slotNewPlayerCard( bool lastCard, Card* card )
{
    kDebug();
    
    SvgCard* svgCard = new SvgCard( mRenderer, CARD_ANIMATION_TIME );
    //svgCard->getAnimation()->setCard( card );
    svgCard->getAnimation()->setNewCardText( card->getCardText() );
    svgCard->setPos( mDeck->pos() );
    scene()->addItem( svgCard );
    
    if( lastCard ){
        mDeck->setVisible( false );
    }
    
    mPlayerCards->slotAddNewCard( svgCard );
}

void CentralWidget::slotNewTrumpCard( Card* card )
{
    kDebug();
    
    mTrumpCard = new SvgCard( mRenderer, CARD_ANIMATION_TIME );

    connect( mTrumpCard, SIGNAL( signalClick( SvgCard* ) ), mClient, SLOT( slotSelectTrumpCard() ) );
    
    mTrumpCard->setPos( mDeck->pos() );
    
    QPointF endTrumpCardPos( mDeck->pos().x() + mDeck->boundingRect().width() + DECK_TRUMPCARD_DISTANCE, mDeck->pos().y() );
    
    mTrumpCard->getAnimation()->setEndPosition( endTrumpCardPos );
    //mTrumpCard->getAnimation()->setCard( card );
    mTrumpCard->getAnimation()->setNewCardText( card->getCardText() );
    
    //mTrumpCard->setVisible( true );
    scene()->addItem( mTrumpCard );
    
    mTrumpCard->getAnimation()->startAnimation();
    
    //QTimer::singleShot( mTrumpCard->getAnimation()->timeLine()->duration(), mClient, SLOT( slotProcessCommands() ) );
    mTimer->start( CARD_ANIMATION_TIME );
}

void CentralWidget::slotTrumpCardSelectableChanged( bool selectable )
{
    mTrumpCard->setSelectable( selectable );
}

void CentralWidget::slotOpponentChangeTrumpCard( int cardId, Card* newTrumpCard )
{
    kDebug();
    kDebug() << cardId << newTrumpCard->getCardText();
    
    //Disconnect from signal of animation
    mTrumpCard->disconnect();
    
    SvgCard* newSvgTrumpCard = mOpponentCards->takeCard( cardId );
    newSvgTrumpCard->getAnimation()->setEndPosition( mTrumpCard->pos() );
    newSvgTrumpCard->getAnimation()->setNewCardText( newTrumpCard->getCardText() );
    newSvgTrumpCard->getAnimation()->startAnimation();
    
    mTrumpCard->setElementId( SvgCard::backCardText() );
    mOpponentCards->slotAddNewCard( mTrumpCard );
    
    mTrumpCard = newSvgTrumpCard;
}

void CentralWidget::slotPlayerChangeTrumpCard( int playerCardId )
{
    kDebug() << playerCardId;
    
    //Disconnect from signal of animation
    mTrumpCard->disconnect();
    
    SvgCard* newSvgTrumpCard = mPlayerCards->takeCard( playerCardId );
    newSvgTrumpCard->getAnimation()->setEndPosition( mTrumpCard->pos() );
    newSvgTrumpCard->getAnimation()->startAnimation();
    
    
    mPlayerCards->slotAddNewCard( mTrumpCard );
    mTrumpCard = newSvgTrumpCard;
}

void CentralWidget::slotOpponentCardsSizeChanged()
{
    kDebug();
    
    //Set player's cards new position
    QPointF cardsGroupEndPos( ( sceneRect().width() - mOpponentCards->boundingRect().width() ) / 2,
                              mOpponentCards->y() );
    
    //mOpponentCards->getAnimation()->setEndPosition( cardsGroupEndPos );
    //mOpponentCards->getAnimation()->startAnimation();
    mOpponentCards->setPos( cardsGroupEndPos );
    
    //Set player's score table new position
    QPointF scoreTableEndPos( cardsGroupEndPos.x() + mOpponentCards->boundingRect().width() + CARDS_SCORE_TABLE_DISTANCE,
                              mOpponentScoreTable->y() );
    
    mOpponentScoreTable->getAnimation()->setEndPosition( scoreTableEndPos );
    mOpponentScoreTable->getAnimation()->startAnimation();
    
    //Set position of arrow
    mOpponentArrow->setPos( scoreTableEndPos.x(), 
                            mOpponentCards->y() + mOpponentCards->boundingRect().height() - mOpponentArrow->boundingRect().height() );
    //mOpponentArrow->setVisible( true );
    
    //Set opponent's texts
    setOpponentSchnapsenTextPos();
    setOpponentFortyTextPos();
    setOpponentTwentyTextPos();
}

void CentralWidget::slotPlayerCardsSizeChanged()
{
    kDebug();
    
    //Set player's cards new position
    QPointF cardsGroupEndPos( ( sceneRect().width() - mPlayerCards->boundingRect().width() ) / 2,
                              mPlayerCards->y() );
    
    //mPlayerCards->getAnimation()->setEndPosition( cardsGroupEndPos );
    //mPlayerCards->getAnimation()->startAnimation();
    mPlayerCards->setPos( cardsGroupEndPos );
    
    //Set player's score table new position
    QPointF scoreTableEndPos( cardsGroupEndPos.x() + mPlayerCards->boundingRect().width() + CARDS_SCORE_TABLE_DISTANCE,
                              mPlayerScoreTable->y() );
    
    mPlayerScoreTable->getAnimation()->setEndPosition( scoreTableEndPos );
    mPlayerScoreTable->getAnimation()->startAnimation();
    
    //Set forty button geometry
    mFortyButton->setGeometry( QRectF( QPointF( scoreTableEndPos.x(), scoreTableEndPos.y() - BUTTON_Y_DISTANCE - BUTTON_HEIGHT ),
                                       QSizeF( BUTTON_WIDTH, BUTTON_HEIGHT ) ) );
    //mFortyButton->setVisible( true );
    
    //Set twenty button geometry
    mTwentyButton->setGeometry( QRectF( QPointF( mFortyButton->x(), mFortyButton->y() - BUTTON_Y_DISTANCE - BUTTON_HEIGHT ),
                                       QSizeF( BUTTON_WIDTH, BUTTON_HEIGHT ) ) );
    //mTwentyButton->setVisible( true );
    
    //Set schnapsen button geometry
    mSchnapsenButton->setGeometry( QRectF( QPointF( mTwentyButton->x(), mTwentyButton->y() - BUTTON_Y_DISTANCE - BUTTON_HEIGHT ),
                                       QSizeF( BUTTON_WIDTH, BUTTON_HEIGHT ) ) );
    //mSchnapsenButton->setVisible( true );
    
    //Set position of arrow
    mPlayerArrow->setPos( mSchnapsenButton->x(), mPlayerCards->y() );
    //mPlayerArrow->setVisible( true );
}

void CentralWidget::slotCentralCardsSizeChanged()
{
    kDebug();
    
    QPointF cardsGroupEndPos( ( sceneRect().width() - mCentralCards->boundingRect().width() ) / 2,
                              mCentralCards->y() );
    
    mCentralCards->setPos( cardsGroupEndPos );
}

void CentralWidget::slotPlayerSelectedCard()
{
    if( mOpponentFortyText->isVisible() ){
        mOpponentFortyText->setVisible( false );
    }
    
    if( mOpponentTwentyText->isVisible() ){
        mOpponentTwentyText->setVisible( false );
    }
}

void CentralWidget::slotCloseButtonVisible( bool visible )
{
    mCloseButton->setVisible( visible );
}

void CentralWidget::slotSchnapsenButtonVisible( bool visible )
{
    mSchnapsenButton->setVisible( visible );
}

void CentralWidget::slotTwentyButtonVisible( bool visible )
{
    mTwentyButton->setVisible( visible );
}

void CentralWidget::slotFortyButtonVisible( bool visible )
{
    mFortyButton->setVisible( visible );
}

void CentralWidget::slotCloseDeck()
{
    mTrumpCard->setElementId( "back" );
}

void CentralWidget::slotShowPlayerArrow()
{
    mPlayerArrow->show();
}

void CentralWidget::slotHidePlayerArrow()
{
    mPlayerArrow->hide();
}

void CentralWidget::slotShowOpponentArrow()
{
    mOpponentArrow->show();
}

void CentralWidget::slotHideOpponentArrow()
{
    mOpponentArrow->hide();
}

void CentralWidget::slotPlayerGetCentralCards()
{
    kDebug();
    
    QPointF endPos( mapToScene( width(), height() ) );
    
    setRemoveCard( endPos );
}

void CentralWidget::slotOpponentGetCentralCards()
{
    kDebug();
    
    QPointF endPos( mapToScene( width(), height() ).x(),
                    mapToScene( width(), 0 ).y() - mDeck->boundingRect().height() );
    
    setRemoveCard( endPos );
}

void CentralWidget::setRemoveCard( QPointF pos )
{
    mRemoveCards = new QList< SvgCard* >;
    //Get the 2 central cards
    mRemoveCards->append( mCentralCards->takeFirstCard() );
    mRemoveCards->append( mCentralCards->takeFirstCard() );
    
    for( int i = 0; i < mRemoveCards->size(); ++i ){
        mRemoveCards->at( i )->getAnimation()->setEndPosition( pos );
        mRemoveCards->at( i )->getAnimation()->startAnimation();
    }
    
    QTimer::singleShot( CARD_ANIMATION_TIME, this, SLOT( slotRemoveCardArrived() ) );
}

void CentralWidget::slotRemoveCardArrived()
{
    /*while( !mRemoveCards->empty() ){
        SvgCard* card = mRemoveCards->takeFirst();
        
        scene()->removeItem( card );
        delete card;
    }
    
    delete mRemoveCards;
    mRemoveCards = 0;
    
    mClient->slotProcessCommands();
    */
    if( mRemoveCards ){
        while( !mRemoveCards->empty() ){
            SvgCard* card = mRemoveCards->takeFirst();
        
            scene()->removeItem( card );
            delete card;
        }
    
        delete mRemoveCards;
        mRemoveCards = 0;
    
        mClient->slotProcessCommands();
    }
}

void CentralWidget::slotOpponentSchnapsenButtonClicked()
{
    mOpponentSchnapsenText->setVisible( true );
}

void CentralWidget::slotOpponentFortyButtonClicked()
{
    mOpponentFortyText->setVisible( true );
}

void CentralWidget::slotOpponentTwentyButtonClicked()
{
    mOpponentTwentyText->setVisible( true );
}

void CentralWidget::setClient( Client* client )
{
    mClient = client;
    
    connect( mClient, SIGNAL( signalInitialize( QString, QString, Knapsen::TypeOfCards ) ), this, SLOT( slotInitialize( QString, QString, Knapsen::TypeOfCards ) ) );
}

void CentralWidget::setOpponentSchnapsenTextPos()
{
    qreal x = mOpponentCards->x() + 
              mOpponentCards->boundingRect().width() + 
              CARDS_SCORE_TABLE_DISTANCE;
    
    qreal y = mOpponentCards->y() + 
              mOpponentScoreTable->boundingRect().height() + 
              BUTTON_Y_DISTANCE;
              
    mOpponentSchnapsenText->setPos( x, y );
}

void CentralWidget::setOpponentFortyTextPos()
{
    qreal x = mOpponentCards->x() +
              mOpponentCards->boundingRect().width() + 
              CARDS_SCORE_TABLE_DISTANCE;
              
    qreal y = mOpponentCards->y() +
              mOpponentScoreTable->boundingRect().height() + 
              BUTTON_Y_DISTANCE +
              mOpponentSchnapsenText->boundingRect().height()+
              BUTTON_Y_DISTANCE;
    
    mOpponentFortyText->setPos( x, y );
}

void CentralWidget::setOpponentTwentyTextPos()
{
    qreal x = mOpponentCards->x() +
              mOpponentCards->boundingRect().width() + 
              CARDS_SCORE_TABLE_DISTANCE;
              
    qreal y = mOpponentCards->y() +
              mOpponentScoreTable->boundingRect().height() + 
              BUTTON_Y_DISTANCE +
              mOpponentSchnapsenText->boundingRect().height()+
              BUTTON_Y_DISTANCE +
              mOpponentFortyText->boundingRect().height() +
              BUTTON_Y_DISTANCE;
    
    mOpponentTwentyText->setPos( x, y );
}

void CentralWidget::clearWidget()
{
    //Remove connects from all signal
    //mClient->disconnect();
    
    scene()->clear();
    
    if( mTimer->isActive() ){
        mTimer->stop();
    }
    delete mTimer;
    
    delete mRenderer;
    //mRenderer = 0;
    
    if( mRemoveCards ){
        delete mRemoveCards;
        mRemoveCards = 0;
    }
    
    delete mOpponentCards;
    delete mPlayerCards;
    delete mCentralCards;
}
