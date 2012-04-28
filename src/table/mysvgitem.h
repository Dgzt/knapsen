#ifndef mysvgitem_h
#define mysvgitem_h

#include <QtSvg/QGraphicsSvgItem>

class MySvgItem : public QGraphicsSvgItem
{
	Q_OBJECT
	
	int mId;
	
	bool mSelectable;
	
public:
    MySvgItem();
	
	void setId( int );
	void setSelectable( bool );
	
protected:
	void mousePressEvent ( QGraphicsSceneMouseEvent * );
	
signals:
	void click( int );
	
};

#endif //mysvgitem_h
