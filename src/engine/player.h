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
 * The Player class represents a base of player who has cards, scores, tricks, etc.
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <QtNetwork/QTcpSocket>
#include "card.h"
#include "enums.h"

//Name
static const QString NAME_COMMAND = ":NAME=";
static const QString NAME_IS_BUSY_COMMAND =":NAME_IS_BUSY";

//Server
static const QString SERVER_IS_FULL_COMMAND = ":SERVER_IS_FULL";

//Configs
static const QString TYPE_OF_CARDS_GERMAN_SUITS_VALUE = "GERMAN_SUIT";
static const QString TYPE_OF_CARDS_FRENCH_SUITS_VALUE = "FRENCH_SUIT";
static const QString INITIALIZE_TABLE_COMMAND = ":INITIALIZE_TABLE=";

//Cards
static const QString NEW_PLAYER_CARD_COMMAND = ":NEW_PLAYER_CARD=";
static const QString NEW_PLAYER_CARD_TRUMP_CARD_COMMAND = ":NEW_PLAYER_CARD_TRUMP_CARD";
static const QString NEW_OPPONENT_CARD_COMMAND = ":NEW_OPPONENT_CARD";
static const QString NEW_OPPONENT_CARD_TRUMP_CARD_COMMAND = ":NEW_OPPONENT_CARD_TRUMP_CARD";
static const QString NEW_TRUMP_CARD_COMMAND = ":NEW_TRUMP_CARD=";
static const QString TRUMP_CARD_SELECTABLE_COMMAND = ":TRUMP_CARD_SELECTABLE";
static const QString CHANGE_TRUMP_CARD_COMMAND = ":CHANGE_TRUMP_CARD";
static const QString OPPONENT_CHANGE_TRUMP_CARD_COMMAND = ":OPPONENT_CHANGE_TRUMP_CARD=";

//
static const QString SELECTABLE_ALL_CARDS_COMMAND = ":SELECTABLE_ALL_CARDS";

//Selectable cards that's type equal with trump card's type or central card's type
static const QString SELECTABLE_CERTAIN_CARDS_COMMAND = ":SELECTABLE_CERTAIN_CARDS";

//
static const QString OPPONENT_IN_ACTION_COMMAND = ":OPPONENT_IN_ACTION";
//

static const QString SELECTED_CARD_ID_COMMAND = ":SELECTED_CARD_ID=";
static const QString OPPONENT_SELECTED_CARD_COMMAND = ":OPPONENT_SELECTED_CARD=";

//
static const QString PLAYER_TRICKS_CHANGED_COMMAND = ":PLAYER_TRICKS_CHANGED=";
static const QString OPPONENT_TRICKS_CHANGED_COMMAND = ":OPPONENT_TRICKS_CHANGED=";
static const QString PLAYER_SCORES_CHANGED_COMMAND = ":PLAYER_SCORES_CHANGED=";
static const QString OPPONENT_SCORES_CHANGED_COMMAND = ":OPPONENT_SCORES_CHANGED=";

//
static const QString OPPONENT_DISCONNECTED_COMMAND = ":OPPONENT_DISCONNECTED";

//
static const QString PLAYER_GET_CENTRAL_CARDS_COMMAND = ":PLAYER_GET_CENTRAL_CARDS";
static const QString OPPONENT_GET_CENTRAL_CARDS_COMMAND = ":OPPONENT_GET_CENTRAL_CARDS";

//
static const QString TWENTY_BUTTON_VISIBLE_COMMAND = ":TWENTY_BUTTON_VISIBLE";
static const QString FORTY_BUTTON_VISIBLE_COMMAND = ":FORTY_BUTTON_VISIBLE";
static const QString SCHNAPSEN_BUTTON_VISIBLE_COMMAND = ":SCHNAPSEN_BUTTON_VISIBLE";

static const QString TWENTY_BUTTON_CLICKED_COMMAND = ":TWENTY_BUTTON_CLICKED";
static const QString OPPONENT_TWENTY_BUTTON_CLICKED_COMMAND = ":OPPONENT_TWENTY_BUTTON_CLICKED";

