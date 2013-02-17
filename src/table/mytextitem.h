#ifndef mytextitem_h
#define mytextitem_h

#include <QtGui/QGraphicsTextItem>

class Animation;

class MyTextItem : public QGraphicsTextItem
{
    Q_OBJECT
    
    Animation* mAnimation;
    
protected:
    void paint( QPainter * , const QStyleOptionGraphicsItem * , QWidget * );
    
public:
    MyTextItem( const QString &, QGraphicsItem * parent = 0 );
    ~MyTextItem();
    
    Animation* getAnimation(){ return mAnimation; }
    
private slots:
    void slotAnimationEnd();
    
signals:
    void signalMyTextItemAnimationEnd();
    
};

#endif //mytextitem_h
