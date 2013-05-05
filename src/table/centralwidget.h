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
 * The CentralWidget class is the wiget where show the items of game.
 */

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
class Text;
class ScoreTable;

class CentralWidget : public QGraphicsView
{
    Q_OBJECT
    
    Client* mClient;
    
    QSvgRenderer* mRenderer;
    //QPointer< QSvgRenderer > mRenderer;
    
    //
    Text* mOpponentName;
    //QPointer< MyTextItem > mOpponentName;
    
    Text* mPlayerName;
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
    Text* mOpponentSchnapsenText;
    Text* mOpponentFortyText;
    Text* mOpponentTwentyText;
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
     * Get the position of the opponent's schnapsen text.
     * 
     * @return The position.
     */
    QPointF getOpponentSchnapsenTextPos();
    
    /*!
     * Set the start position of the opponent's schnapsen text.
     */
    void setOpponentSchnapsenTextStartPos();
    
    /*!
     * Set the end position of the opponent's schnapsen text.
     */
    void setOpponentSchnapsenTextEndPos();
    
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
    /*!
     * Initialize the items on the widget. Setup connections to signals.
     * 
     * @param playerName The player's name.
     * @param opponentName The opponent's name.
     * @param typeOfCards The type of cards.
     */
    void slotInitialize( QString playerName, QString opponentName, Knapsen::TypeOfCards typeOfCards );
    
    /*!
     * Start the game. Show the player's and opponent's name and score table.
     */
    void slotNewGame(); 
    
    /*!
     * Start the round. Show the deck .
     */
    void slotNewRound();
    
    /*!
     * Move new card to the opponent's cards group. If this card is the 
     * last in the deck then hide the deck.
     * 
     * @param lastCard The card is the last card in the deck.
     */
    void slotNewOpponentCard( bool lastCard );
    
    /*!
     * Move the trump card to the opponent's cards group.
     */
    void slotNewOpponentCardTrumpCard();
    
    /*!
     * Add the new card to the player's cards group.
     * If this is the last card then the cards of deck will be unvisible.
     * 
     * @param lastCard True/False.
     * @param card The new card.
     */
    void slotNewPlayerCard( bool lastCard, Card* card );
    
    /*!
     * The player gets the trump card.
     */
    void slotNewPlayerCardTrumpCard();
    
    /*!
     * Add new trump card from cards of deck.
     * 
     * @param card The new trump card.
     */
    void slotNewTrumpCard( Card* card );
    
    /*!
     * Change the enabled of trump card.
     * 
     * @param enabled True/False.
     */
    void slotTrumpCardSelectableChanged( bool enabled );
    
    /*!
     * Change opponent's card with the trump card.
     * 
     * @param id The id of card in opponent's cards group.
     * @param newTrumpCard The new trump card from opponent's cards group.
     */
    void slotOpponentChangeTrumpCard( int id, Card* newTrumpCard );
    
    /*!
     * Change player's card with the trump card.
     * 
     * @param id The id of card in player's cards group.
     */
    void slotPlayerChangeTrumpCard( int id );
    
    /*!
     * Change the opponent's cards group position.
     */
    void slotOpponentCardsSizeChanged();
    
    /*!
     * Change the player's cards group position.
     */
    void slotPlayerCardsSizeChanged();
    
    /*!
     * Change the central cards group position.
     */
    void slotCentralCardsSizeChanged();
    
    /*!
     * The opponent's forty and twenty text will be unvisible.
     */
    void slotPlayerSelectedCard();
    
    /*!
     * Change the visible of close button.
     * 
     * @param visible True/False.
     */
    void slotCloseButtonVisible( bool visible );
    
    /*!
     * Change the visible of schnapsen button.
     * 
     * @param visible True/False.
     */
    void slotSchnapsenButtonVisible( bool visible );
    
    /*!
     * Change the visible of twenty button.
     * 
     * @param visible True/False
     */
    void slotTwentyButtonVisible( bool visible );
    
    /*!
     * Change the visible of forty button.
     * 
     * @param visible True/False.
     */
    void slotFortyButtonVisible( bool visible );
    
    /*!
     * The trump card will be unvisible.
     */
    void slotCloseDeck();
    
    /*!
     * Show the player's arrow.
     */
    void slotShowPlayerArrow();
    
    /*!
     * Hide the player's arrow.
     */
    void slotHidePlayerArrow();
    
    /*!
     * Show the opponent's arrow.
     */
    void slotShowOpponentArrow();
    
    /*!
     * Hide the opponent's arrow.
     */
    void slotHideOpponentArrow();
    
    /*!
     * Move the central cards to the bottom right corner.
     */
    void slotPlayerGetCentralCards();
    
    /*!
     * Move the central cards to the top right corner.
     */
    void slotOpponentGetCentralCards();
    
    /*!
     * Delete the arrived central cards.
     */
    void slotRemoveCardArrived();
    
    /*!
     * Show opponent's schnapsen text.
     */
    void slotOpponentSchnapsenButtonClicked();
    
    /*!
     * Show opponent's forty text.
     */
    void slotOpponentFortyButtonClicked();
    
    /*!
     * Show opponent's twenty text.
     */
    void slotOpponentTwentyButtonClicked();
    
public:
    /*!
     * The constructor.
     * 
     * @param parent The parent widget.
     */
    CentralWidget( QWidget* parent = 0 );
    
    /*!
     * Set the client and connect to the signalInitialize signal.
     * 
     * @param client The client.
     */
    void setClient( Client* client );
    
    /*!
     * Clear the widget. Delete all items.
     */
    void clearWidget();
    
};

#endif //centralwidget_h
