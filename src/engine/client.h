#ifndef CLIENT_H
#define CLIENT_H

#include "player.h"

class CentralCards;
class Trump;

class Client : public Player
{
    Q_OBJECT
    
    int mSizeOfDeck;
    int mSizeOfDeckNow;
    
    CentralCards *mCentralCards;
    Trump *mTrump;
    
    QList<QString> commandList;
    
    //
    QList<QString>* getValues( QString );
    //
    
    virtual void newCommand( QString );
    
private slots:
    void slotConnected();
    
protected:
    //
    int getSizeOfDeckNow(){ return mSizeOfDeckNow; }
    //
    
    CentralCards* getCentralCards(){ return mCentralCards; }
    
    Trump* getTrump(){ return mTrump; }
    
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
    
    //void signalNewOpponentCardId( int );
    void signalNewOpponentCard();
    
    //void signalNewPlayerCard( int, const Card* );
    void signalNewPlayerCard( const Card* );
    
    void signalNewTrumpCard( const Card* );
    
    void signalNewCentralCard( int, const Card* );
    
    void signalClearCentralCards();
    
    void signalOpponentSelectedCardId( int );
    
    //void signalOpponentDisconnected();
    
    void signalOpponentTricksChanged( int );
    
    void signalOpponentScoresChanged( int );
    
    void signalPlayerTricksChanged( int );
    
    void signalPlayerScoresChanged( int );
    
    void signalHideDeck();
    
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
    
    void signalStartGame();
    
    //
    void signalEndRound( QString, int );
    //
    
    //
    void signalEndGame( QString );
    //
    
};      
    
#endif //CLIENT_H
