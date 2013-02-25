#ifndef scoretable_h
#define scoretable_h

//#include <QtGui/QGraphicsItem>
#include <QtGui/QGraphicsObject>

class Animation;

//class ScoreTable : public QGraphicsItem
class ScoreTable : public QGraphicsObject
{
    Q_OBJECT
    
    Animation* mAnimation;
    
    QGraphicsTextItem* tricksItem;
    QGraphicsTextItem* scoreItem;
    
public:
    ScoreTable( int );
    ~ScoreTable();
    
    virtual QRectF boundingRect() const;
    virtual void paint( QPainter * , const QStyleOptionGraphicsItem * , QWidget * );
    
    Animation* getAnimation(){ return mAnimation; }
    
public slots:
    void slotTricksChanged( int );
    void slotScoresChanged( int );
    
};

#endif //scoretable_h