static const QString FORTY_BUTTON_CLICKED_COMMAND = ":FORTY_BUTTON_CLICKED";
static const QString OPPONENT_FORTY_BUTTON_CLICKED_COMMAND = ":OPPONENT_FORTY_BUTTON_CLICKED";

static const QString SCHNAPSEN_BUTTON_CLICKED_COMMAND = ":SCHNAPSEN_BUTTON_CLICKED";
static const QString OPPONENT_SCHNAPSEN_BUTTON_CLICKED_COMMAND = ":OPPONENT_SCHNAPSEN_BUTTON_CLICKED";

static const QString VISIBLE_OPPONENT_CARDS_COMMAND = ":VISIBLE_OPPONENT_CARDS=";

//
static const QString CLOSE_BUTTON_VISIBLE_COMMAND = ":CLOSE_BUTTON_VISIBLE";
static const QString CLOSE_BUTTON_CLICKED_COMMAND = ":CLOSE_BUTTON_CLICKED";
static const QString OPPONENT_CLICKED_TO_CLOSE_BUTTON_COMMAND = ":OPPONENT_CLICKED_TO_CLOSE_BUTTON";

//
static const QString OPPONENT_WANT_START_NEXT_ROUND_COMMAND = ":OPPONENT_WANT_START_NEXT_ROUND";
//
static const QString NEW_ROUND_COMMAND = ":NEW_ROUND";
static const QString END_ROUND_COMMAND = ":END_ROUND=";
static const QString START_NEXT_ROUND_COMMAND = ":START_NEXT_ROUND";

static const QString COMMANDS_END_COMMAND = ":COMMANDS_END";

//
static const QString NEW_GAME_COMMAND = ":NEW_GAME";

//
static const QString END_GAME_COMMAND = ":END_GAME=";
static const QString START_NEXT_GAME_COMMAND = ":START_NEXT_GAME";
//

class CentralCards;
class Trump;

class Player : public QTcpSocket
{
    Q_OBJECT
    
    //Player's name
    QString mName;
    
    Card::Type mLowestCardType;
    
    QList< Card* > mCards;
    
    //Player's tricks
    int mTricks;
    
    //Player's scores
    int mScores;
    
    //If this variable is visible, then visible the twenty button
    bool mTwentyButtonVisible;
    //If this variable is visible, then visible the forty button
    bool mFortyButtonVisible;
    //If this variable is visible, then visible the close button
    bool mCloseButtonVisible;
    
    //If this variable is visible, then visible the schnapsen button
    bool mSchnapsenButtonVisible;
    
    
    virtual void newCommand( QString );
    
    bool setSelectableCardsWhatEqualSuit( Card::Suit );
    
private slots:
    /*!
     * Ready process the command.
     */
    void slotReadyRead();
    
    /*!
     * The socket disconnected.
     */
    void slotDisconnected(){ emit signalPlayerDisconnected( this ); }
    
protected:
    /*!
     * Return with the name part of command.
     * 
     * @param command The command.
     * @return The name part of command.
     */
    QString getCommandName( QString command );
    
    /*!
     * Return with value part of command.
     * 
     * @param command The command.
     * @return The value part of vommand.
     */
    QString getCommandValue( QString command );
    
    /*!
     * Clear the player's scores.
     */
    void newGame();
    
    /*!
     * Remove the player's cards and clear the player's tricks.
     */
    void newRound();
    
    /*!
     * Set the lowest card. If the given value is 20 then the
     * lowest card is Jack else the given value is 24 then the 
     * the lowest card is Nine.
     * 
     * @param deckSize The size of deck.
     */
    void setLowestCard( int deckSize );
    
    /*!
     * Return with the lowest card. This is Jack or Nine.
     * 
     * @return The lowest card.
     */
    Card::Type getLowestCardType(){ return mLowestCardType; }
    
    /*!
     * Add the new card to the cards. The new card will the last.
     * 
     * @param card The new card.
     */
    void addNewCard( Card* card ){ mCards.append( card ); }
    
