#include <KDE/KDebug>
#include <QtCore/QRectF>
#include <QtCore/QPair>
#include <QtCore/QTimer>
//
#include <QtCore/QTimeLine>
//
#include <QtGui/QGraphicsScene>
#include "engine/card.h"
#include "table/svgcard.h"
#include "table/animation.h"
#include "table/cardsgroup.h"

const int INVALID_CARD_ID = -1;

const double HIGHLIGHT_Y_PERCENT = 20;

CardsGroup::CardsGroup() :
    mPos( 0,0 ),
    mHighlightCardId( INVALID_CARD_ID )//,
    //mSelectedCardId( INVALID_CARD_ID )
{
    mCards = new QList< SvgCard* >;
    
    mShowCards = 0;
}

CardsGroup::~CardsGroup()
{
    //delete mAnimation;
    delete mCards;
}

QRectF CardsGroup::boundingRect() const
{
    qreal width = 0;
    qreal height = 0;
    
    if( mCards->size() > 0 ){
        width = ( mCards->size() + 1 ) * ( mCards->at( 0 )->boundingRect().width() / 2 );
        height = mCards->at( 0 )->boundingRect().height();
    }
    
    return QRectF( 0, 0, width, height );
}

QPointF CardsGroup::getNewCardPos()
{
    qreal width = 0.0;
    
    if( mCards->size() > 0 ){
        width = ( (qreal)mCards->size() / 2 )  * mCards->at( 0 )->boundingRect().width();
    }
    
    return QPointF( width, 0.0 );
}

void CardsGroup::setCardsPosition()
{
    qreal halfCardWidth = 0.0;
    if( mCards->size() >= 1 ){
        halfCardWidth = mCards->first()->boundingRect().width() / 2;
    }
    kDebug() << "halfCardWidth:" << halfCardWidth;
    
    kDebug() << "mCards->size():" << mCards->size();
    for( int i = 0; i < mCards->size(); ++i ){
        QPointF distance( i * halfCardWidth, 0 );
        
        //if( mCards->at( i )->pos().x() != mPos.x() + distance.x() ){
        //kDebug() << "mPos:" << mPos << "distance:" << distance << "mPos + distance:" << mPos + distance;
        mCards->at( i )->getAnimation()->setEndPosition( mPos + distance );
        mCards->at( i )->getAnimation()->startAnimation();
        //}
    }
}

void CardsGroup::highlightCard( int id )
{
    mCards->at( id )->setPos( mCards->at( id )->pos().x(),
                             mCards->at( id )->pos().y() - mCards->at( id )->boundingRect().height() * ( HIGHLIGHT_Y_PERCENT / 100 ) );
            
    //Next cards move right
    /*for( int i = id + 1; i < mCards.size(); ++i ){
        mCards.at( i )->setPos( mCards.at( i )->pos().x() + ( mCardSize.width() ) / 2, 
                                mCards.at( i )->pos().y() );
    } */
}

void CardsGroup::removeHighlight( int id )
{
    //Remove highlightCard
    mCards->at( id )->setPos( mCards->at( id )->pos().x(),
                             mCards->at( id )->pos().y() + mCards->at(id)->boundingRect().height() * ( HIGHLIGHT_Y_PERCENT / 100 ) );
        
    //Next cards move back
    /*for( int i = id + 1; i < mCards.size(); ++i ){
        mCards.at( i )->setPos( mCards.at( i )->pos().x() - ( mCardSize.width() ) / 2, 
                                mCards.at( i )->pos().y() );
    }*/
}

void CardsGroup::slotMouseEnter( SvgCard* svgCard )
{
    int id = mCards->indexOf( svgCard );
    
    kDebug() << id;
    
    if( mHighlightCardId != INVALID_CARD_ID ){
        removeHighlight( mHighlightCardId );
        
        mHighlightCardId = INVALID_CARD_ID;
    }
    
    highlightCard( id );
    mHighlightCardId = id;
    
    //emit signalSizeChanged();
}

void CardsGroup::slotMouseLeave( SvgCard* svgCard )
{
    int id = mCards->indexOf( svgCard );
    
    kDebug() << id;
    
    if( id == mHighlightCardId ){
        removeHighlight( mHighlightCardId );
        mHighlightCardId = INVALID_CARD_ID;
        
        //emit signalSizeChanged();
    }
}

