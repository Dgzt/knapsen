#include <QtGui/QGraphicsSceneMouseEvent>
#include <KDE/KDebug>
#include "mysvgitem.h"

#include <QTimer>

MySvgItem::MySvgItem() : 
	mId( 0 ),
	mSelectable( false )
{}

void MySvgItem::setId( int id )
{
	mId = id;
}

void MySvgItem::setSelectable( bool selectable )
{
	mSelectable = selectable;
	kDebug() << mId << mSelectable;
}

void MySvgItem::mousePressEvent( QGraphicsSceneMouseEvent* gsme )
{
    Q_UNUSED( gsme );
	
	if( mSelectable ){
		//selectable = false;
		emit click( mId );
	}
}
