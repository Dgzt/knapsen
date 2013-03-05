/*!
 * @file
 * @author  Zsuro Tibor <zsurotibor@gmail.com>
 * @version 1.0
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
//static const QString OPPONENT_SELECTED_CARD_ID_COMMAND = ":OPPONENT_SELECTED_CARD_ID=";
//static const QString OPPONENT_ADD_NEW_CENTRAL_CARD_COMMAND = ":OPPONENT_ADD_NEW_CENTRAL_CARD=";
static const QString OPPONENT_SELECTED_CARD_COMMAND = ":OPPONENT_SELECTED_CARD=";

//
static const QString PLAYER_TRICKS_CHANGED_COMMAND = ":PLAYER_TRICKS_CHANGED=";
static const QString OPPONENT_TRICKS_CHANGED_COMMAND = ":OPPONENT_TRICKS_CHANGED=";
static const QString PLAYER_SCORES_CHANGED_COMMAND = ":PLAYER_SCORES_CHANGED=";
static const QString OPPONENT_SCORES_CHANGED_COMMAND = ":OPPONENT_SCORES_CHANGED=";

//
static const QString OPPONENT_DISCONNECTED_COMMAND = ":OPPONENT_DISCONNECTED";

//
//static const QString CLEAR_CENTRAL_CARDS_COMMAND = ":CLEAR_CENTRAL_CARDS";
static const QString PLAYER_GET_CENTRAL_CARDS_COMMAND = ":PLAYER_GET_CENTRAL_CARDS";
static const QString OPPONENT_GET_CENTRAL_CARDS_COMMAND = ":OPPONENT_GET_CENTRAL_CARDS";

//
static const QString TWENTY_BUTTON_VISIBLE_COMMAND = ":TWENTY_BUTTON_VISIBLE";
static const QString FORTY_BUTTON_VISIBLE_COMMAND = ":FORTY_BUTTON_VISIBLE";
static const QString SCHNAPSEN_BUTTON_VISIBLE_COMMAND = ":SCHNAPSEN_BUTTON_VISIBLE";
static const QString TWENTY_BUTTON_CLICKED_COMMAND = ":TWENTY_BUTTON_CLICKED";
static const QString FORTY_BUTTON_CLICKED_COMMAND = ":FORTY_BUTTON_CLICKED";
static const QString SCHNAPSEN_BUTTON_CLICKED_COMMAND = ":SCHNAPSEN_BUTTON_CLICKED";
static const QString VISIBLE_OPPONENT_CARDS_COMMAND = ":VISIBLE_OPPONENT_CARDS=";

//
static const QString CLOSE_BUTTON_VISIBLE_COMMAND = ":CLOSE_BUTTON_VISIBLE";
static const QString CLOSE_BUTTON_CLICKED_COMMAND = ":CLOSE_BUTTON_CLICKED";
static const QString OPPONENT_CLICKED_TO_CLOSE_BUTTON_COMMAND = ":OPPONENT_CLICKED_TO_CLOSE_BUTTON";

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
    
    //int mNumberOfCardsInHand;
    //Card** mCards;
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
    void slotReadyRead();
    void slotDisconnected(){ emit signalPlayerDisconnected( this ); }
    
protected:
    QString getCommandPartOfCommand( QString );
    QString getValuePartOfCommand( QString );
    
    void newGame();
    void newRound();
    
    void setLowestCard( int );
    Card::Type getLowestCardType(){ return mLowestCardType; }
    
    //int addNewCard( Card* );
    void addNewCard( Card* card ){ mCards.append( card ); }
    //void removeCard( int );

    
    
    //Set selectable OR not selectable all avalibe card
    void setSelectableAllCards( bool );
    //Set selectable cards, which suit of cards equal the suit of central card or suit of trump card
    void setSelectableCertainCards( CentralCards* , Trump* );
    
    void twentyButtonClicked();
    void fortyButtonClicked();
    
    void setCloseButtonVisible( bool visible ){ mCloseButtonVisible = visible; emit signalCloseButtonVisible( visible ); }
    bool isCloseButtonVisible() const{ return mCloseButtonVisible; }
    
    void setTricks( int tricks ){ mTricks = tricks; }
    void setScores( int scores ){ mScores = scores; }
    
    //Send command to socket
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
    
    bool haveRegularMarriages( const Trump* trump ) const;
    bool haveTrumpMarriages( const Trump* trump ) const;
    
    bool canChangeTrumpCard( Trump* ) const;
    int changeTrumpCard( Trump* );
    
    void setTwentyButtonVisible( bool );
    bool isTwentyButtonVisible() const{ return mTwentyButtonVisible; }
    void setFortyButtonVisible( bool );
    bool isFortyButtonVisible() const{ return mFortyButtonVisible; }
    //
    void setSchnapsenButtonVisible( bool );
    bool isSchnapsenButtonVisible(){ return mSchnapsenButtonVisible; }
    //
    
    void setSelectableRegularMarriagesCards( Trump* );
    void setSelectableTrumpMarriagesCards( Trump* );
    
    //-- Commands --
    
    //
    void sendNameIsBusy(){ sendCommand( NAME_IS_BUSY_COMMAND ); }
    
    void sendServerIsFull(){ sendCommand( SERVER_IS_FULL_COMMAND ); }
    //

    //void sendInitializeTable( QString, Knapsen::TypeOfCards, int, int );
    void sendInitializeTable( QString, Knapsen::TypeOfCards, int );
    
    //Set and send the new card
    void sendNewCard( Card* );
    void sendNewCardTrumpCard(){ sendCommand( NEW_PLAYER_CARD_TRUMP_CARD_COMMAND ); }
    
    //Send to player the new opponent card id
    void sendNewOpponentCard(){ sendCommand( NEW_OPPONENT_CARD_COMMAND ); }
    void sendNewOpponentCardTrumpCard(){ sendCommand( NEW_OPPONENT_CARD_TRUMP_CARD_COMMAND ); }
    
    //Set and send the new trump card
    void sendNewTrumpCard( Trump* );
    
    //Send the opponent change trump card
    void sendOpponentChangeTrumpCard( int, Trump* );
    
    //void sendClearTrumpCard(){ sendCommand( CLEAR_TRUMP_CARD_COMMAND ); }
    
    //Set and send enabled all card
    void sendSelectableAllCards();
    
    //void sendSelectableCertainCards();
    void sendSelectableCertainCards( CentralCards *, Trump* );
    
    void sendOpponentInAction(){ sendCommand( OPPONENT_IN_ACTION_COMMAND ); }
    
    //void sendOpponentSelectedCardId( int id ){ sendCommand( OPPONENT_SELECTED_CARD_ID_COMMAND+QString::number( id ) ); }
    
    //void sendOpponentAddNewCentralCard( const Card* );
    
    //
    void sendOpponentSelectedCard( int, const Card* );
    //
    
    void sendOpponentTricksChanged( int value ){ sendCommand( OPPONENT_TRICKS_CHANGED_COMMAND+QString::number( value ) ); }
    void sendOpponentScoresChanged( int value ){ sendCommand( OPPONENT_SCORES_CHANGED_COMMAND+QString::number( value ) ); }
    
    void sendOpponentDisconnected(){ sendCommand( OPPONENT_DISCONNECTED_COMMAND ); }
    
    //void sendClearCentralCards(){ sendCommand( CLEAR_CENTRAL_CARDS_COMMAND ); }
    void sendPlayerGetCentralCards(){ sendCommand( PLAYER_GET_CENTRAL_CARDS_COMMAND ); }
    void sendOpponentGetCentralCards(){ sendCommand( OPPONENT_GET_CENTRAL_CARDS_COMMAND ); }
    
    void sendTwentyButtonVisible(){ sendCommand( TWENTY_BUTTON_VISIBLE_COMMAND ); }
    void sendFortyButtonVisible(){ sendCommand( FORTY_BUTTON_VISIBLE_COMMAND ); }
    //
    void sendSchnapsenButtonVisible(){ sendCommand( SCHNAPSEN_BUTTON_VISIBLE_COMMAND ); }
    //
    
    void sendCloseButtonVisible();
    
    void sendOpponentClickedToCloseButton(){ sendCommand( OPPONENT_CLICKED_TO_CLOSE_BUTTON_COMMAND ); }
    
    void sendSelectableTrumpCard(){ sendCommand( TRUMP_CARD_SELECTABLE_COMMAND ); }
    void sendChangeTrumpCard(){ sendCommand( CHANGE_TRUMP_CARD_COMMAND ); }     
    
    //If the opponent clicked to twenty or forty button, then show that cards
    void sendVisibleOpponentCards( int, Card*, int, Card* );
    
    void sendNewRound(){ newRound(); sendCommand( NEW_ROUND_COMMAND ); }
    
    void sendNewGame(){ newGame(); sendCommand( NEW_GAME_COMMAND ); }
    
    void sendEndRound( QString winnerName, int winnnerScores ){ sendCommand( END_ROUND_COMMAND+winnerName+","+QString::number( winnnerScores ) ); }
    
    void sendEndGame( QString winnerName ){ sendCommand( END_GAME_COMMAND+winnerName ); }
    
    void sendCommandsEnd(){ sendCommand( COMMANDS_END_COMMAND ); }
    
signals:
    //Signals to server
    void signalNewPlayer( Player* );
    void signalPlayerDisconnected( Player* );
    //void signalSelectedCard( Card*, int );
    void signalSelectedCardId( int );
    void signalTwentyButtonClicked();
    void signalFortyButtonClicked();
    void signalSchnapsenButtonClicked();
    void signalCloseButtonClicked();
    void signalChangeTrumpCard( Player* );
    
    void signalStartNextRound( Player* );
    void signalStartNextGame( Player* );
    
    
    //Signals to player
    void signalPlayerCardSelectableChanged( int, bool );
    void signalTwentyButtonVisible( bool );
    void signalFortyButtonVisible( bool );
    void signalSchnapsenButtonVisible( bool );
    void signalCloseButtonVisible( bool );
    
};

#endif //PLAYER_H
