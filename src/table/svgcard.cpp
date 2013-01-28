#include <QtSvg/QSvgRenderer>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QCursor>
#include <KDE/KDebug>
#include "table/svgcard.h"

SvgCard::SvgCard( QSvgRenderer* renderer, double scale, QGraphicsItem* parentItem ) :
    QGraphicsSvgItem(parentItem),
    mScale( scale ),
    mSelectable( false )
{
    setSharedRenderer( renderer );
    setScale( mScale );
    
    setAcceptHoverEvents( true );
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

QSizeF SvgCard::getSizeF()
{
    return QSizeF( boundingRect().width()*mScale, boundingRect().height()*mScale );
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