void CardsGroup::slotCardClicked( SvgCard* svgCard )
{
    int id = mCards->indexOf( svgCard );
    
    kDebug() << id;
    
    //
    mHighlightCardId = INVALID_CARD_ID;
    //
    
    emit signalSelectedCardId( id );
    
    SvgCard* svgCard1 = mCards->takeAt( id );
    
    emit signalSelectedCard( svgCard1 );
    
    //
    emit signalSizeChanged();
    //
}

void CardsGroup::setPos( qreal x, qreal y )
{
    //QGraphicsObject::setPos( x, y );
    mPos = QPointF( x, y );
    
    setCardsPosition();
}

SvgCard* CardsGroup::takeCard( int id )
{
    SvgCard* card = mCards->takeAt( id );
    
    //Remove connects from signalMouseEnter, signalMouseLeave, signalClick signals
    card->disconnect();
    
    return card;
}

void CardsGroup::clear()
{
    kDebug();
    
    while( !mCards->empty() ){
        delete mCards->takeLast();
    }
    
    emit signalSizeChanged();
}

void CardsGroup::slotAddNewCard( SvgCard* svgCard )
{
    for( int i = 0; i < mCards->size(); ++i ){
        mCards->at( i )->stackBefore( svgCard );
    }
    
    mCards->append( svgCard );
    
    //This signal will start the animation of svgCard
    emit signalSizeChanged();
    
    //When end of the animation then continue the processing commands in client
    kDebug() << "New card duration:" << svgCard->getAnimation()->timeLine()->duration();
    QTimer::singleShot( svgCard->getAnimation()->timeLine()->duration(), this, SLOT( slotCardAnimatedEnd() ) );
}

void CardsGroup::slotCardAnimatedEnd()
{
    kDebug();
    connect( mCards->last(), SIGNAL( signalMouseEnter(SvgCard*) ), this, SLOT( slotMouseEnter(SvgCard*) ) );
    connect( mCards->last(), SIGNAL( signalMouseLeave(SvgCard*) ), this, SLOT( slotMouseLeave(SvgCard*) ) );
    connect( mCards->last(), SIGNAL( signalClick( SvgCard* ) ), this, SLOT( slotCardClicked(SvgCard*) ) );
    
    emit signalCardArrived();
}

void CardsGroup::slotSelectableChanged( int id, bool enabled )
{
    kDebug() << id << enabled;
    
    mCards->at( id )->setSelectable( enabled );
}

void CardsGroup::slotSelectedCard( int id, Card* card )
{
    SvgCard* svgCard = mCards->takeAt( id );
    
    //
    //svgCard1->getAnimation()->setCard( card );
    svgCard->getAnimation()->setNewCardText( card->getCardText() );
    //
    
    emit signalSelectedCard( svgCard );
    
    //
    emit signalSizeChanged();
    //
}

void CardsGroup::slotShowCards( int card0Id, Card card0, int card1Id, Card card1 )
{
    kDebug();
    
    //
    mShowCards = new QPair< int, QPointF >[2];
    mShowCards[0].first = card0Id;
    mShowCards[0].second = mCards->at( card0Id )->pos();
    mShowCards[1].first = card1Id;
    mShowCards[1].second = mCards->at( card1Id )->pos();
    //
    
    QPointF card1EndPos( mCards->at( card0Id )->pos().x(),
                         mCards->at( card0Id )->pos().y() + mCards->at( card0Id )->boundingRect().height() * ( HIGHLIGHT_Y_PERCENT / 100 ) );
            
    QPointF card2EndPos( mCards->at( card1Id )->pos().x(),
                         mCards->at( card1Id )->pos().y() + mCards->at( card1Id )->boundingRect().height() * ( HIGHLIGHT_Y_PERCENT / 100 ) );
            
    mCards->at( card0Id )->setPos( card1EndPos );
    mCards->at( card1Id )->setPos( card2EndPos );
    
    mCards->at( card0Id )->setElementId( card0.getCardText() );
    mCards->at( card1Id )->setElementId( card1.getCardText() );
    
    QTimer::singleShot( 2000, this, SLOT( slotHideCards() ) );
}

void CardsGroup::slotHideCards()
{
    int card0Id = mShowCards[0].first;
    QPointF card0Pos = mShowCards[0].second;
    int card1Id = mShowCards[1].first;
    QPointF card1Pos = mShowCards[1].second;
    
    delete[] mShowCards;
    mShowCards = 0;
    
    mCards->at( card0Id )->setElementId( "back" );
    mCards->at( card0Id )->setPos( card0Pos );
    mCards->at( card1Id )->setElementId( "back" );
    mCards->at( card1Id )->setPos( card1Pos );
    
    emit signalHideCards();
}
