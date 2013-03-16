#ifndef ANIMATION_H
#define ANIMATION_H

#include <QtGui/QGraphicsItemAnimation>

class MyTextItem;
class ScoreTable;
class SvgCard;
class CardsGroup;

class Animation : public QGraphicsItemAnimation
{
    Q_OBJECT

    //Card* mCard;
    QString mNewCardText;
    
    void setupTimeLine( int );
    
private slots:
    void slotTimeLineTimeOut();
    
public:
    Animation( QGraphicsItem* graphicsItem, int animationTime );
    ~Animation();
    
    //
    void setAnimationTime( int animationTime );
    //
    
    void setEndPosition( QPointF );
    void setEndPosition( qreal x, qreal y ){ setEndPosition( QPointF( x, y ) ); }
    
    QPointF getEndPosition(){ return posAt( 1.0 ); }
    
    //
    //void setCard( Card* );
    void setNewCardText( QString cardText ){ mNewCardText = cardText; }
    //
    
    void startAnimation();
    
signals:
    void signalAnimationEnd();
    
};

#endif //ANIMATION_H
