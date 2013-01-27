#include <KDE/KDebug>
#include <QtSvg/QSvgRenderer>
//
#include <QtGui/QPainter>
//
//
#include <QtCore/QSizeF>
#include <QtCore/QPointF>
//
#include "engine/card.h"
#include "table/svgcard.h"
#include "table/hand.h"

const int ENHANCEMENT_Y = 20;

Hand::Hand( QSvgRenderer* renderer, double scale ) :
    mRenderer( renderer ),
    mScale( scale )
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
    
    return QRectF( QPointF(0,0), QSizeF( width, height ) );
}

void Hand::paint( QPainter* /*painter*/, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/ )
{}

//void Hand::slotMouseEnter( int id )
void Hand::slotMouseEnter( SvgCard* svgCard )
{
    int id = getCardId( svgCard );

    //Highlight card
    mCards.at( id )->setPos( mCards.at( id )->pos().x(),
                             mCards.at( id )->pos().y()-ENHANCEMENT_Y );
    
    //Next cards move right
    for( int i = id + 1; i < mCards.size(); ++i ){
        mCards.at( i )->setPos( mCards.at( i )->pos().x() + ( mCards.at( i )->getSizeF().width() ) / 2, 
                                mCards.at( i )->pos().y() );
    }
}

//void Hand::slotMouseLeave( int id )
void Hand::slotMouseLeave( SvgCard* svgCard )
{
    int id = getCardId( svgCard );
    
    //Highlight back
    mCards.at( id )->setPos( mCards.at( id )->pos().x(),
                             mCards.at( id )->pos().y()+ENHANCEMENT_Y );
    
    //Next cards move back
    for( int i = id + 1; i < mCards.size(); ++i ){
        mCards.at( i )->setPos( mCards.at( i )->pos().x() - ( mCards.at( i )->getSizeF().width() ) / 2, 
                                mCards.at( i )->pos().y() );
    }
}

//void Hand::slotCardClicked( int id )
void Hand::slotCardClicked( SvgCard* svgCard )
{
    int id = getCardId( svgCard );
    
    emit signalSelectedCardId( id );
    
    delete mCards.takeAt( id );
    
    for( int i = id; i < mCards.size(); ++i ){
        mCards.at( i )->setPos( mCards.at( i )->x() - mCards.at( i )->getSizeF().width(), mCards.at( i )->y() );
    }
    
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
    
    //cardSvgItem->setId( mCards.size() );
    
    mCards.append( cardSvgItem );
    
    connect( cardSvgItem, SIGNAL( signalMouseEnter( SvgCard* ) ),    this, SLOT( slotMouseEnter( SvgCard* ) ) );
    connect( cardSvgItem, SIGNAL( signalMouseLeave( SvgCard* ) ),    this, SLOT( slotMouseLeave( SvgCard* ) ) );
    connect( cardSvgItem, SIGNAL( signalClick( SvgCard* ) ),         this, SLOT( slotCardClicked( SvgCard* ) ) );
    
    emit signalSizeChanged();
}

int Hand::getCardId( SvgCard* svgCard )
{
    int id = -1;
    
    for( int i = 0; i < mCards.size(); ++i ){
        if( svgCard == mCards.at( i ) ){
            id = i;
        }
    }
    
    return id;
}

void Hand::slotNewCard()
{
    newCard( "back" );
}

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
