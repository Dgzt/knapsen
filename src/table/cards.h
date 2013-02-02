#ifndef CARDS_H
#define CARDS_H

#include <QtGui/QGraphicsObject>

class QSvgRenderer;
class SvgCard;
class Card;

class Cards: public QGraphicsObject
{
    Q_OBJECT
    
    QSvgRenderer* mRenderer;
    
    double mScale;
    
    QList< SvgCard* > mCards;
    
    //
    int mHighlightCardId;
    //
    
    //
    QPair< int, int > *mShowOpponentCardsId;
    //
    
    
    void newCard( QString );

    void highlightCard( int );
    void removeHighlight( int );
    
public:
    Cards( QSvgRenderer*, double );
    
    virtual QRectF boundingRect() const;
    virtual void paint( QPainter * , const QStyleOptionGraphicsItem * , QWidget * );
    
    //
    //void removeAllCards();
    //
    
private slots:
    void slotMouseEnter( SvgCard* );
    void slotMouseLeave( SvgCard* );
    
    void slotCardClicked( SvgCard* );
    
    //
    void slotHideCards();
    //
    
public slots:
    void slotNewCard(){ newCard( "back" ); }
    void slotNewCard( const Card* );
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
    
};

#endif //CARDS_H