    /*!
     * If the given value is TRUE then the all cards are selectable
     * else the given value is FALSE then the all cards aren't selectable.
     * 
     * @param selectable True/False.
     */
    void setSelectableAllCards( bool selectable );
    
    /*!
     * Set true selectable certain cards. 
     * 
     * @param centralCards The central cards.
     * @param trump The trump.
     */
    void setSelectableCertainCards( const QList< Card* >& centralCards, const Trump* trump );
    
    /*!
     * The twenty button will hidden. If the forty button is visible then
     * this button will hidden. The all cards won't selectable.
     */
    void twentyButtonClicked();
    
    /*!
     * The forty button will hidden. If the twenty button is visible then
     * this button will hidden. The all cards won't selectable.
     */
    void fortyButtonClicked();
    
    /*!
     * Set the close button visible. If the given value is TRUE then
     * the button will visible. If the given value is FALSE then the 
     * button will hidden.
     * 
     * @param visible True/False.
     */
    void setCloseButtonVisible( bool visible ){ mCloseButtonVisible = visible; emit signalCloseButtonVisible( visible ); }
    
    /*!
     * Return with state of close button. If the close button is visible then
     * return with true else false.
     * 
     * @return True/False.
     */
    bool isCloseButtonVisible() const{ return mCloseButtonVisible; }
    
    /*!
     * Set the player's tricks. 
     * 
     * @param tricks The player's tricks.
     */
    void setTricks( int tricks ){ mTricks = tricks; emit signalPlayerTricksChanged( mTricks ); }
    
    /*!
     * Set the player's scores.
     * 
     * @param scores The player's scores.
     */
    void setScores( int scores ){ mScores = scores; emit signalPlayerScoresChanged( mScores ); }
    
    /*!
     * Send the command text to the socket.
     * 
     * @param command Command.
     */
    void sendCommand( QString command ){ write( command.toAscii() ); }
    
public:
    /*!
     * Constructor sets the name from a given value and sets the variables to the default value.
     *
     * @param name The player's name.
     */
    Player( QString name );
    
    /*!
     * Destructor clear the class.
     */
    ~Player();
    
    /*!
     * Get the player's name.
     * 
     * @return The player's name.
     */
    QString getName() const { return mName; }
    
    /*!
     * Get the number of cards.
     * 
     * @return Number of cards.
     */
    int getNumberOfCards(){ return mCards.size(); }
    
    /*!
     * Get the card. This function isn't remove the card from the player.
     * 
     * @param id The id of card.
     * @return The card.
     */
    Card* getCard( int id ){ return mCards.at( id ); }
    
    /*!
     * Take the card. This function remove the card from the player.
     * 
     * @param id The id of card.
     * @return The card.
     */
    Card* takeCard( int id ){ return mCards.takeAt( id ); }
    
    /*!
     * Get position of pair of card. If the given card is King then return 
     * position of equal suit Queen. Else the given card is Queen then return 
     * position of equal suit King.  
     * 
     * @param id Position of card. The card must be Queen or King.
     * @return Return position of pair of card. 
     */
    int getPositionOfPairOfCardId( int id );
    
    /*!
     * Add the new tricks to player and send the number of all tricks to client.
     * 
     * @param newTricks The new tricks.
     */
    void addTricks( int newTricks );
    
    /*!
     * Get the number of tricks.
     * 
     * @return Number of tricks.
     */
    int getTricks() const{ return mTricks; }
    
    /*!
     * Add the new scores to player and send the number of all scores to client.
     * 
     * @param newScores The new scores.
     */
    void addScores( int newScores );
    
    /*!
     * Get the number of scores.
     * 
     * @return Number of scores.
     */
    int getScores() const{ return mScores; }
    
    /*!
     * If the player have regular marriage (Queen + King what 
     * suits of cards aren't equal with suit of trump card) then return true else false.
     * 
     * @param trump The trump.
     * @return True/False.
     */
    bool haveRegularMarriage( const Trump* trump ) const;
    
