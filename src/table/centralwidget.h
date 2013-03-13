#ifndef centralwidget_h
#define centralwidget_h

//
#include <QtCore/QPointer>
//
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
    //QPointer< QSvgRenderer > mRenderer;
    
    //
    MyTextItem* mOpponentName;
    //QPointer< MyTextItem > mOpponentName;
    
    MyTextItem* mPlayerName;
    //QPointer< MyTextItem > mPlayerName;
    //
    
    SvgCard* mDeck;
    //QPointer< SvgCard > mDeck;
    
    SvgCard* mTrumpCard;
    //QPointer< SvgCard > mTrumpCard;
    
    CardsGroup* mOpponentCards;
    //QPointer< CardsGroup > mOpponentCards;
    ScoreTable* mOpponentScoreTable;
    //QPointer< ScoreTable > mOpponentScoreTable;
    
    CardsGroup* mPlayerCards;
    //QPointer< CardsGroup > mPlayerCards;
    ScoreTable* mPlayerScoreTable;
    //QPointer< ScoreTable > mPlayerScoreTable;
    
    CardsGroup* mCentralCards;
    //QPointer< CardsGroup > mCentralCards;
    
    //Buttons
    QGraphicsProxyWidget* mCloseButton;
    //QPointer< QGraphicsProxyWidget > mCloseButton;
    QGraphicsProxyWidget* mSchnapsenButton;
    //QPointer< QGraphicsProxyWidget > mSchnapsenButton;
    QGraphicsProxyWidget* mTwentyButton;
    //QPointer< QGraphicsProxyWidget > mTwentyButton;
    QGraphicsProxyWidget* mFortyButton;
    //QPointer< QGraphicsProxyWidget > mFortyButton;
    
    //Arrows
    QGraphicsPixmapItem* mPlayerArrow;
    //QPointer< QGraphicsPixmapItem > mPlayerArrow;
    QGraphicsPixmapItem* mOpponentArrow;
    //QPointer< QGraphicsPixmapItem > mOpponentArrow;
    
    //
    QTimer* mTimer;
    //
    
    //
    MyTextItem* mOpponentSchnapsenText;
    MyTextItem* mOpponentFortyText;
    MyTextItem* mOpponentTwentyText;
    //
    
    //
    QList< SvgCard* >* mRemoveCards;
    
    void setRemoveCard( QPointF );
    //
    
    void setOpponentSchnapsenTextPos();
    void setOpponentFortyTextPos();
    void setOpponentTwentyTextPos();
    
    
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
    
    void slotPlayerSelectedCard();
    
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
    
    //
    void slotOpponentSchnapsenButtonClicked();
    void slotOpponentFortyButtonClicked();
    void slotOpponentTwentyButtonClicked();
    //
    
    
protected:
    virtual void resizeEvent( QResizeEvent* );
    
public:
    CentralWidget( QWidget* parent = 0 );
    
    void setClient( Client* );
    
    void clearWidget();
    
};


#endif //centralwidget_h
