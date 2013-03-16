#ifndef mytextitem_h
#define mytextitem_h

#include <QtGui/QGraphicsTextItem>
#include <QtCore/QPointer>

class Animation;

class MyTextItem : public QGraphicsTextItem
{
    Q_OBJECT
    
    Animation* mAnimation;
    //QPointer< Animation > mAnimation;
    
protected:
    void paint( QPainter * , const QStyleOptionGraphicsItem * , QWidget * );
    
public:
    MyTextItem( const QString& , int animationTime = 0 );
    ~MyTextItem();
    
    Animation* getAnimation(){ return mAnimation; }
    
    //Reimplemented
    //QPointF pos();
    //
    
/*private slots:
    void slotAnimationEnd();*/
    
//signals:
//    void signalMyTextItemAnimationEnd();
    
};

#endif //mytextitem_h
