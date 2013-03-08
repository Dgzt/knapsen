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
 * The Deck class represents the deck of cards.
 */

#ifndef DECK_H
#define DECK_H

#include <QtCore/QString>
#include <QtCore/QList>
#include "enums.h"

class Card;

class Deck
{
    int mDeckSize;
    
    QList<Card*> mCards;
    
public:
    /*!
     * The constructor.
     * 
     * @param deckSize The size of deck.
     */
    Deck( int deckSize );
    
    /*!
     * Generate the deck of cards with random cards.
     */
    void build();
    
    /*!
     * Return with the current deck of size.
     * 
     * @return The current deck of size.
     */
    int getDeckSize() const{ return mCards.size(); }
    
    /*!
     * Take the last card. This function
     * remove the card from the deck of cards.
     * 
     * @return The last card in deck of cards.
     */
    Card* takeCard(){ return mCards.takeLast(); }
    
};

#endif //DECK_H
