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
class Card;

class Cards;
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
    
    MyTextItem* mOpponentName;
    MyTextItem* mPlayerName;
    
    QSvgRenderer* mRenderer;
    SvgCard* mDeck;
    
    Cards* mOpponentCards;
    Cards* mPlayerCards;

    //
    QGraphicsPixmapItem* mPlayerArrow;
    QGraphicsPixmapItem* mOpponentArrow;
    //
    
    ScoreTable* mOpponentScoreTable;
    ScoreTable* mPlayerScoreTable;
    
    SvgCard* mTrumpCard;
    
    //QGraphicsSvgItem* mCentralCards;
    Cards* mCentralCards;
    
    QGraphicsProxyWidget* mCloseButton;
    QGraphicsProxyWidget* mTwentyButton;
    QGraphicsProxyWidget* mFortyButton;
    QGraphicsProxyWidget* mSchnapsenButton;
    
    void setInGamePositions();
    
public:
    CentralWidget( QWidget* parent = 0 );
    ~CentralWidget();
    
    void setClient( Client* );
    
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
    
    void slotTwentyButtonVisible( bool );
    void slotFortyButtonVisible( bool );
    void slotSchnapsenButtonVisible( bool );
    void slotCloseButtonVisible( bool );
    
    void slotNewRound();
    
    void slotStartGame();

};


#endif //centralwidget_h
