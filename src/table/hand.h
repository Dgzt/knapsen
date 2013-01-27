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
    
    void newCard( QString );
    
    int getCardId( SvgCard* );
    
public:
    Hand( QSvgRenderer*, double );
    
    virtual QRectF boundingRect() const;
    virtual void paint( QPainter * , const QStyleOptionGraphicsItem * , QWidget * );
    
private slots:
    //void slotMouseEnter( int );
    void slotMouseEnter( SvgCard* );
    //void slotMouseLeave( int );
    void slotMouseLeave( SvgCard* );
    
    //void slotCardClicked( int id ){ emit signalSelectedCardId( id ); }
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
