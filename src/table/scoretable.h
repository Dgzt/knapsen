#ifndef scoretable_h
#define scoretable_h

//#include <QtGui/QGraphicsItem>
#include <QtGui/QGraphicsObject>

//class ScoreTable : public QGraphicsItem
class ScoreTable : public QGraphicsObject
{
    Q_OBJECT
    
    QGraphicsTextItem* tricksItem;
    QGraphicsTextItem* scoreItem;
    
public:
    ScoreTable();
    
    virtual QRectF boundingRect() const;
    virtual void paint( QPainter * , const QStyleOptionGraphicsItem * , QWidget * );
    
public slots:
    void slotTricksChanged( int );
    void slotScoresChanged( int );
    
};

#endif //scoretable_h
