#include <QSvgRenderer>
#include <QGraphicsSvgItem>
#include <QPainter>
#include "svgimage.h"

SvgImage::SvgImage( QDeclarativeItem *parent ) :
    QDeclarativeItem( parent ),
    mX( 0 ),
    mY( 0 )
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

void SvgImage::setX( int x )
{
    mX = x;
    
    mSvgItem->setX( mX );
    
    emit xChanged();
}

void SvgImage::setY( int y )
{
    mY = y;
    
    mSvgItem->setY( mY );
    
    emit yChanged();
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
