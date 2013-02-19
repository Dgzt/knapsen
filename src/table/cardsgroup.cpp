#include <KDE/KDebug>
#include <QtCore/QRectF>
#include <QtGui/QGraphicsScene>
#include "engine/card.h"
#include "table/svgcard.h"
#include "table/animation.h"
#include "table/cardsgroup.h"

const int INVALID_HIGHLIGHT_CARD_ID = -1;

const double HIGHLIGHT_Y_PERCENT = 20;

CardsGroup::CardsGroup(/* QSizeF cardSize */) : 
    //mCardSize( cardSize ),
    //mCardArrive( false ),
    mHighlightCardId( INVALID_HIGHLIGHT_CARD_ID )
{
    mAnimation = new Animation( this );
}

CardsGroup::~CardsGroup()
{
    delete mAnimation;
}

QRectF CardsGroup::boundingRect() const
{
    qreal width = 0.0;
    
    if( mCards.size() > 0 ){
        width = ( mCards.size() + 1 ) * ( mCards.at( 0 )->boundingRect().width() / 2 );
    }
    
    return QRectF( 0, 0, width, 0 );
}

void CardsGroup::paint( QPainter* , const QStyleOptionGraphicsItem* , QWidget* )
{}

/*QPointF CardsGroup::getNewCardPos()
{
    int width = 0.0;
    
    if( mCards.size() != 0 ){
        width = mCards.size() * ( mCardSize.width() / 2 );
    }
    
    return QPointF( width, 0.0 );
}*/

/*void CardsGroup::newCardArrive()
{
    mCardArrive = true;
    
    emit signalSizeChanged();
}*/

QPointF CardsGroup::getNewCardPos()
{
    qreal width = 0.0;
    
    if( mCards.size() > 0 ){
        width = ( (qreal)mCards.size() / 2 )  * mCards.at( 0 )->boundingRect().width();
    }
    
    return QPointF( width, 0.0 );
}

void CardsGroup::highlightCard( int id )
{
    mCards.at( id )->setPos( mCards.at( id )->pos().x(),
                             mCards.at( id )->pos().y() - /*mCardSize.height()*/ mCards.at(id)->boundingRect().height() * ( HIGHLIGHT_Y_PERCENT / 100 ) );
            
    //Next cards move right
    /*for( int i = id + 1; i < mCards.size(); ++i ){
        mCards.at( i )->setPos( mCards.at( i )->pos().x() + ( mCardSize.width() ) / 2, 
                                mCards.at( i )->pos().y() );
    } */
}

void CardsGroup::removeHighlight( int id )
{
    //Remove highlightCard
    mCards.at( id )->setPos( mCards.at( id )->pos().x(),
                             mCards.at( id )->pos().y() + /*mCardSize.height()*/ mCards.at(id)->boundingRect().height() * ( HIGHLIGHT_Y_PERCENT / 100 ) );
        
    //Next cards move back
    /*for( int i = id + 1; i < mCards.size(); ++i ){
        mCards.at( i )->setPos( mCards.at( i )->pos().x() - ( mCardSize.width() ) / 2, 
                                mCards.at( i )->pos().y() );
    }*/
}

void CardsGroup::slotMouseEnter( SvgCard* svgCard )
{
    int id = mCards.indexOf( svgCard );
    
    kDebug() << id;
    
    if( mHighlightCardId != INVALID_HIGHLIGHT_CARD_ID ){
        removeHighlight( mHighlightCardId );
        
        mHighlightCardId = INVALID_HIGHLIGHT_CARD_ID;
    }
    
    highlightCard( id );
    mHighlightCardId = id;
    
    //emit signalSizeChanged();
}

void CardsGroup::slotMouseLeave( SvgCard* svgCard )
{
    int id = mCards.indexOf( svgCard );
    
    kDebug() << id;
    
    if( id == mHighlightCardId ){
        removeHighlight( mHighlightCardId );
        mHighlightCardId = INVALID_HIGHLIGHT_CARD_ID;
        
        //emit signalSizeChanged();
    }
}

void CardsGroup::slotCardClicked( SvgCard* svgCard )
{
    int id = mCards.indexOf( svgCard );
    
    emit signalSelectedCard( id, svgCard );
    
    //mHighlightCardId = INVALID_HIGHLIGHT_CARD_ID;
    
    //emit signalSizeChanged();
}

void CardsGroup::addNewCard( SvgCard* svgCard, QGraphicsItem* fromItem )
{
    kDebug();
    
    //Add the card to scene
    svgCard->setParentItem( this );
    svgCard->setVisible( false );
    //scene()->addItem( svgCard );
    
    QPointF endPosition = getNewCardPos();
    
    mCards.append( svgCard );
    
    emit signalSizeChanged();
    
    //Move the card to the new position
    svgCard->setPos( mapFromItem( fromItem, QPointF( 0, 0 ) ) );
    
    svgCard->getAnimation()->setEndPosition( endPosition );
    
    svgCard->setVisible( true );
    
    connect( svgCard->getAnimation(), SIGNAL( signalAnimationEnd() ), this, SLOT( slotCardAnimatedEnd() ) );
    
    svgCard->getAnimation()->startAnimation();
}

void CardsGroup::slotCardAnimatedEnd()
{
    kDebug();
    
    mCards.last()->getAnimation()->disconnect();
    
    connect( mCards.last(), SIGNAL( signalMouseEnter(SvgCard*) ), this, SLOT( slotMouseEnter(SvgCard*) ) );
    connect( mCards.last(), SIGNAL( signalMouseLeave(SvgCard*) ), this, SLOT( slotMouseLeave(SvgCard*) ) );
    
    emit signalCardArrived();
}

void CardsGroup::slotSelectableChanged( int id, bool enabled )
{
    kDebug() << id << enabled;
    
    mCards.at( id )->setSelectable( enabled );
}
