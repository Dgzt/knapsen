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
 * The Client class.
 */

#ifndef CLIENT_H
#define CLIENT_H

#include "player.h"

class Trump;

class Client : public Player
{
    Q_OBJECT
    
    //Starting size of cards of deck
    int mStartDeckSize;
    
    //Actual size of cards of deck 
    int mDeckSize;
    
    //Central cards
    QList< Card* > mCentralCards;
    
    //Trump
    Trump *mTrump;
    
    //The list of commands
    QList<QString> mCommandList;
    
    //The commands under processing
    bool mCommandsUnderProcessing;
    
    //The id of the last selected player card
    int mLastSelectedCardId;
    
    //Get the values
    QStringList getValues( const QString& );
    
    
private slots:
    /*!
     * The socket connected to the server and send the player's name.
     */
    void slotConnected();
    
protected:
    /*!
     * New command.
     * 
     * @param command The new command.
     */
    virtual void newCommand( QString command );
    
    /*!
     * Clear the central cards. Delete all cards.
     */
    void clearCentralCards(){ while( !mCentralCards.empty() ) delete mCentralCards.takeFirst(); }
    
    /*!
     * Return with the actual size of cards of deck.
     * 
     * @return The size of cards of deck.
     */
    int getDeckSize(){ return mDeckSize; }
    
    /*!
     * Get the central cards. 
     * 
     * @return The central cards.
     */
    const QList< Card* >& getCentralCards(){ return mCentralCards; }
    
    /*!
     * Get the trump. 
     * 
     * @return Trump.
     */
    Trump* getTrump(){ return mTrump; }
    
    /*!
     * Send to the server the player changed the trump card.
     */
    void sendChangeTrumpCard(){ sendCommand( CHANGE_TRUMP_CARD_COMMAND ); }
    
    /*!
     * Initialize the table. Set suit of cards, set size of cards of deck,
     * set opponent's name. This is a command function.
     * 
     * @param commandValue Command values.
     */
    void commandInitializeTable( const QString& commandValue );
    
    /*!
     * New game. This is a command function.
     */
    void commandNewGame();
    
    /*!
     * New round. This is a command function.
     */
    void commandNewRound();
    
    /*!
     * Add the card to the player. This is a command function.
     * 
     * @param commandValue Command values.
     */
    void commandNewPlayerCard( const QString& commandValue );
    
    /*!
     * Add a card to the opponent. This is a command function.
     */
    void commandNewOpponentCard();
    
    /*!
     * Add the trump card to the player' cards. This is a command function.
     */
    void commandNewPlayerTrumpCard();
    
    /*!
     * Add the trump card to the opponent's cards. This is a command function.
     */
    void commandNewOpponentTrumpCard();
    
    /*!
     * New trump card. This is a command function.
     * 
     * @param commandValue Command values.
     */
    void commandNewTrumpCard( const QString& commandValue );
    
    /*!
     * New trump card and remove the old trump card.
     * This is a command function.
     * 
     * @param commandValue Command values.
     */
    void commandOpponentChangeTrumpCard( const QString& commandValue );
    
    /*!
     * Set the twenty button visible.
     * This is a command function.
     */
    void commandTwentyButtonVisible();
    
    /*!
     * Set the forty button visible.
     * This is a command function.
     */
    void commandFortyButtonVisible();
    
    /*!
     * Set the schnapsen button visible.
     * This is a command function.
     */
    void commandSchnapsenButtonVisible();
    
    /*!
     * Set the close button visible.
     * This is a command function.
     */
    void commandCloseButtonVisible();
    
    /*!
     * Set true the selectable of trump card.
     * This is a command function.
     */
    void commandTrumpCardSelectable();
    
    /*!
     * Set true the selectable of all player's cards.
     * This is a command function.
     */
    void commandSelectableAllCards();
    
    /*!
     * Set the the certain player's cards.
     * This is a command function.
     */
    void commandSelectableCertainCards();
    
    /*!
     * Emit signal the opponent clicked to the schnapsen button.
     * This is a command function.
     */
    void commandOpponentSchnapsenButtonClicked();
    
    /*!
     * Emit signal the opponent clicked to the forty button.
     * This is a command function.
     */
    void commandOpponentFortyButtonClicked();
    
