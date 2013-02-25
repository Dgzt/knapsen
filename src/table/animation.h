#ifndef ANIMATION_H
#define ANIMATION_H

#include <QtGui/QGraphicsItemAnimation>

class MyTextItem;
class ScoreTable;
class SvgCard;
class CardsGroup;
//
class Card;
//

class Animation : public QGraphicsItemAnimation
{
    Q_OBJECT

    Card* mCard;
    
    void setupTimeLine( int );
    
private slots:
    void slotTimeLineTimeOut();
    
public:
    /*Animation( MyTextItem* );
    Animation( ScoreTable* );
    Animation( SvgCard* );*/
    Animation( QGraphicsItem*, int );
    ~Animation();
    
    void setEndPosition( QPointF );
    
    //
    void setCard( Card* );
    //
    
    void startAnimation();
    
signals:
    void signalAnimationEnd();
    
};

#endif //ANIMATION_H
