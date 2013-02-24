#include <KDE/KGlobal>
#include <KDE/KStandardDirs>
#include <KDE/KDebug>
#include <KDE/KLocalizedString>
#include <QtSvg/QSvgRenderer>
#include <QtCore/QPointF>
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

//const int BUTTON_X_DISTANCE = 10;
const int BUTTON_Y_DISTANCE = 2;
const int BUTTON_WIDTH = 100;
const int BUTTON_HEIGHT = 30;

CentralWidget::CentralWidget( QWidget* parent ): 
    QGraphicsView( parent )
{
    mRenderer = 0;
    
    mOpponentName = 0;
    mPlayerName = 0;
    
    mDeck = 0;
    mTrumpCard = 0;
    mOpponentCards = 0;
    mOpponentScoreTable = 0;
    mPlayerCards = 0;
    mPlayerScoreTable = 0;
    mCentralCards = 0;
    //
    mCloseButton = 0;
    mSchnapsenButton = 0;
    mTwentyButton = 0;
    mFortyButton = 0;
    //
    
    //
    mPlayerArrow = 0;
    mOpponentArrow = 0;
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
    mOpponentName = new MyTextItem( opponentNameStr );
    mOpponentName->setVisible( false );
    
    scene()->addItem( mOpponentName );
    
    //Setup opponent's score table
    mOpponentScoreTable = new ScoreTable;
    mOpponentScoreTable->setVisible( false );
    
    scene()->addItem( mOpponentScoreTable );
    
    //Setup player's name
    mPlayerName = new MyTextItem( playerNameStr );
    mPlayerName->setVisible( false );
    
    scene()->addItem( mPlayerName );
    
    //Setup player's score table
    mPlayerScoreTable = new ScoreTable;
    mPlayerScoreTable->setVisible( false );
    
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
    mOpponentCards = new CardsGroup;
    //scene()->addItem( mOpponentCards );
    
    connect( mClient, SIGNAL( signalNewOpponentCard() ), this, SLOT( slotNewOpponentCard() ) );
    //
    connect( mClient, SIGNAL( signalOpponentSelectedCard( int, Card* ) ), mOpponentCards, SLOT( slotSelectedCard( int, Card* ) ) );
    //
    connect( mOpponentCards, SIGNAL( signalSizeChanged() ), this, SLOT( slotOpponentCardsSizeChanged() ) );
    connect( mOpponentCards, SIGNAL( signalCardArrived() ), mClient, SLOT( slotProcessCommands() ) );
    
    //Setup Player's cards
    mPlayerCards = new CardsGroup;
    //scene()->addItem( mPlayerCards );
    
    connect( mClient, SIGNAL( signalNewPlayerCard( Card* ) ), this, SLOT( slotNewPlayerCard( Card* ) ) );
    connect( mClient, SIGNAL( signalPlayerCardSelectableChanged( int , bool ) ), mPlayerCards, SLOT( slotSelectableChanged( int , bool ) ) );
    connect( mPlayerCards, SIGNAL( signalSizeChanged() ), this, SLOT( slotPlayerCardsSizeChanged() ) );
    connect( mPlayerCards, SIGNAL( signalCardArrived() ), mClient, SLOT( slotProcessCommands() ) );
    connect( mPlayerCards, SIGNAL( signalSelectedCardId( int ) ), mClient, SLOT( slotSelectCardId( int ) ) );
    
    //Setup central cards
    mCentralCards = new CardsGroup;
    
    /*mCentralCards->setPos( ( sceneRect().width() - ( mDeck->boundingRect().size().width() * 1.5 ) ) / 2, 
                           ( sceneRect().height() - mDeck->boundingRect().height() ) / 2 );*/
    connect( mCentralCards, SIGNAL( signalSizeChanged() ), this, SLOT( slotCentralCardsSizeChanged() ) ); 
    connect( mPlayerCards, SIGNAL( signalSelectedCard( SvgCard* ) ), mCentralCards, SLOT( slotAddNewCard( SvgCard* ) ) );
    //
    connect( mOpponentCards, SIGNAL( signalSelectedCard( SvgCard* ) ), mCentralCards, SLOT( slotAddNewCard( SvgCard* ) ) );
    //
    //
    connect( mClient, SIGNAL( signalPlayerGetCentralCards() ), this, SLOT( slotPlayerGetCentralCards() ) );
    connect( mClient, SIGNAL( signalOpponentGetCentralCards() ), this, SLOT( slotOpponentGetCentralCards() ) );
    //
    
    //scene()->addItem( mCentralCards );
    
    connect( mClient, SIGNAL( signalNewGame() ), this, SLOT( slotNewGame() ) );
    
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
    
    connect( mPlayerName, SIGNAL( signalMyTextItemAnimationEnd() ), this, SLOT( slotNewRound() ) );
    
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
    
    //Set close button position
    mCloseButton->setGeometry( QRectF( QPointF( endDeckPos.x(), endDeckPos.y() + mDeck->boundingRect().height() + BUTTON_Y_DISTANCE ), 
                                       QSizeF( mDeck->boundingRect().width(), BUTTON_HEIGHT ) ) );
}

void CentralWidget::slotNewOpponentCard()
{
    kDebug();
    
    //SvgCard* svgCard = new SvgCard( mRenderer );
    //mOpponentCards->addNewCard( svgCard, mDeck );
    
    SvgCard* svgCard = new SvgCard( mRenderer );
    //
    svgCard->setPos( mDeck->pos() );
    svgCard->setVisible( false );
    scene()->addItem( svgCard );
    //
    
    mOpponentCards->slotAddNewCard( svgCard );
}

void CentralWidget::slotNewPlayerCard( Card* card )
{
    kDebug();
    
    SvgCard* svgCard = new SvgCard( mRenderer );
    svgCard->getAnimation()->setCard( card );
    //
    svgCard->setPos( mDeck->pos() );
    svgCard->setVisible( false );
    scene()->addItem( svgCard );
    //
    
    //mPlayerCards->addNewCard( svgCard, mDeck );
    mPlayerCards->slotAddNewCard( svgCard );
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
    mRemoveCards = mCentralCards->takeCards();
    
    for( int i = 0; i < mRemoveCards->size(); ++i ){
        mRemoveCards->at( i )->getAnimation()->setEndPosition( pos );
        connect( mRemoveCards->at( i )->getAnimation(), SIGNAL( signalAnimationEnd() ), this, SLOT( slotRemoveCardArrived() ) );
        mRemoveCards->at( i )->getAnimation()->startAnimation();
    }
}

void CentralWidget::slotRemoveCardArrived()
{
    mRemoveCards->first()->disconnect();
    delete mRemoveCards->takeFirst();
    
    if( mRemoveCards->empty() ){
        delete mRemoveCards;
        mRemoveCards = 0;
        
        mClient->slotProcessCommands();
    }
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
    
    if( mRemoveCards ){
        delete mRemoveCards;
        mRemoveCards = 0;
    }
}
