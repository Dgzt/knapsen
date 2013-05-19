#include <QSvgRenderer>
#include <QGraphicsSvgItem>
#include "svgimage.h"

SvgImage::SvgImage( QDeclarativeItem *parent ) :
    QDeclarativeItem( parent )
{
    mSvgItem = new QGraphicsSvgItem( this );
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
    
    emit elementIdChanged();
}
