#ifndef scoretable_h
#define scoretable_h

#include <QtGui/QGraphicsItem>

class ScoreTable : public QGraphicsItem
{

	QGraphicsTextItem* tricksItem;
	QGraphicsTextItem* scoreItem;

public:
    ScoreTable();
	
	virtual QRectF boundingRect() const;
	
	void setTricks( int );
	void setScores( int );
	
protected:
	virtual void paint( QPainter * , const QStyleOptionGraphicsItem * , QWidget * );
	
};

#endif //scoretable_h
