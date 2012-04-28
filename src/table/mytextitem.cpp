#include <QtGui/QPainter>
#include "mytextitem.h"

MyTextItem::MyTextItem(const QString& text, QGraphicsItem* parent): QGraphicsTextItem(text, parent)
{}

void MyTextItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget  )
{
	painter->fillRect( boundingRect(), Qt::white );
	
    QGraphicsTextItem::paint( painter, option, widget);
}
