#include <QSvgRenderer>
#include <QGraphicsSvgItem>
#include <QPainter>
#include <KDebug>
#include "svgimage.h"

SvgImage::SvgImage( QDeclarativeItem *parent ) :
    QDeclarativeItem( parent )
{
    mSvgItem = new QGraphicsSvgItem( this );
}

int SvgImage::width()
{
    return mSvgItem->boundingRect().width();
}

int SvgImage::height()
{
    return mSvgItem->boundingRect().height();
}

void SvgImage::setSource( QString source )
{
    mSource = source;

    mSvgItem->setSharedRenderer( new QSvgRenderer( mSource ) );
    
    emit sourceChanged();
}

void SvgImage::setElementId( QString elementId )
{
    mElementId = elementId;
    
    mSvgItem->setElementId( mElementId );
    
    QDeclarativeItem::setWidth( mSvgItem->boundingRect().width() );
    QDeclarativeItem::setHeight( mSvgItem->boundingRect().height() );
    
    emit elementIdChanged();
}