    /*!
     * If the player have trump marriage (Queen + King what
     * suit of cards are equal with suit of trump card) then return true, else false.
     * 
     * @param trump The trump.
     * @return True/False.
     */
    bool haveTrumpMarriage( const Trump* trump ) const;
    
    /*!
     * If the player have smallest card (Jack or Nine) and
     * suit of this card equal with suit of trump card then return true else false.
     * 
     * @param trump The trump.
     * @return True/False.
     */
    bool canChangeTrumpCard( const Trump* trump ) const;
    
    /*!
     * Change the trump card with the lowest card. The suit of lowst card is equal with 
     * suit of trump card. The card is Jack or Nine what was set in the setting.
     * 
     * @param trump The trump.
     * @return What was the position the lowest card.
     */
    int changeTrumpCard( Trump* trump );
    
    /*!
     * If the given value is true then the twenty button 
     * will visible and enabled, if the given value is false
     * then will invisible and disabled.
     * 
     * @param visible Set the twenty button visible.
     */
    void setTwentyButtonVisible( bool visible );
    
    /*!
     * Return the state of twenty button. If the twenty
     * button is visible then return true else false.
     * 
     * @return True/False.
     */
    bool isTwentyButtonVisible() const{ return mTwentyButtonVisible; }
    
    /*!
     * If the given value is true then the forty button
     * will visible and enabled, if the given value is false
     * then will invisible and disabled.
     * 
     * @param visible Set the forty button visible.
     */
    void setFortyButtonVisible( bool visible );
    
    /*!
     * Return the sate of forty button. If the forty 
     * button is visible then return true else false. 
     * 
     * @return True/False.
     */
    bool isFortyButtonVisible() const{ return mFortyButtonVisible; }
    
    /*!
     * If the given value is true then the schnapsen button
     * will visible and enabled, if the given value is false
     * then will invisible and disabled.
     * 
     * @param visible Set the schnapsen button visible.
     */
    void setSchnapsenButtonVisible( bool visible );
    
    /*!
     * Return the sate of schnapsen button. If the schnapsen 
     * button is visible then return true else false. 
     * 
     * @return True/False.
     */
    bool isSchnapsenButtonVisible(){ return mSchnapsenButtonVisible; }
    
    /*!
     * Set true King and Queen cards's selectable that suit aren't
     * equal with suit of trump cards.
     * 
     * @param trump The trump.
     */
    void setSelectableRegularMarriageCards( const Trump* trump );
    
    /*!
     * Set true King and Queen cards's selectable that suit are
     * equal with suit of trump cards.
     * 
     * @param trump The trump.
     */
    void setSelectableTrumpMarriageCards( const Trump* trump );
    
    //-- Commands --
    
    /*!
     * Send to the client the name is busy.
     */
    void sendNameIsBusy(){ sendCommand( NAME_IS_BUSY_COMMAND ); }
    
    /*!
     * Send to the client the server is full.
     */
    void sendServerIsFull(){ sendCommand( SERVER_IS_FULL_COMMAND ); }
    
    /*!
     * Send to the client the command. This will initialize the game, setup 
     * opponent's name, type of cards and size of deck. 
     * 
     * @param opponentName The opponent's name.
     * @param typeOfCards The type of cards. German suit or French suit.
     * @param sizeOfDeck Size of deck. 20 cards wihtout nines, 24 cards with nines. 
     */
    void sendInitializeTable( QString opponentName, Knapsen::TypeOfCards typeOfCards, int sizeOfDeck );
    
    /*!
     * Add the card to the player and send to the client.
     * 
     * @param card The new card.
     */
    void sendNewCard( Card* card );
    
    //void sendNewCardTrumpCard(){ sendCommand( NEW_PLAYER_CARD_TRUMP_CARD_COMMAND ); }
    void sendNewCardTrumpCard( Card* card );
    
    /*!
     * Send to the client the opponent have a new card from deck.
     */
    void sendNewOpponentCard(){ sendCommand( NEW_OPPONENT_CARD_COMMAND ); }
    
    /*!
     * Send to the client the opponent get the trump card.
     */
    void sendNewOpponentCardTrumpCard(){ sendCommand( NEW_OPPONENT_CARD_TRUMP_CARD_COMMAND ); }
    
