#ifndef CARDS_GROUP_H
#define CARDS_GROUP_H

//#include <QtGui/QGraphicsObject>
#include <QtCore/QObject>
#include <QtCore/QPointF>
#include <QtCore/QPair>

class SvgCard;
class Animation;
class Card;

//class CardsGroup : public QGraphicsObject
class CardsGroup : public QObject
{
    Q_OBJECT
    
    QPointF mPos;
    
    int mHighlightCardId;
    
    QList< SvgCard* >* mCards;
    
    //
    QPair< int, QPointF >* mShowCards;
    //
    
    ///
    QPointF getNewCardPos();
    //
    
    //
    void setCardsPosition();
    //
    
    void highlightCard( int );
    void removeHighlight( int );
    
public:
    CardsGroup();
    ~CardsGroup();
    
    QRectF boundingRect() const;
    
    //
    qreal x(){ return mPos.x(); }
    qreal y(){ return mPos.y(); }
    
    void setPos( qreal, qreal );
    void setPos( QPointF point ){ setPos( point.x(), point.y() ); }
    
    QList< SvgCard* >* takeCards();
    
    SvgCard* takeCard( int );
    
private slots:
    void slotCardAnimatedEnd();
    //void slotCardAnimatedEnd(){ emit signalCardArrived(); }
    
    void slotMouseEnter( SvgCard* );
    void slotMouseLeave( SvgCard* );
    
    void slotCardClicked( SvgCard* );
    
    //
    void slotHideCards();
    //
    
public slots:
    //
    void slotAddNewCard( SvgCard* );
    //
    void slotSelectableChanged( int, bool );
    
    //
    void slotSelectedCard( int, Card* );
    //
    
    //
    void slotShowCards( int, Card, int, Card );
    //
    
    
signals:
    void signalSizeChanged();
    
    void signalCardArrived();
    
    //
    void signalSelectedCard( SvgCard* );
    //
    void signalSelectedCardId( int );
    
    //
    void signalHideCards();
    //
    
};

#endif //CARDS_GROUP_H
