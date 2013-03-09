/*!
 * @file
 * @author  Zsuro Tibor <zsurotibor@gmail.com>
 * 
 * @section LICENSE
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 * The Server class controll the game. Add cards to player, set tricks, set scores, etc.
 */

#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork/QTcpServer>
#include "enums.h"

//class GameSequence;

class Player;
class Bot;
//class Deck;
//class CentralCards;
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
    
    //
    QList< Card* > mCardDeck;
    //
    
    //Number of cards in hand, 5 or 6
    int mNumberOfCardsInHand;
    
    //Type of cards, German suits or french suits
    Knapsen::TypeOfCards mTypeOfCards;
    
    //Enable Schnapsen button in the game
    bool mSchnapsenButton;

    //The player lists
    QList<Player*> mPlayerList;
    
    //
    int mCurrentPlayerId;
    int mGameStarterPlayerId;
    int mRoundStarterPlayerId;
    //
    
    //Deck
    //Deck* mDeck;
    
    Trump* mTrump;

    //Central cards
    //CentralCards *mCentralCards;
    QList< Card* > mCentralCards;
    
    //Live this pointer, when someone clicked to 20 or 40 button
    QPair<Player*, int> *mWaitingMarriage;

    bool mTwentyButtonClickedThisTurn;
    bool mFortyButtonClickedThisTurn;
    
    bool mSchnapsenButtonClickedThisRound;
    
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
    
    /*!
     * Build the deck of cards. Random generate cards.
     */
    void buildDeck();
    
    /*!
     * Return with the current player.
     * 
     * @return The current player.
     */
    Player* getCurrentPlayer(){ return mPlayerList.at( mCurrentPlayerId ); }
    
    /*!
     * Set the current player. 
     * 
     * @param player This player will the current player.
     */
    void setCurrentPlayer( Player* player );
    
    /*!
     * Get the next player.
     * 
     * @return The next player.
     */
    Player* getNextPlayer();
    
    /*!
     * Clear the central cards. Delete all cards.
     */
    void clearCentralCards(){ while( !mCentralCards.empty() ) delete mCentralCards.takeFirst(); }
    
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
    //
    void addTrumpCardToPlayer( Player* );
    //

private slots:
    void slotNewPlayer( Player* );
    void slotPlayerDisconnected( Player* );
    
    //void slotPlayerSelectedCard( Card*, int );
    void slotPlayerSelectedCardId( int );
    
    void slotPlayerTwentyButtonClicked();
    void slotPlayerFortyButtonClicked();
    void slotPlayerSchnapsenButtonClicked();
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
    
    //Set schnapsen button
    void setEnableSchnapsenButton( bool enable ){ mSchnapsenButton = enable; }
    
    void addBot( QString, Knapsen::GameDifficulty );
    
    void startGame();
    
signals:
    void signalPlayerConnected( QString );
    void signalPlayerDisconnected( QString );
    void signalServerFull();
    void signalServerEmpty();
};

#endif //SERVER_H
