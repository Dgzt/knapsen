#include <QtSvg/QSvgRenderer>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QCursor>
#include <KDE/KDebug>
#include "table/animation.h"
#include "table/svgcard.h"

SvgCard::SvgCard( QSvgRenderer* renderer, int animationTime/*, QString cardText */) :
    mSelectable( false )
{
    mAnimation = new Animation( this, animationTime );
    
    setSharedRenderer( renderer );
    
    //setElementId( cardText );
    setElementId( SvgCard::EmptyCard() );
    
    setAcceptHoverEvents( true );
}

SvgCard::~SvgCard()
{
    delete mAnimation;
}

void SvgCard::mousePressEvent( QGraphicsSceneMouseEvent* )
{
    if( mSelectable ){
        emit signalClick();
        emit signalClick( this );
    }
}

void SvgCard::hoverEnterEvent( QGraphicsSceneHoverEvent* )
{
    if( mSelectable ){
        emit signalMouseEnter( this );
    }
}

void SvgCard::hoverLeaveEvent( QGraphicsSceneHoverEvent* )
{
    if( mSelectable ){
        emit signalMouseLeave( this );
    }
}

void SvgCard::setSelectable( bool selectable )
{
    mSelectable = selectable;
    
    if( mSelectable ){
        setCursor( QCursor( Qt::PointingHandCursor ) );
        
        //
        if( isUnderMouse() ){
            emit signalMouseEnter( this );
        }
        //
    }else{
        setCursor( QCursor( Qt::ArrowCursor ) );
    }
}
