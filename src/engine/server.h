#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork/QTcpServer>
#include "enums.h"

//class GameSequence;

class Player;
class Bot;
class Deck;
class CentralCards;
class Trump;
class Card;

class Server : public QTcpServer
{
    Q_OBJECT

    //GameSequence* mGameSequence;

    //If this is a multiplayer game, then this is an unused variable(pointer)
    Bot *mBot;
    
    //Size of deck, 20 or 24
    int mSizeOfDeck;
    
    //Number of cards in hand, 5 or 6
    int mNumberOfCardsInHand;
    
    //Type of cards, German suits or french suits
    Knapsen::TypeOfCards mTypeOfCards;

    //The player lists
    QList<Player*> mPlayerList;
    
    //
    int mCurrentPlayerId;
    int mGameStarterPlayerId;
    int mRoundStarterPlayerId;
    //
    
    //Deck
    Deck* mDeck;
    
    Trump* mTrump;

    //Central cards
    CentralCards *mCentralCards;
    
    //Live this pointer, when someone clicked to 20 or 40 button
    QPair<Player*, int> *mWaitingMarriage;

    bool mTwentyButtonClickedThisTurn;
    bool mFortyButtonClickedThisTurn;
    
    //Closing deck
    bool mClickedToCloseButtonThisTurn;
    
    /* The closer fails and before he/she closed the deck:
    *   -the opponent had got tricks, the opponent get 2 scores
    *   -the opponent hadn't got tricks, the opponent get 3 scores
    */
    bool mOpponentHaveNotTricksBeforePlayerClickedToCloseButton;
    
    Player *mPlayerWhoClickedToCloseButtonThisRound;
    
    QList< Player* > *mPlayerListWhoWantNewRound;
    QList< Player* > *mPlayerListWhoWantNewGame;
    
    Player* getCurrentPlayer(){ return mPlayerList.at( mCurrentPlayerId ); }
    void setCurrentPlayer( Player* );
    Player* getNextPlayer();
    
    bool isGameOver();
    bool isRoundOver();
    
    void newGame();
    void newRound();
    void roundOver();
    
    void nextPlayerStartRound();
    void nextPlayerStartGame();

    void addNewCard( Player*, Card* );
    void addTricks( Player*, int );
    void addScores( Player*, int );

private slots:
    void slotNewPlayer( Player* );
    void slotPlayerDisconnected( Player* );
    
    void slotPlayerSelectedCard( Card*, int );
    
    void slotPlayerTwentyButtonClicked();
    void slotPlayerFortyButtonClicked();
    void slotPlayerClickedToCloseButton();
    
    void slotPlayerChangeTrumpCard( Player* );
    
    void slotCheckCentralCards();
    
    void slotPlayerWantStartNextRound( Player* );
    void slotPlayerWantStartNextGame( Player* );

protected:
    void incomingConnection(int);

public:
    Server(QObject* parent = 0);
    ~Server();

    //Set who start the game, the first round
    void setWhoStartGame( Knapsen::WhoStartGame whoStartGame );
    
    //Set the cards in hand, 5 or 6
    void setNumberOfCardsInHand( int numberOfCardsInHand ){ mNumberOfCardsInHand = numberOfCardsInHand; }
    
    //Set the size of deck, 20 or 24
    void setSizeOfDeck( int sizeOfDeck ){ mSizeOfDeck = sizeOfDeck; }
    
    //Set type of cards, german suits or french suits
    void setTypeOfCards( Knapsen::TypeOfCards typeOfCards ){ mTypeOfCards = typeOfCards; }
    
    void addBot( QString, Knapsen::GameDifficulty );
    
    void startGame();
    
signals:
    void signalPlayerConnected( QString );
    void signalPlayerDisconnected( QString );
    void signalServerFull();
    void signalServerEmpty();
};

#endif //SERVER_H
