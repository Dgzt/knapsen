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
 * The Trump class represents the trump card.
 */

#ifndef TRUMP_H
#define TRUMP_H

#include "card.h"

class Trump
{
    Card *mCard;
    
    Card::Suit mCardSuit;
    
public:
    /*!
     * The default constructor.
     */
    Trump();
    
    /*!
     * Add new card to trump. Save the card and set the 
     * suit of card.
     * 
     * @param card The new trump card.
     */
    void addNewCard( Card* card );
    
    /*!
     * Return with the trump card.
     * 
     * @return The trump card.
     */
    Card* getCard() const{ return mCard; }
    
    /*!
     * Take the trump card. Remove the card from trump.
     * 
     * @return The trump card.
     */
    Card* takeCard();
    
    /*!
     * Return with the suit of trump card.
     * 
     * @return The suit of trump card.
     */
    Card::Suit getCardSuit() const{ return mCardSuit; }
    
    /*!
     * If the trump is empty, it hasn't got card then
     * return FALSE else TRUE.
     * 
     * @return True/False.
     */
    bool isEmpty();
    
    /*!
     * Clear the trump card. If the given value is true then
     * delete the data else not.
     * 
     * @param removeData Clear data or not.
     */
    //void clearTrumpCard( bool removeData );
    void clearTrumpCard(){ delete mCard; mCard = 0; }
    
};

#endif //TRUMP_CARD_H
