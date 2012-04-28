#ifndef mytextitem_h
#define mytextitem_h

#include <QtGui/QGraphicsTextItem>

class MyTextItem : public QGraphicsTextItem
{
	
public:
    MyTextItem( const QString &, QGraphicsItem * parent = 0 );
	
protected:
	void paint( QPainter * , const QStyleOptionGraphicsItem * , QWidget * );
	
};

#endif //mytextitem_h