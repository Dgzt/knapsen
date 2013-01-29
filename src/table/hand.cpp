#include <KDE/KDebug>
//
#include <QtCore/QDebug>
//
#include <QtSvg/QSvgRenderer>
//
#include <QtCore/QSizeF>
#include <QtCore/QPointF>
//
#include "engine/card.h"
#include "table/svgcard.h"
#include "table/hand.h"

const int ENHANCEMENT_Y = 20;

const int INVALID_HIGHLIGHT_CARD_ID = -1;

Hand::Hand( QSvgRenderer* renderer, double scale ) :
    mRenderer( renderer ),
    mScale( scale ),
    mHighlightCardId( INVALID_HIGHLIGHT_CARD_ID )
{
    kDebug() << "mScale:" << mScale;
}

QRectF Hand::boundingRect() const
{
    int width = 0;
    int height = 0;
    
    if( mCards.size() > 0 ){
        height = mCards.at( 0 )->getSizeF().height();
    
        //
        //width = ( mCards.size() + 2 ) * ( mCards.at( 0 )->getSizeF().width() / 2 );
    }
    
    for( int i = 0; i < mCards.size(); ++i ){
        //width += mCards.at( i )->getSizeF().width() / 2;
    
        if( i != mCards.size()-1 ){
            width += mCards.at( i )->getSizeF().width() / 2;  
        }else{
            width += mCards.at( i )->getSizeF().width();
        }
    }
    
    //
    if( mCards.size() > 0 && mHighlightCardId != INVALID_HIGHLIGHT_CARD_ID && mHighlightCardId != ( mCards.size() - 1 )  ){
        width += mCards.at( 0 )->getSizeF().width() / 2;
    }
    //
    
    return QRectF( QPointF(0,0), QSizeF( width, height ) );
}

void Hand::paint( QPainter* /*painter*/, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/ )
{}

void Hand::slotMouseEnter( SvgCard* svgCard )
{
    //int id = getCardId( svgCard );
    int id = mCards.indexOf( svgCard );
    
    kDebug() /*<< "slotMouseEnter(): "*/ << id;

    /*//Highlight card
    mCards.at( id )->setPos( mCards.at( id )->pos().x(),
                             mCards.at( id )->pos().y()-ENHANCEMENT_Y );
    
    //Next cards move right
    for( int i = id + 1; i < mCards.size(); ++i ){
        mCards.at( i )->setPos( mCards.at( i )->pos().x() + ( mCards.at( i )->getSizeF().width() ) / 2, 
                                mCards.at( i )->pos().y() );
    }*/
    
    if( id != mHighlightCardId ){
        //Highlight card
        mCards.at( id )->setPos( mCards.at( id )->pos().x(),
                                 mCards.at( id )->pos().y()-ENHANCEMENT_Y );
    
        //Next cards move right
        for( int i = id + 1; i < mCards.size(); ++i ){
            mCards.at( i )->setPos( mCards.at( i )->pos().x() + ( mCards.at( i )->getSizeF().width() ) / 2, 
                                    mCards.at( i )->pos().y() );
        }
        
        mHighlightCardId = id;
        
        //
        emit signalSizeChanged();
    }
    
}

void Hand::slotMouseLeave( SvgCard* svgCard )
{
    //int id = getCardId( svgCard );
    int id = mCards.indexOf( svgCard );
    
    kDebug() /*<< "slotMouseLeave(): "*/ << id;
    
    /*//Highlight back
    mCards.at( id )->setPos( mCards.at( id )->pos().x(),
                             mCards.at( id )->pos().y()+ENHANCEMENT_Y );
    
    //Next cards move back
    for( int i = id + 1; i < mCards.size(); ++i ){
        mCards.at( i )->setPos( mCards.at( i )->pos().x() - ( mCards.at( i )->getSizeF().width() ) / 2, 
                                mCards.at( i )->pos().y() );
    }*/
    
    if( id == mHighlightCardId ){
        //Highlight back
        mCards.at( id )->setPos( mCards.at( id )->pos().x(),
                                 mCards.at( id )->pos().y()+ENHANCEMENT_Y );
    
        //Next cards move back
        for( int i = id + 1; i < mCards.size(); ++i ){
            mCards.at( i )->setPos( mCards.at( i )->pos().x() - ( mCards.at( i )->getSizeF().width() ) / 2, 
                                    mCards.at( i )->pos().y() );
        }
    
        mHighlightCardId = INVALID_HIGHLIGHT_CARD_ID;
        
        //
        emit signalSizeChanged();
    }
    
}

//void Hand::slotCardClicked( int id )
void Hand::slotCardClicked( SvgCard* svgCard )
{
    //int id = getCardId( svgCard );
    int id = mCards.indexOf( svgCard );
    
    emit signalSelectedCardId( id );
    
    delete mCards.takeAt( id );
    
    for( int i = id; i < mCards.size(); ++i ){
        mCards.at( i )->setPos( mCards.at( i )->x() - mCards.at( i )->getSizeF().width(), mCards.at( i )->y() );
    }
    
    mHighlightCardId = INVALID_HIGHLIGHT_CARD_ID;
    
    emit signalSizeChanged();
}

void Hand::newCard( QString cardText )
{
    kDebug() << cardText;
    
    SvgCard *cardSvgItem = new SvgCard( mRenderer, mScale, this );
    cardSvgItem->setElementId( cardText );
    
    double x = 0;
    for( int i = 0; i < mCards.size(); ++i ){
        x+=mCards.at( i )->getSizeF().width()/2;
    }
    
    cardSvgItem->setPos( x, 0 );
    
    mCards.append( cardSvgItem );
    
    connect( cardSvgItem, SIGNAL( signalMouseEnter( SvgCard* ) ),    this, SLOT( slotMouseEnter( SvgCard* ) ) );
    connect( cardSvgItem, SIGNAL( signalMouseLeave( SvgCard* ) ),    this, SLOT( slotMouseLeave( SvgCard* ) ) );
    connect( cardSvgItem, SIGNAL( signalClick( SvgCard* ) ),         this, SLOT( slotCardClicked( SvgCard* ) ) );
    
    emit signalSizeChanged();
}

/*void Hand::highlightCard( int id )
{
    //Highlight card
    mCards.at( id )->setPos( mCards.at( id )->pos().x(),
                             mCards.at( id )->pos().y()-ENHANCEMENT_Y );
    
    //Next cards move right
    for( int i = id + 1; i < mCards.size(); ++i ){
        mCards.at( i )->setPos( mCards.at( i )->pos().x() + ( mCards.at( i )->getSizeF().width() ) / 2, 
                                mCards.at( i )->pos().y() );
    }
}*/

/*void Hand::removeHighlight( int id )
{
    //Remove highlight
    mCards.at( id )->setPos( mCards.at( id )->pos().x(),
                             mCards.at( id )->pos().y()+ENHANCEMENT_Y );
        
    //Next cards move back
    for( int i = id + 1; i < mCards.size(); ++i ){
        mCards.at( i )->setPos( mCards.at( i )->pos().x() - ( mCards.at( i )->getSizeF().width() ) / 2, 
                                mCards.at( i )->pos().y() );
    }
}*/

void Hand::slotNewCard( const Card* card )
{
    newCard( card->getCardText() );
}

void Hand::slotSelectableChanged( int id, bool enabled )
{
    kDebug() << id << enabled;
    
    mCards.at( id )->setSelectable( enabled );
}

void Hand::slotRemoveCard( int )
{
    delete mCards.takeLast();
    
    emit signalSizeChanged();
}
