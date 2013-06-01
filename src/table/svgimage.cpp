#include <QSvgRenderer>
#include <QGraphicsSvgItem>
#include <QPainter>
#include <KDebug>
#include "svgimage.h"

SvgImage::SvgImage( QDeclarativeItem *parent ) :
    QDeclarativeItem( parent )
{
    mSvgItem = new QGraphicsSvgItem( this );
    
    //
    //setAcceptHoverEvents( true );
}

/*void SvgImage::hoverEnterEvent( QGraphicsSceneHoverEvent* /event/ )
{
    //QGraphicsItem::hoverEnterEvent(event);
    
    if( mSvgItem->isUnderMouse() ){
        kDebug() << "Under mouse";
        emit signalMouseEntered();
    }
}*/

/*void SvgImage::hoverLeaveEvent( QGraphicsSceneHoverEvent* /event/ )
{
    //QGraphicsItem::hoverLeaveEvent(event);
    
    if( mSvgItem->isUnderMouse() ){
        kDebug() << "Not under mouse";
        emit signalMouseExited();
    }
}*/

qreal SvgImage::scale() const
{
    return mSvgItem->scale();
}

/*bool SvgImage::mouseOnEnter() const
{
    return mSvgItem->isUnderMouse();
}

bool SvgImage::mouseOnExit() const
{
    return !mSvgItem->isUnderMouse();
}*/

/*bool SvgImage::isContainsMouse() const
{
    return mSvgItem->isUnderMouse();
}*/

void SvgImage::setSource( QString source )
{
    /*mSource = source;

    mSvgItem->setSharedRenderer( new QSvgRenderer( mSource ) );
    
    emit sourceChanged();*/
    if( !source.isEmpty() ){
        mSource = source;

        mSvgItem->setSharedRenderer( new QSvgRenderer( mSource ) );
        setElementId( mElementId );
            
        emit sourceChanged();
        
        emit widthChanged();
        emit heightChanged();
    }
}

void SvgImage::setElementId( QString elementId )
{
    mElementId = elementId;
    
    mSvgItem->setElementId( mElementId );
    
    QDeclarativeItem::setWidth( mSvgItem->boundingRect().width() * mSvgItem->scale() );
    emit widthChanged();
    
    QDeclarativeItem::setHeight( mSvgItem->boundingRect().height() * mSvgItem->scale() );
    emit heightChanged();
    
    emit elementIdChanged();
}

void SvgImage::setScale( qreal scale )
{
    mSvgItem->setScale( scale );
    
    QDeclarativeItem::setWidth( mSvgItem->boundingRect().width() * mSvgItem->scale() );
    emit widthChanged();
    
    QDeclarativeItem::setHeight( mSvgItem->boundingRect().height() * mSvgItem->scale() );
    emit heightChanged();
    
    emit scaleChanged();
}
