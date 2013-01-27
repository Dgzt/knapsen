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
    virtual void paint( QPainter * , const QStyleOptionGraphicsItem * , QWidget * );
    
    void setTricks( int );
    void setScores( int );
    
    
};

#endif //scoretable_h
