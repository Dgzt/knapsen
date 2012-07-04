#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QCursor>
#include <KDE/KDebug>
#include "mysvgitem.h"

MySvgItem::MySvgItem() : 
	mId( 0 ),
	mSelectable( false )
{	
	//setAcceptHoverEvents( true );
}

void MySvgItem::mousePressEvent( QGraphicsSceneMouseEvent* )
{
    //Q_UNUSED( gsme );
	
	if( mSelectable ){
		//selectable = false;
		
		emit click();
		emit click( mId );
	}
}

void MySvgItem::setId( int id )
{
	mId = id;
}

void MySvgItem::setSelectable( bool selectable )
{
	mSelectable = selectable;
	kDebug() << mId << mSelectable;
	
	if( mSelectable ){
		setCursor( QCursor( Qt::PointingHandCursor ) );
	}else{
		setCursor( QCursor( Qt::ArrowCursor ) );
	}
}