    /*!
     * Emit signal the opponent clicked to the twenty button.
     * This is a command function.
     */
    void commandOpponentTwentyButtonClicked();
    
    /*!
     * Emit signal the opponent's in action.
     * This is a command function.
     */
    void commandOpponentInAction();
    
    /*!
     * Add the opponent's selected card to the central cards.
     * This is a command function.
     * 
     * @param commandValue Command values.
     */
    void commandOpponentSelectedCard( const QString& commandValue );
    
    /*!
     * Emit signal the opponent shows their cards. Cards are queen and king. 
     * This is a command function.
     * 
     * @param commandValue Command values
     */
    void commandVisibleOpponentCards( const QString& commandValue );
    
    /*!
     * Change the player's tricks and emit the signal. This is a command function.
     * 
     * @param commandValue Command values.
     */
    void commandPlayerTricksChanged( const QString& commandValue );
    
    /*!
     * Change the player's scores and emit the signal. This is a command function.
     * 
     * @param commandValue Command value.
     */
    void commandPlayerScoresChanged( const QString& commandValue );
    
    /*!
     * Emit signal the opponent's tricks changed. This is a command function.
     * 
     * @param commandValue Command values.
     */
    void commandOpponentTricksChanged( const QString& commandValue );
    
    /*!
     * Emit signal the opponent's scores changed. This is a command function.
     * 
     * @param commandValue Command values.
     */
    void commandOpponentScoresChanged( const QString& commandValue );
    
    /*!
     * Emit the signalCloseDeck() signal. This is a command function.
     */
    void commandOpponentCloseButtonClicked(){ emit signalCloseDeck(); }
    
    /*!
     * Clear the central cards and emit the signalPlayerGetCentralCards() signal.
     */
    void commandPlayerGetCentralCards();
    
    /*!
     * Clear the central cards and emit the signalOpponentGetCentralCards() signal.
     */
    void commandOpponentGetCentralCards();
    
    /*!
     * Show who won the round.
     * 
     * @param commandValue Command value.
     */
    void commandEndRound( const QString& commandValue );
    
    /*!
     * Show who whon the game.
     * 
     * @param commandValue Command value.
     */
    void commandEndGame( const QString& commandValue );
    
protected slots:
    /*!
     * Send to the server the last selected card ID. 
     */
    void slotSendSelectedCardId(){ sendCommand( SELECTED_CARD_ID_COMMAND+QString::number( mLastSelectedCardId ) ); }
    
    /*!
     * Emit the signalPlayerGetCentralCard signal.
     */
    void slotPlayerGetCentralCards(){ emit signalPlayerGetCentralCards(); }
    
    /*!
     * Emit the signalOpponentGetCentralCard signal.
     */
    void slotOpponentGetCentralCards(){ emit signalOpponentGetCentralCards(); }
    
public:
    enum GameErrorType{ NameIsBusy, ServerIsFull, OpponentDisconnected };
    
    /*!
     * Set the name and setup the client.
     * 
     * @param name Player's name.
     */
    Client( QString name );
    
    /*!
     * Clear the variables.
     */
    ~Client();
    
    /*!
     * Send to the server start the next round.
     */
    void startNextRound(){ sendCommand( START_NEXT_ROUND_COMMAND ); }
    
    /*!
     * Send to the server start the next game.
     */
    void startNextGame(){ sendCommand( START_NEXT_GAME_COMMAND ); }
    
public slots:
    /*!
     * Select the id of card. Set false to the all cards.
     * Move the card to the central cards. Hide buttons.
     * If the trump card is selectable then set to false.
     * 
     * @param cardId Selected id of card.
     */
    //void slotSelectCardId( int cardId );
    void slotSelectCardId( int cardId, int delay );
    
    
    /*!
     * Change trump card with a card. The trump card
     * and the lowst card must be selectable. 
     */
    void slotSelectTrumpCard();
    
    /*!
     * Click to the twent button. Then only the king and
     * the queen will be selectable.
     */
    void slotTwentyButtonClicked();
    
    /*!
     * Click to the forty button. Then only the king and
     * the queen will be selectable.
     */
    void slotFortyButtonClicked();
    
