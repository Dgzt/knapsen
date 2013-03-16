#include <KDE/KDebug>
#include <QtGui/QPainter>
#include "QtCore/QTimeLine"
#include "table/animation.h"
#include "table/mytextitem.h"

#include <iostream>

MyTextItem::MyTextItem( const QString& text, int animationTime ) :
    QGraphicsTextItem( text )
{
    mAnimation = new Animation( this, animationTime );
    
    connect( mAnimation, SIGNAL( signalAnimationEnd() ), this, SLOT( slotAnimationEnd() ) );
}

MyTextItem::~MyTextItem()
{
    //if( mAnimation ){
    delete mAnimation;
    //}
}

/*QPointF MyTextItem::pos()
{
    if( mAnimation ){
        kDebug() << "Animation is running";
        return getAnimation()->getEndPosition();
    }
    
    kDebug() << "Animation isn't running.";
    return QGraphicsTextItem::pos();
}*/

void MyTextItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget  )
{
    painter->fillRect( boundingRect(), Qt::white );
    
    QGraphicsTextItem::paint( painter, option, widget);
}

/*void MyTextItem::slotAnimationEnd()
{
    kDebug();
    
    emit signalMyTextItemAnimationEnd();
    
    delete mAnimation;
    
    kDebug();
}*/
