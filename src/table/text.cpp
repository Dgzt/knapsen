#include <KDE/KDebug>
#include <QtGui/QPainter>
#include "table/animation.h"
#include "table/text.h"

Text::Text( const QString& text, int animationTime ) :
    QGraphicsTextItem( text )
{
    if( animationTime > 0 ){
        mAnimation = new Animation( this, animationTime );
    }
    
    kDebug() << font();
}

Text::~Text()
{
    if( mAnimation ){
        delete mAnimation;
    }
}

void Text::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget  )
{
    //White background
    painter->fillRect( boundingRect(), Qt::white );
    
    //Draw text
    QGraphicsTextItem::paint( painter, option, widget);
}
