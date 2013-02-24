#include <KDE/KDebug>
#include <QtCore/QRectF>
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
    
    for( int i = 0; i < mCards->size(); ++i ){
        QPointF distance( i * halfCardWidth, 0 );
        
        if( mCards->at( i )->pos().x() != /*pos().x()*/ mPos.x() + distance.x() ){
            mCards->at( i )->getAnimation()->setEndPosition( /*pos()*/ mPos + distance );
            mCards->at( i )->getAnimation()->startAnimation();
        }
    }
}

void CardsGroup::highlightCard( int id )
{
    mCards->at( id )->setPos( mCards->at( id )->pos().x(),
                             mCards->at( id )->pos().y() - /*mCardSize.height()*/ mCards->at( id )->boundingRect().height() * ( HIGHLIGHT_Y_PERCENT / 100 ) );
            
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
                             mCards->at( id )->pos().y() + /*mCardSize.height()*/ mCards->at(id)->boundingRect().height() * ( HIGHLIGHT_Y_PERCENT / 100 ) );
        
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

/*void CardsGroup::clear( QPointF pos )
{
    for( int i = 0; i < mCards.size(); ++i ){
        mCards.at( i )->getAnimation()->setEndPosition( pos );
        mCards.at( i )->getAnimation()->startAnimation();
    }
    
}*/

QList< SvgCard* >* CardsGroup::takeCards()
{
    QList< SvgCard* >* retCards = mCards;
    mCards = new QList< SvgCard* >;
    
    return retCards;
}

void CardsGroup::slotCardAnimatedEnd()
{
    kDebug();
    
    mCards->last()->getAnimation()->disconnect();
    
    connect( mCards->last(), SIGNAL( signalMouseEnter(SvgCard*) ), this, SLOT( slotMouseEnter(SvgCard*) ) );
    connect( mCards->last(), SIGNAL( signalMouseLeave(SvgCard*) ), this, SLOT( slotMouseLeave(SvgCard*) ) );
    connect( mCards->last(), SIGNAL( signalClick( SvgCard* ) ), this, SLOT( slotCardClicked(SvgCard*) ) );
    
    emit signalCardArrived();
}

void CardsGroup::slotAddNewCard( SvgCard* svgCard )
{
    //
    if( !mCards->empty() ){
        mCards->last()->stackBefore( svgCard );
    }
    //
    
    mCards->append( svgCard );
   
    connect( svgCard->getAnimation(), SIGNAL( signalAnimationEnd() ), this, SLOT( slotCardAnimatedEnd() ) );
    
    emit signalSizeChanged();
    
    svgCard->setVisible( true );
}

void CardsGroup::slotSelectableChanged( int id, bool enabled )
{
    kDebug() << id << enabled;
    
    mCards->at( id )->setSelectable( enabled );
}

void CardsGroup::slotSelectedCard( int id, Card* card )
{
    SvgCard* svgCard1 = mCards->takeAt( id );
    
    //
    svgCard1->getAnimation()->setCard( card );
    //
    
    emit signalSelectedCard( svgCard1 );
    
    //
    emit signalSizeChanged();
    //
}
