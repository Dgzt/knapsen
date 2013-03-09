#ifndef CLIENT_H
#define CLIENT_H

#include "player.h"

//class CentralCards;
class Trump;

class Client : public Player
{
    Q_OBJECT
    
    int mSizeOfDeck;
    int mSizeOfDeckNow;
    
    //CentralCards *mCentralCards;
    QList< Card* > mCentralCards;
    Trump *mTrump;
    
    QList<QString> mCommandList;
    //
    bool mCommandsUnderProcessing;
    //
    
    //
    QList<QString>* getValues( QString );
    //
    
    /*!
     * Clear the central cards. Delete all cards.
     */
    void clearCentralCards(){ while( !mCentralCards.empty() ) delete mCentralCards.takeFirst(); }
    
    virtual void newCommand( QString );
    
private slots:
    void slotConnected();
    
protected:
    //
    int getSizeOfDeckNow(){ return mSizeOfDeckNow; }
    //
    
    //CentralCards* getCentralCards(){ return mCentralCards; }
    const QList< Card* >& getCentralCards(){ return mCentralCards; }
    
    Trump* getTrump(){ return mTrump; }
    
    void sendChangeTrumpCard(){ sendCommand( CHANGE_TRUMP_CARD_COMMAND ); }
    
public:
    enum GameErrorType{ NameIsBusy, ServerIsFull, OpponentDisconnected };
    
    //Client( QObject* parent = 0 );
    Client( QString );
    /*virtual */~Client();
    
    void startNextRound(){ sendCommand( START_NEXT_ROUND_COMMAND ); }
    void startNextGame(){ sendCommand( START_NEXT_GAME_COMMAND ); }
    
public slots:
    void slotSelectCardId( int );
    void slotSelectTrumpCard();
    void slotTwentyButtonClicked();
    void slotFortyButtonClicked();
    void slotSchnapsenButtonClicked();
    void slotCloseButtonClicked();
    //
    void slotProcessCommands();
    //
    
signals:
    //Error signals
    //void signalClientError();
    void signalGameError( Client::GameErrorType );
    //
    
    //void signalInitialize( QString, QString, Knapsen::TypeOfCards, int );
    void signalInitialize( QString, QString, Knapsen::TypeOfCards );
    
    //void signalNewOpponentCard();
    void signalNewOpponentCard( bool );
    void signalNewOpponentCardTrumpCard();
    
    //void signalNewPlayerCard( /*const*/ Card* );
    void signalNewPlayerCard( bool, Card* );
    void signalNewPlayerCardTrumpCard();
    
    void signalChangePlayerCard( int, const Card* );
    
    void signalNewTrumpCard( /*const*/ Card* );
    void signalOpponentChangeTrumpCard( int, Card* );
    void signalPlayerChangeTrumpCard( int );
    
    //void signalNewCentralCard( int, const Card* );
    //void signalNewCentralCard( const Card* );
    
    //void signalClearCentralCards();
    void signalOpponentGetCentralCards();
    void signalPlayerGetCentralCards();
    
    //void signalOpponentSelectedCardId( int );
    void signalOpponentSelectedCard( int, Card* );
    
    //void signalOpponentDisconnected();
    
    void signalOpponentTricksChanged( int );
    
    void signalOpponentScoresChanged( int );
    
    //void signalHideDeck();
    
    void signalCloseDeck();
    
    void signalHideTrumpCard();
    //
    void signalTrumpCardSelectableChanged( bool );
    //
    
    void signalShowOpponentCards( int, Card, int, Card );
    
    //
    void signalNewRound();
    //
    
    void signalPlayerInAction();
    //
    void signalOpponentInAction();
    //
    
    //void signalStartGame();
    void signalNewGame();
    
    //
    void signalEndRound( QString, int );
    //
    
    //
    void signalEndGame( QString );
    //
    
};      
    
#endif //CLIENT_H
