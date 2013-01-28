#ifndef HAND_H
#define HAND_H

#include <QtGui/QGraphicsObject>

class QSvgRenderer;
class SvgCard;
class Card;

class Hand: public QGraphicsObject
{
    Q_OBJECT
    
    QSvgRenderer* mRenderer;
    
    double mScale;
    
    QList< SvgCard* > mCards;
    
    //
    int mHighlightCardId;
    //
    
    
    void newCard( QString );

    //void highlightCard( int );
    //void removeHighlight( int );
    
public:
    Hand( QSvgRenderer*, double );
    
    virtual QRectF boundingRect() const;
    virtual void paint( QPainter * , const QStyleOptionGraphicsItem * , QWidget * );
    
private slots:
    void slotMouseEnter( SvgCard* );
    void slotMouseLeave( SvgCard* );
    
    void slotCardClicked( SvgCard* );
    
public slots:
    void slotNewCard();
    void slotNewCard( const Card* );
    
    void slotSelectableChanged( int, bool );
    
    //
    void slotRemoveCard( int );
    //
    
signals:
    void signalSizeChanged();
    void signalSelectedCardId( int );
    
};

#endif //HAND_H