    /*!
     * Send to the client the new trump card.
     * 
     * @param trump The trump.
     */
    void sendNewTrumpCard( const Trump* trump );
    
    /*!
     * Send to the client the opponent changed the trump card with their card.
     * 
     * @param id The position of opponent's card what changed.
     * @param trump The new trump card.
     */
    void sendOpponentChangeTrumpCard( int id, const Trump* trump );
    
    /*!
     * Set true all cards's selectable and send to the client.
     */
    void sendSelectableAllCards();
    
    /*!
     * Set true certain cards's selectable and send to the client. 
     * 
     * @param centralCards The central cards.
     * @param trump The trump.
     */
    void sendSelectableCertainCards( const QList< Card* >& centralCards , const Trump* trump );
    
    /*!
     * Send to the client the opponent's in action.
     */
    void sendOpponentInAction(){ sendCommand( OPPONENT_IN_ACTION_COMMAND ); }
    
    /*!
     * Send to the client the opponent selected a card and its position.
     * 
     * @param id The position of card.
     * @param card The selected card.
     */
    void sendOpponentSelectedCard( int id, const Card* card );
    
    /*!
     * Send the new number of opponent's tricks to the client.
     * 
     * @param tricks The number of tricks.
     */
    void sendOpponentTricksChanged( int tricks ){ sendCommand( OPPONENT_TRICKS_CHANGED_COMMAND+QString::number( tricks ) ); }
    
    /*!
     * Send the new number of opponent's scores to the client.
     * 
     * @param scores The number of scores.
     */
    void sendOpponentScoresChanged( int scores ){ sendCommand( OPPONENT_SCORES_CHANGED_COMMAND+QString::number( scores ) ); }
    
    /*!
     * Send to the client the opponent disconnected.
     */
    void sendOpponentDisconnected(){ sendCommand( OPPONENT_DISCONNECTED_COMMAND ); }
    
    /*!
     * Send to the client the player gets the central cards.
     */
    void sendPlayerGetCentralCards(){ sendCommand( PLAYER_GET_CENTRAL_CARDS_COMMAND ); }
    
    /*!
     * Send to the client the opponent gets the central cards.
     */
    void sendOpponentGetCentralCards(){ sendCommand( OPPONENT_GET_CENTRAL_CARDS_COMMAND ); }
    
    /*!
     * Send to the client the twenty button is visible.
     */
    void sendTwentyButtonVisible(){ sendCommand( TWENTY_BUTTON_VISIBLE_COMMAND ); }
    
    /*!
     * Send to the client the forty button is visible.
     */
    void sendFortyButtonVisible(){ sendCommand( FORTY_BUTTON_VISIBLE_COMMAND ); }
    
    /*!
     * Send to the client the schnapsen button is visible.
     */
    void sendSchnapsenButtonVisible(){ sendCommand( SCHNAPSEN_BUTTON_VISIBLE_COMMAND ); }
    
    /*!
     * Send to the client the close button is visible.
     */
    void sendCloseButtonVisible();
    
    /*!
     * Send to the client the opponent clicked to the close button.
     */
    void sendOpponentClickedToCloseButton(){ sendCommand( OPPONENT_CLICKED_TO_CLOSE_BUTTON_COMMAND ); }
    
    /*!
     * Send to the client the opponent clicked to the schnapsen button.
     */
    void sendOpponentSchnapsenButtonClicked(){ sendCommand( OPPONENT_SCHNAPSEN_BUTTON_CLICKED_COMMAND ); }
    
    /*!
     * Send to the client the opponent clicked to the forty button.
     */
    void sendOpponentFortyButtonClicked(){ sendCommand( OPPONENT_FORTY_BUTTON_CLICKED_COMMAND ); }
    
    /*!
     * Send to the client the opponent clicked to the twenty button.
     */
    void sendOpponentTwentyButtonClicked(){ sendCommand( OPPONENT_TWENTY_BUTTON_CLICKED_COMMAND ); }
    
