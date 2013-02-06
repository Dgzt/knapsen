#ifndef CARDS_H
#define CARDS_H

#include <QtGui/QGraphicsObject>
//#include <QtCore/QObject>
//#include <QtGui/QGraphicsItemGroup>

class QSvgRenderer;
class SvgCard;
class Card;

class Cards: public QGraphicsObject
//class Cards : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
    
    //
    QSizeF mCardSize;
    //
    
    QList< SvgCard* > mCards;
    
    //
    int mHighlightCardId;
    //
    
    //
    QPair< int, int > *mShowOpponentCardsId;
    //
    
    //
    bool mCardIsComing;
    //
    

    void highlightCard( int );
    void removeHighlight( int );
    
public:
    //Cards( QSvgRenderer*, double );
    Cards( QSizeF );
    
    virtual QRectF boundingRect() const;
    virtual void paint( QPainter * , const QStyleOptionGraphicsItem * , QWidget * );
    
    
    QPointF getNewCardPos();
    
    void cardWillArrive();
    
    void addNewCard( SvgCard* );
    
private slots:
    void slotMouseEnter( SvgCard* );
    void slotMouseLeave( SvgCard* );
    
    void slotCardClicked( SvgCard* );
    
    //
    void slotHideCards();
    //
    
    //
    void slotTimeLineFinished(){ emit signalCardArriwed(); }
    //
    
public slots:
    //void slotNewCard(){ newCard( "back" ); }
    //void slotNewCard( const Card* );
    //void slotNewCard( SvgCard* );
    void slotNewCardArrived();
    
    void slotChangeCard( int, const Card* );
    
    void slotSelectableChanged( int, bool );
    
    //
    void slotRemoveCard( int );
    //
    
    void slotRemoveAllCards();
    
    //
    void slotShowCards( int, Card, int, Card );
    //
    
signals:
    void signalSizeChanged();
    void signalSelectedCardId( int );
    
    void signalHiddenShowedCard();
    
    //
    void signalCardArriwed();
    //
    
};

#endif //CARDS_H
