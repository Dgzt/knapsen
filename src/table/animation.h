#ifndef ANIMATION_H
#define ANIMATION_H

#include <QtCore/QPointF>
#include <QtGui/QGraphicsItemAnimation>

class SvgCard;
class Card;
class Cards;
//class QTimeLine;

class Animation : public QGraphicsItemAnimation
{
    Q_OBJECT
    
    Card* mCard;
    
    Cards* mEndCardsGroup;
    
    QTimeLine* mTimeLine;
    
    void setupTimeLine();
    
public:
    Animation( SvgCard* , Cards* , Card* card = 0 );
    Animation( SvgCard* , QPointF , Card* card = 0 );
    
    void startAnimation();
    
private slots:
    void slotTimeLineTimeOut();
    
    
signals:
    void signalCardArrived();
    void signalCardArrived( SvgCard*, Cards* );
    
};

#endif //ANIMATION_H
