#ifndef centralwidget_h
#define centralwidget_h

#include <QtGui/QGraphicsView>
#include "enums.h"


class QGraphicsSvgItem;
class SvgCard;
class QSvgRenderer;
class MyTextItem;
class ScoreTable;
class QGraphicsPixmapItem;
class QTimer;
class Card;

class Hand;
class Client;

class CentralWidget : public QGraphicsView
{
    Q_OBJECT
    
    //
    Client* mClient;
    //
    
    bool mGameIsRunning;
    
    QSizeF mCardSize;
    
    //
    double mScale;
    //
    
    //
    QPair< int, int > *mShowOpponentCardsId;
    //
    
    MyTextItem* mOpponentName;
    MyTextItem* mPlayerName;
    
    QSvgRenderer* mRenderer;
    //QGraphicsSvgItem* mDeck;
    SvgCard* mDeck;
    
    //
    //int mNumberOfCardsInHand;
    //
    
    //QGraphicsSvgItem* mOpponentCards;
    //MySvgItem* mPlayerCards;
    Hand* mOpponentCards;
    Hand* mPlayerCards;
    
    //
    QTimer *mOpponentCardsShowTimer;
    //
    
    //
    QGraphicsPixmapItem* mPlayerArrow;
    QGraphicsPixmapItem* mOpponentArrow;
    //
    
    ScoreTable* mOpponentScoreTable;
    ScoreTable* mPlayerScoreTable;
    
    SvgCard* mTrumpCard;
    
    QGraphicsSvgItem* mCentralCards;
    
    QGraphicsProxyWidget* mCloseButton;
    QGraphicsProxyWidget* mTwentyButton;
    QGraphicsProxyWidget* mFortyButton;
    
    void setInGamePositions();
    
    //
    //void setOpponentScoreTablePos();
    //void setPlayerScoreTablePos();
    //
    
public:
    CentralWidget( QWidget* parent = 0 );
    ~CentralWidget();
    
    //
    //void setClient( Client* client ){ mClient = client; }
    void setClient( Client* );
    //
    
    void clearWidget();
    
protected:
    void resizeEvent( QResizeEvent* );
    
private slots:
    //
    void slotOpponentCardsSizeChanged();
    void slotPlayerCardsSizeChanged();
    
    void slotInitialize( QString, QString, Knapsen::TypeOfCards );
    
    void slotNewTrumpCard( const Card* );
    void slotHideTrumpCard();
    void slotTrumpCardSelectableChanged( bool );
    
    void slotNewCentralCard( int, const Card* );
    void slotClearCentralCards();
    
    void slotHideDeck();
    void slotCloseDeck();
    
    void slotOpponentTricksChanged( int );
    void slotOpponentScoresChanged( int );
    void slotPlayerTricksChanged( int );
    void slotPlayerScoresChanged( int );
    
    void slotShowOpponentArrow();
    void slotShowPlayerArrow();
    void slotHideOpponentArrow();
    void slotHidePlayerArrow();
    //
    //void slotTwentyButtonClicked(){ emit signalTwentyButtonClicked(); }
    //void slotFortyButtonClicked(){ emit signalFortyButtonClicked(); }
    //void slotCloseButtonClicked(){ emit signalCloseButtonClicked(); }
    //void slotSelectTrumpCardSlot(){ emit signalSelectTrumpCard(); }
    void slotCoverOpponentCards();
    
    void slotNewRound();
    
    void slotStartGame();
    
public slots:
    
    
    
    void slotTwentyButtonVisible( bool );
    void slotFortyButtonVisible( bool );
    void slotCloseButtonVisible( bool );
    
    void slotShowOpponentCards( int, Card, int, Card );
    
};


#endif //centralwidget_h
