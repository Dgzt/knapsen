#include <QtSvg/QSvgRenderer>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QCursor>
#include <KDE/KDebug>
#include "table/animation.h"
#include "table/svgcard.h"

SvgCard::SvgCard( QSvgRenderer* renderer, qreal height, int animationTime ) :
    QGraphicsSvgItem(),
    mSelectable( false )
{
    mAnimation = new Animation( this, animationTime );
    
    setSharedRenderer( renderer );
    
    setElementId( SvgCard::backCardText() );
    
    setAcceptHoverEvents( true );
    
    //
    kDebug() << boundingRect();
    qreal scale = height / boundingRect().height();
    kDebug() << height << "/" << boundingRect().height()<< "=" << scale;
    
    //mSize = QSizeF( width, boundingRect().height() * scale );
    mSize = QSizeF( boundingRect().width() * scale, height );
    
    kDebug() << mSize;
    
    update();
    kDebug() << boundingRect();
}

SvgCard::~SvgCard()
{
    delete mAnimation;
}

void SvgCard::mousePressEvent( QGraphicsSceneMouseEvent* )
{
    if( mSelectable ){
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

QRectF SvgCard::boundingRect() const
{   
    if( mSize != QSizeF() ){
        return QRectF( QPointF( .0, .0 ), mSize );
    }
    
    return QGraphicsSvgItem::boundingRect();
}

void SvgCard::paint(QPainter* painter, const QStyleOptionGraphicsItem* , QWidget* )
{
    //QGraphicsSvgItem::paint(painter, option, widget);
    
    renderer()->render(painter, elementId(), boundingRect());
}
