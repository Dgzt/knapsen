#ifndef centralwidget_h
#define centralwidget_h

#include <QtCore/QPointer>
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
    
    //SvgCard* mTrumpCard;
    QPointer< SvgCard > mTrumpCard;
    
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
    
protected:
    /*!
     * Set the start position of the deck.
     */
    void setDeckStartPos();
    
    /*!
     * Set the end position of the deck.
     */
    void setDeckEndPos();
    
    /*!
     * Set the geometry of the close button.
     */
    void setCloseButtonGeometry();
    
    /*!
     * Set the end position of the trump card.
     */
    void setTrumpCardStartPos();
    
    /*!
     * Set the end position of the trump card.
     */
    void setTrumpCardEndPos();
    
    /*!
     * Set the start position of the name.
     */
    void setOpponentNameStartPos();
    
    /*!
     * Set the end position of the name.
     */
    void setOpponentNameEndPos();
    
    /*!
     * Set the position of the opponent's cards group.
     */
    void setOpponentCardsPos();
    
    /*!
     * Set the start position of the opponent's score table.
     */
    void setOpponentScoreTableStartPos();
    
    /*!
     * Set the end position of the opponent's score table.
     */
    void setOpponentScoreTableEndPos();
    
    /*!
     * Set the position of the opponent's schnapsen text.
     */
    void setOpponentSchnapsenTextPos();
    
    /*!
     * Set the position of the opponent's forty text.
     */
    void setOpponentFortyTextPos();
    
    /*!
     * Set the position of the opponent's twenty text.
     */
    void setOpponentTwentyTextPos();
    
    /*!
     * Set the position of the opponent's arrow.
     */
    void setOpponentArrowPos();
    
    /*!
     * Set the start position of the player's name.
     */
    void setPlayerNameStartPos();
    
    /*!
     * Set the end position of the player's name.
     */
    void setPlayerNameEndPos();
    
    /*!
     * Set the position of the player's cards group.
     */
    void setPlayerCardsPos();
    
    /*!
     * Set the start position of the player's score table.
     */
    void setPlayerScoreTableStartPos();
    
    /*!
     * Set the end positiono of the player's score table.
     */
    void setPlayerScoreTableEndPos();
    
    /*!
     * Set the geometry of the player's forty button.
     */
    void setPlayerFortyButtonGeometry();
    
    /*!
     * Set the geometry of the player's twenty button.
     */
    void setPlayerTwentyButtonGeometry();
    
    /*!
     * Set the geometry of the player's schnapsen button.
     */
    void setPlayerSchnapsenButtonGeometry();
    
    /*!
     * Set the position of the player's arrow.
     */
    void setPlayerArrowPos();
    
    /*!
     * Set the position of the central cards group.
     */
    void setCentralCardsPos();
    
    /*!
     * Call this function when the windows was resized.
     */
    virtual void resizeEvent( QResizeEvent* );
    
protected slots:
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
    
public:
    CentralWidget( QWidget* parent = 0 );
    
    void setClient( Client* );
    
    void clearWidget();
    
};


#endif //centralwidget_h
