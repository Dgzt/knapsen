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
 * The Card class represents the card. It have Suit and Type and etc.
 */

#ifndef CARD_H
#define CARD_H

#include <QtCore/QString>

class Card
{
public:
    enum Suit{ Heart, Diamond, Spade, Club };
    enum Type{ Ace, Ten, King, Queen, Jack, Nine };
    
private:
    int mValue;
    
    bool mSelectable;
    
public:
    static const int CARD_NUMBERS_WITHOUT_9 = 20;
    static const int CARD_NUMBERS_WITH_9 = 24;

    /*!
     * The contrustor set the value of card.
     * 
     * @param value The value of card.
     */
    Card( int value );
    
    /*!
     * Return with the value of card.
     * 
     * @return The value of card.
     */
    int getValue() const{ return mValue; }

    /*!
     * Set card selectable. If the given value is true then
     * the card is selectable else the card isn't selectable.
     * 
     * @param selectable True/False.
     */
    void setSelectable( bool selectable ){ mSelectable = selectable; }
    
    /*!
     * Return with the state of card selectable.
     * 
     * @return True/False.
     */
    bool isSelectable() const{ return mSelectable; }
    
    /*!
     * Return with the text of card.
     * 
     * @return The text of card.
     */
    QString getCardText() const;
    
    /*!
     * Return with the points of card.
     * 
     * @return The points of carrd.
     */
    int getCardPoint() const;
    
    /*!
     * Return with the suit of card.
     * 
     * @return The suit of card.
     */
    Suit getSuit() const;
    
    /*!
     * Return with the type of card.
     * 
     * @return The type of card.
     */
    Type getType() const;

};

#endif //CARD_H
