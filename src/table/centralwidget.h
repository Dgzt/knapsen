#ifndef centralwidget_h
#define centralwidget_h

#include <QtGui/QGraphicsView>
#include "enums.h"

class Client;
class QSvgRenderer;
class SvgCard;
class Card;
class CardsGroup;
//
class MyTextItem;
class ScoreTable;
//

//

class CentralWidget : public QGraphicsView
{
    Q_OBJECT
    
    Client* mClient;
    
    QSvgRenderer* mRenderer;
    
    //
    MyTextItem* mOpponentName;
    
    MyTextItem* mPlayerName;
    //
    
    SvgCard* mDeck;
    
    SvgCard* mTrumpCard;
    
    CardsGroup* mOpponentCards;
    ScoreTable* mOpponentScoreTable;
    
    CardsGroup* mPlayerCards;
    ScoreTable* mPlayerScoreTable;
    
    CardsGroup* mCentralCards;
    
    //Buttons
    QGraphicsProxyWidget* mCloseButton;
    QGraphicsProxyWidget* mSchnapsenButton;
    QGraphicsProxyWidget* mTwentyButton;
    QGraphicsProxyWidget* mFortyButton;
    
    //Arrows
    QGraphicsPixmapItem* mPlayerArrow;
    QGraphicsPixmapItem* mOpponentArrow;
    
    //
    QList< SvgCard* >* mRemoveCards;
    
    void setRemoveCard( QPointF );
    //
    
    
private slots:
    void slotInitialize( QString, QString, Knapsen::TypeOfCards );
    
    void slotNewGame(); 
    void slotNewRound();
    
    void slotNewOpponentCard( bool );
    void slotNewOpponentCardTrumpCard();
    void slotNewPlayerCard( bool, Card* );
    void slotNewPlayerCardTrumpCard();
    
    void slotNewTrumpCard( Card* );
    //
    void slotTrumpCardSelectableChanged( bool );
    //
    //
    void slotOpponentChangeTrumpCard( int, Card* );
    void slotPlayerChangeTrumpCard( int );
    //
    
    void slotOpponentCardsSizeChanged();
    void slotPlayerCardsSizeChanged();
    //
    void slotCentralCardsSizeChanged();
    //
    
    //void slotPlayerSelectedCard( SvgCard* );
    void slotCloseButtonVisible( bool );
    void slotSchnapsenButtonVisible( bool );
    void slotTwentyButtonVisible( bool );
    void slotFortyButtonVisible( bool );
    
    void slotCloseDeck();
    
    //
    void slotShowPlayerArrow();
    void slotHidePlayerArrow();
    void slotShowOpponentArrow();
    void slotHideOpponentArrow();
    //
    
    //
    void slotPlayerGetCentralCards();
    void slotOpponentGetCentralCards();
    
    void slotRemoveCardArrived();
    //
    
    
protected:
    virtual void resizeEvent( QResizeEvent* );
    
public:
    CentralWidget( QWidget* parent = 0 );
    
    void setClient( Client* );
    
    void clearWidget();
    
};


#endif //centralwidget_h
