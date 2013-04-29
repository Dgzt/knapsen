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
class Trump;
class Card;

class Server : public QTcpServer
{
    Q_OBJECT

    //GameSequence* mGameSequence;

    //If this is a multiplayer game, then this is an unused variable(pointer)
    Bot *mBot;
    
    //Type of cards, German suits or french suits
    Knapsen::TypeOfCards mTypeOfCards;
    
    //Size of deck, 20 or 24
    int mSizeOfDeck;
    
    //Number of cards in hand, 5 or 6
    int mNumberOfCardsInHand;
    
    //
    QList< Card* > mCardDeck;
    //
    
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
    
    
private slots:
    /*!
     * A new player connected to the server.
     * 
     * @param player The new player.
     */
    void slotNewPlayer( Player* player );
    
    /*!
     * The player disconnected from the server.
     * 
     * @param player The player.
     */
    void slotPlayerDisconnected( Player* player );
    
    /*!
     * The current player send the selected card id.
     * 
     * @param selectedCardId The selected card id.
     */
    void slotPlayerSelectedCardId( int selectedCardId );
    
    /*!
     * The current player clicked to the twenty button.
     */
    void slotPlayerTwentyButtonClicked();
    
    /*!
     * The current player clicked to the forty button.
     */
    void slotPlayerFortyButtonClicked();
    
    /*!
     * The current player clicked to the schnapsen button.
     */
    void slotPlayerSchnapsenButtonClicked();
    
    /*!
     * The current player clicked to the close button.
     */
    void slotPlayerClickedToCloseButton();
    
    /*!
     * The player changed the trump card with their card.
     * 
     * @param player The player.
     */
    void slotPlayerChangeTrumpCard( Player* player );
    
    /*!
     * If two cards is in the central cards then check it who get the points, 
     * who start next turn, etc.
     */
    void slotCheckCentralCards();
    
    /*!
     * The player want start next round.
     * 
     * @param player The player.
     */
    void slotPlayerWantStartNextRound( Player* player );
    
    /*!
     * The player want start next game.
     * 
     * @param player The player.
     */
    void slotPlayerWantStartNextGame( Player* player );

protected:
    /*!
     * A socket connected to server. The socket then connected
     * finally to the server when send the player's name.
     * 
     * @param socketDescriptor The socket descriptor.
     */
    void incomingConnection( int socketDescriptor );
    
    /*!
     * Set who start the game. Local player or opponent player or random.
     * 
     * @param whoStartGame Who start the game.
     */
    void setWhoStartGame( Knapsen::WhoStartGame whoStartGame );
    
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
    
    /*!
     * Return with true when a player have equal or more than 7 scores else false.
     * 
     * @return True/False.
     */
    bool isGameOver();
    
    /*!
     * Reutrn with true when a player have equal or more than 66 tricks else false.
     * 
     * @return True/False.
     */
    bool isRoundOver();
    
    /*!
     * Clear scores at all players and start new game.
     */
    void newGame();
    
    /*!
     * Clear tricks at all players and start new round.
     */
    void newRound();
    
    /*!
     * Check who win the round and give the tricks to winner player.
     */
    void roundOver();
    
    /*!
     * The next player will start the next round.
     */
    void nextPlayerStartRound();
    
    /*!
     * The next player will start the next game.
     */
    void nextPlayerStartGame();

    /*!
     * Add the card to the player and send to the others the info.
     */
    void addNewCard( Player* player, Card* card );
    
    /*!
     * Add the tricks to the player and send to the others the info.
     */
    void addTricks( Player* player, int tricks );
    
    /*!
     * Add the scores to the player and send to the others the info.
     */
    void addScores( Player* player, int scores );
    
    /*!
     * Add the trump card to the player and send to the other the info.
     */
    void addTrumpCardToPlayer( Player* player );
    

public:
    /*!
     * The constructor set the values to the variables. 
     * 
     * @param typeOfCards The type of cards. German suit or French suit.
     * @param sizeOfDeck The size of cards of deck. 20 or 24.
     * @param numberOfCardsInHand The number of cards in hand. 5 or 6.
     * @param whoStartGame Who start the game. Local or opponent or random player.
     * @param schnapsenButton If this variable true, then enabled the schnapsen button.
     */
    Server( Knapsen::TypeOfCards typeOfCards, 
            int sizeOfDeck, 
            int numberOfCardsInHand, 
            Knapsen::WhoStartGame whoStartGame,
            bool schnapsenButton
          );
    
    /*!
     * The destructor. Remove variables.
     */
    ~Server();

    /*!
     * Add a bot to the server.
     * 
     * @param name The bot's name.
     * @param difficulty The bot's difficulty.
     */
    void addBot( QString name, Knapsen::GameDifficulty difficulty );
    
    /*!
     * Start the game. Send initialize to the players, send cards, etc.
     */
    void startGame();
    
signals:
    /*!
     * A new player connected.
     * 
     * @param name The player's name.
     */
    void signalPlayerConnected( QString name );
    
    /*!
     * Player disconnected.
     * 
     * @param name The player's name.
     */
    void signalPlayerDisconnected( QString name );
    
    /*!
     * The server is full.
     */
    void signalServerFull();
    
    /*!
     * The server is empty.
     */
    void signalServerEmpty();
};

#endif //SERVER_H
