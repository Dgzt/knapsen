#include <QtSvg/QSvgRenderer>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QCursor>
#include <KDE/KDebug>
#include "table/svgcard.h"

SvgCard::SvgCard( QSvgRenderer* renderer, double scale, QGraphicsItem* parentItem ) :
    QGraphicsSvgItem(parentItem),
    mScale( scale ),
    //mId( -1 ),
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
        //emit signalClick( mId );
        emit signalClick( this );
    }
}

void SvgCard::hoverEnterEvent( QGraphicsSceneHoverEvent* )
{
    if( mSelectable ){
        //emit signalMouseEnter( mId );
        emit signalMouseEnter( this );
    }
}

void SvgCard::hoverLeaveEvent( QGraphicsSceneHoverEvent* )
{
    if( mSelectable ){
        //emit signalMouseLeave( mId );
        emit signalMouseLeave( this );
    }
}

QSizeF SvgCard::getSizeF()
{
    return QSizeF( boundingRect().width()*mScale, boundingRect().height()*mScale );
}

/*void SvgCard::setId( int id )
{
    mId = id;
}*/

void SvgCard::setSelectable( bool selectable )
{
    mSelectable = selectable;
    kDebug() << /*mId <<*/ mSelectable;
    
    if( mSelectable ){
        setCursor( QCursor( Qt::PointingHandCursor ) );
    }else{
        setCursor( QCursor( Qt::ArrowCursor ) );
    }
}
