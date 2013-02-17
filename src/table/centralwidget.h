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
//
//
//class Animation;
//

class CentralWidget : public QGraphicsView
{
    Q_OBJECT
    
    Client* mClient;
    
    QSvgRenderer* mRenderer;
    
    //
    MyTextItem* mOpponentName;
    //Animation* mOpponentNameAnimation;
    
    MyTextItem* mPlayerName;
    //Animation* mPlayerNameAnimation;
    //
    
    SvgCard* mDeck;
    
    SvgCard* mTrumpCard;
    
    CardsGroup* mOpponentCards;
    CardsGroup* mPlayerCards;
    
    CardsGroup* mCentralCards;
    
private slots:
    void slotInitialize( QString, QString, Knapsen::TypeOfCards );
    
    void slotNewGame(); 
    void slotNewRound();
    
    void slotNewOpponentCard();
    void slotNewPlayerCard( Card* );
    
    void slotNewTrumpCard( Card* );
    
    //void slotOpponentCardsSizeChanged();
    //void slotPlayerCardsSizeChanged();
    
    void slotPlayerSelectedCard( int, SvgCard* );
    
protected:
    virtual void resizeEvent( QResizeEvent* );
    
public:
    CentralWidget( QWidget* parent = 0 );
    
    void setClient( Client* );
    
    void clearWidget();
    
};


#endif //centralwidget_h
