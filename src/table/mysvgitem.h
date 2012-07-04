#ifndef mysvgitem_h
#define mysvgitem_h

#include <QtSvg/QGraphicsSvgItem>

class MySvgItem : public QGraphicsSvgItem
{
	Q_OBJECT
	
	int mId;
	
	bool mSelectable;
	
protected:
	void mousePressEvent( QGraphicsSceneMouseEvent * );
	
public:
    MySvgItem();
	
	void setId( int );
	void setSelectable( bool );

signals:
	void click( int );
	void click();
	
};

#endif //mysvgitem_h
