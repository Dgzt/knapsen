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
    
    QList<QString> mCommandList;
    //
    bool mCommandsUnderProcessing;
    //
    
    //The id of the last selected player card
    int mLastSelectedCardId;
    
    //
    //QList<QString>* getValues( QString );
    QStringList getValues( const QString& );
    //
    
    virtual void newCommand( QString );
    
private slots:
    /*!
     * The socket connected to the server and send the player's name.
     */
    void slotConnected();
    
protected:
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
    
    void sendChangeTrumpCard(){ sendCommand( CHANGE_TRUMP_CARD_COMMAND ); }
    
    ///////////////////////////////
    
    /*!
     * Initialize table. Set suit of cards, set size of cards of deck,
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
    
    void startNextRound(){ sendCommand( START_NEXT_ROUND_COMMAND ); }
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
    
    //void signalHideTrumpCard();
    //
    void signalTrumpCardSelectableChanged( bool );
    //
    
    void signalShowOpponentCards( int, Card, int, Card );
    
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