    /*!
     * Click to the schnapsen button.
     */
    void slotSchnapsenButtonClicked();
    
    /*!
     * Click to the deck close button.
     */
    void slotCloseButtonClicked();
    
    /*!
     * Countinue process the commands.
     */
    void slotProcessCommands();
    
signals:
    /*!
     * Error when the game can't continue.
     * 
     * @param gameErrorType Type of game error.
     */
    void signalGameError( Client::GameErrorType gameErrorType );
    
    /*!
     * Initialize the game.
     * 
     * @param playerName The player's name.
     * @param opponentName The opponent's name.
     * @param typeOfCards The type of cards.
     */
    void signalInitialize( QString playerName, QString opponentName, Knapsen::TypeOfCards typeOfCards );
    
    /*!
     * The opponent has a new card.
     * 
     * @param lastCard Was the last card in the cards of deck.
     */
    void signalNewOpponentCard( bool lastCard );
    
    /*!
     * The opponent gets the trump card.
     */
    void signalNewOpponentCardTrumpCard();
    
    /*!
     * The player has a new card.
     * 
     * @param lastCard Was the last card in the cards of deck.
     * @param card The new card.
     */
    void signalNewPlayerCard( bool lastCard, Card* card );
    
    /*!
     * The player gets the trump card.
     */
    void signalNewPlayerCardTrumpCard();
    
    /*!
     * New trump card.
     * 
     * @param card The new trump card.
     */
    void signalNewTrumpCard( Card* card );
    
    /*!
     * The opponent changed the trump card.
     * 
     * @param cardId The position of the new trump card.
     * @param newTrumpCard The new trump card.
     */
    void signalOpponentChangeTrumpCard( int cardId, Card* newTrumpCard );
    
    /*!
     * The player changed trump card.
     * 
     * @param cardId The position of the new trump card.
     */
    void signalPlayerChangeTrumpCard( int cardId );
    
    /*!
     * The opponent gets the central cards.
     */
    void signalOpponentGetCentralCards();
    
    /*!
     * The player gets the central cards
     */
    void signalPlayerGetCentralCards();
    
    /*!
     * The opponent selected card.
     * 
     * @param cardId The position of the selected card.
     * @param card The selected card.
     */
    void signalOpponentSelectedCard( int cardId, Card* card );
    
    /*!
     * Changed the opponent's tricks.
     * 
     * @param tricks The new opponent's tricks.
     */
    void signalOpponentTricksChanged( int tricks );
    
    /*!
     * Changed the opponent's scores.
     * 
     * @param scores The new opponent's scores.
     */
    void signalOpponentScoresChanged( int scores );
    
    /*!
     * Close deck.
     */
    void signalCloseDeck();
    
    /*!
     * Changed the selectable of the trump card.
     * 
     * @param selectable The selectable of the trump card.
     */
    void signalTrumpCardSelectableChanged( bool selectable );
    
    /*!
     * The opponent shows their cards after meld marriages.
     * 
     * @param card1Id The position of the first card.
     * @param card1 The first card.
     * @param card2id The position of the second card.
     * @param card2 The second card.
     */
    void signalShowOpponentCards( int card1Id, Card card1, int card2Id, Card card2 );
    
    /*!
     * The opponent clicked to the schnapsen button.
     */
    void signalOpponentSchnapsenButtonClicked();
    
    /*!
     * The opponent clicked to the forty button.
     */
    void signalOpponentFortyButtonClicked();
    
    /*!
     * The opponent clicked to the twenty button.
     */
    void signalOpponentTwentyButtonClicked();
    
    /*!
     * New round.
     */
    void signalNewRound();
    
    /*!
     * The player is in action.
     */
    void signalPlayerInAction();
    
    /*!
     * The opponent is in action.
     */
    void signalOpponentInAction();
    
    /*!
     * New game.
     */
    void signalNewGame();
    
    /*!
     * End of the current round.
     * 
     * @param name The name of the round winner.
     * @param scores The scores of the round winner.
     */
    void signalEndRound( QString name, int scores );
    
    /*!
     * End of the current game.
     * 
     * @param name The name of the game winner.
     */
    void signalEndGame( QString name );
    
};      
    
#endif //CLIENT_H