    /*!
     * Send to the client the trump card is selectable.
     */
    void sendSelectableTrumpCard(){ sendCommand( TRUMP_CARD_SELECTABLE_COMMAND ); }
    
    /*!
     * Send to the client show opponent's cards. If the opponent meld marriage ( twenty of forty ) 
     * then show those cards. 
     *
     * @param card1Pos The position of first card.
     * @param card1 The first card.
     * @param card2Pos The position of second card.
     * @param card2 The second card. 
     */
    void sendShowOpponentCards( int card1Pos, Card* card1, int card2Pos, Card* card2 );
    
    /*!
     * Send to the client start new round.
     */
    void sendNewRound(){ newRound(); sendCommand( NEW_ROUND_COMMAND ); }
    
    /*!
     * Send to the client start new game.
     */
    void sendNewGame(){ newGame(); sendCommand( NEW_GAME_COMMAND ); }
    
    /*!
     * Send to the client the current round has ended.
     * 
     * @param winnerName The name of the winner of round.
     * @param winnerScores The scores of the round's winner.
     */
    void sendEndRound( QString winnerName, int winnerScores ){ sendCommand( END_ROUND_COMMAND+winnerName+","+QString::number( winnerScores ) ); }
    
    /*!
     * Send to the client the current game has ended.
     * 
     * @param winnerName The name of the winner of game.
     */
    void sendEndGame( QString winnerName ){ sendCommand( END_GAME_COMMAND+winnerName ); }
    
    /*!
     * Send to the client start process the sended commands.
     */
    void sendCommandsEnd(){ sendCommand( COMMANDS_END_COMMAND ); }
    
signals:
    //Signals to server
    
    /*!
     * The player sent their name and connected to the server.
     * 
     * @param player This player.
     */
    void signalNewPlayer( Player* player );
    
    /*!
     * The player disconnected.
     * 
     * @param player This player.
     */
    void signalPlayerDisconnected( Player* player );
    
    /*!
     * The player selected a card.
     * 
     * @param id The position of selected card.
     */
    void signalSelectedCardId( int id );
    
    /*!
     * The player clicked to the twenty button.
     */
    void signalTwentyButtonClicked();
    
    /*!
     * The player clicked to the forty button.
     */
    void signalFortyButtonClicked();
    
    /*!
     * The player clicked to the schnapsen button.
     */
    void signalSchnapsenButtonClicked();
    
    /*!
     * The player clicked to the close button.
     */
    void signalCloseButtonClicked();
    
    /*!
     * The player change trump card.
     * 
     * @param player This player.
     */
    void signalChangeTrumpCard( Player* player );
    
    /*!
     * The player wants start next round.
     * 
     * @param player This player.
     */
    void signalStartNextRound( Player* player );
    
    /*!
     * The player wants start next game.
     * 
     * @param player This player.
     */
    void signalStartNextGame( Player* player );
    
    
    //Signals to player
    
    /*!
     * Change the enabled of card. 
     * 
     * @param id The id of card.
     * @param enabled The enabled of card.
     */
    void signalPlayerCardSelectableChanged( int id, bool enabled );
    
    /*!
     * Change the visible of twenty button.
     * 
     * @param visible The visible of twenty button.
     */
    void signalTwentyButtonVisible( bool visible );
    
    /*!
     * Change the visible of forty button.
     * 
     * @param visible The visible of forty button.
     */
    void signalFortyButtonVisible( bool visible );
    
    /*!
     * Change the visible of schnapsen button.
     * 
     * @param visible The visible of schnapsen button.
     */
    void signalSchnapsenButtonVisible( bool visible );
    
    /*!
     * Change the visible of close button.
     * 
     * @param visible The visible of close button.
     */
    void signalCloseButtonVisible( bool visible );
    
    /*!
     * Change the player's tricks.
     * 
     * @param tricks The player's tricks.
     */
    void signalPlayerTricksChanged( int tricks );
    
    /*!
     * Change the player's scores.
     * 
     * @param scores The player's scores.
     */
    void signalPlayerScoresChanged( int scores );
    
};

#endif //PLAYER_H
