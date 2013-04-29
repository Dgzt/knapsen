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
 * The Bot class represents the bot.
 */

#ifndef BOT_H
#define BOT_H

#include "enums.h"
#include "client.h"

class Bot : public Client
{
    Q_OBJECT

    //The Bot's difficulty
    Knapsen::GameDifficulty mDifficulty;

    //Is the deck is closed
    bool mDeckIsClosed;

    QPair< Card::Suit, bool > pairOfKingWasInCentralCards[4];
    QPair< Card::Suit, bool > pairOfQueenWasInCentralCards[4];
    
protected:
    /*!
     * Check the actual card was at the central cards.
     * 
     * @param type The type of card.
     * @param suit The suit of card.
     * @return True/False.
     */
    bool isPairOfCardWasInCentral( Card::Type type, Card::Suit suit );
    
    /*!
     * Select card in easy mod.
     */
    void easy();
    
    /*!
     * Select card in medium mod.
     */
    void medium();
    
    /*!
     * Select card id hard mod.
     */
    void hard();
    
    /*!
     * Get a random card ID.
     * 
     * @return Selected ID of card.
     */
    int getRandomCardId();
    
    /*!
     * Try change trump card.
     */
    void tryChangeTrumpCard();
    
    /*!
     * Try meld marriage. Firstly forty, after twenty.
     */
    bool tryMeldMarriage();
    
    /*!
     * Try select a card which type not equal with type of trump.
     * 
     * @param type The type of card.
     * @return True/False, whether the selecting was successful.
     */
    bool trySelectNotEqualTrump( Card::Type type );
    
    /*!
     * Try select a card which pair wasn't in the central cards.
     * 
     * @param type The type of card. Only King or Queen.
     * @return True/False, whether the selecting was successful.
     */
    bool trySelectPairWasNotInCentral( Card::Type type ); //Only King or Queen
    
    /*!
     * Try select a card which type equal with type of trump.
     * 
     * @param type The type of card.
     * @return True/False, whether the selecting was successful.
     */
    bool trySelectEqualTrump( Card::Type type );
    
    /*!
     * Try select a card.
     * 
     * @param type The type of card.
     * @return True/False, whether the selecting was successful.
     */
    bool trySelect( Card::Type type );
    
    /*!
     * Try select a card which type equal with type of central card and
     * have minimum points.
     * 
     * @return True/False, whether the selecting was successful.
     */
    bool trySelectEqualCentralMinPoints();
    
    /*!
     * Try select a card which type equal with type of trump card and
     * have minimum points.
     * 
     * @return True/False, whether the selecting was successful.
     */
    bool trySelectEqualTrumpMinPoints();
    
protected slots:
    /*!
     * Select card.
     */
    void slotSelectCard();
    
    /*!
     * New round.
     */
    void slotNewRound();
    
    /*!
     * The deck was closed.
     */
    void slotCloseDeck(){ mDeckIsClosed = true; }
    
    /*!
     * A new card in the central cards.
     * 
     * @param card The new card.
     */
    void slotNewCentralCard( const Card* card );
    
    /*!
     * Start next round.
     * 
     * @param name The round winner player's name.
     * @param scores The round winner player's scores.
     */
    void slotEndRound( QString name, int scores );
    
    /*!
     * Start next game.
     * 
     * @param name The game winner player's name.
     */
    void slotendGame( QString name );
    
public:
    /*!
     * The default constructor.
     * 
     * @param name The bot's name.
     * @param difficulty The bot's difficulty.
     */
    Bot( QString name, Knapsen::GameDifficulty difficulty );

};

#endif //BOT_H
