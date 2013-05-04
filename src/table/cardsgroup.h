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
 * The CardsGroup class control the cards.
 */

#ifndef CARDS_GROUP_H
#define CARDS_GROUP_H

#include <QtCore/QObject>
#include <QtCore/QPointF>
#include <QtCore/QPair>
#include "engine/card.h"

class SvgCard;
class Animation;

class CardsGroup : public QObject
{
    Q_OBJECT
    
    //The cards group pos
    QPointF mPos;
    
    //ID of the highlight card
    int mHighlightCardId;
    
    //Cards of group
    QList< SvgCard* >* mCards;
    
    //Show cards
    QPair< int, QPointF >* mShowCards;
    
    
protected:
    /*!
     * Get the position of the new card 
     * what will add to the cards group.
     * 
     * @return The position of the new card.
     */
    QPointF getNewCardPos();
    
    /*!
     * Set the position of the cards group.
     */
    void setCardsPosition();
    
    /*!
     * Highlight the card.
     * 
     * @param id The position of card.
     */
    void highlightCard( int id );
    
    /*!
     * Remove highlight.
     * 
     * @param id The position of card.
     */
    void removeHighlight( int id );
    
        
protected slots:
    /*!
     * Add the connect to the new card.
     */
    void slotCardAnimatedEnd();
    
    /*!
     * Highlight the card.
     * 
     * @param svgCard The card.
     */
    void slotMouseEnter( SvgCard* svgCard );
    
    /*!
     * Mova back the card.
     * 
     * @param svgCard The card
     */
    void slotMouseLeave( SvgCard* svgCard );
    
    /*!
     * The card was selected.
     * 
     * @param svgCard The card.
     */
    void slotCardClicked( SvgCard* svgCard );
    
    /*!
     * Move back the showed cards.
     */
    void slotHideCards();
    
public:
    /*!
     * The constructor.
     */
    CardsGroup();
    
    /*!
     * The descructor.
     */
    ~CardsGroup();
    
    /*!
     * Return with the bounding rect of cards group.
     * Reimplemented function.
     * 
     * @return The bounding rect of cards group.
     */
    QRectF boundingRect() const;
    
    /*!
     * Get the x coordinate at position of the cards group.
     * 
     * @return The x position of the cards group.
     */
    qreal x(){ return mPos.x(); }
    
    /*!
     * Get the y coordinate position of the cards group.
     * 
     * @return The y position of the cards group.
     */
    qreal y(){ return mPos.y(); }
    
    /*!
     * Get the position of cards group.
     * 
     * @return The position of cards group.
     */
    QPointF getPos(){ return QPointF( x(), y() ); }
    
    /*!
     * Set the position of cards group.
     * 
     * @param x The x coordinate of position.
     * @param y The y coordinate of position.
     */
    void setPos( qreal x, qreal y );
    
    /*!
     * Set the position of cards group.
     * 
     * @param point The position.
     */
    void setPos( QPointF point ){ setPos( point.x(), point.y() ); }
    
    /*!
     * Take the card from cards group.
     * 
     * @param id The id of card.
     * @return The card.
     */
    SvgCard* takeCard( int id );
    
    /*!
     * Take the first card from cards group.
     * 
     * @return The card.
     */
    SvgCard* takeFirstCard(){ return takeCard( 0 ); }
    
    /*!
     * Return with true when empty the cards group else false.
     * 
     * @return True/False.
     */
    bool isEmpty(){ return mCards->isEmpty(); }
    
    /*!
     * Clear the cards group. Remove all cards.
     */
    void clear();

public slots:
    /*!
     * Add the card to the card group.
     * 
     * @param svgCard The card.
     */
    void slotAddNewCard( SvgCard* svgCard );
    
    /*!
     * Change the enabled of card.
     * 
     * @param id The id of card.
     * @param enabled The enabled of card.
     */
    void slotSelectableChanged( int id, bool enabled );
    
    /*!
     * Select a card.
     * 
     * @param id The id of card.
     * @param card The card.
     */
    void slotSelectedCard( int id, Card* card );
    
    /*!
     * Show cards.
     * 
     * @param card0Id The id of first card.
     * @param card0 The first card.
     * @param card1Id The id of second card.
     * @param card1 The second card.
     */
    void slotShowCards( int card0Id, Card card0, int card1Id, Card card1 );
    
signals:
    /*!
     * The size of cards group is changed.
     */
    void signalSizeChanged();
    
    /*!
     * The card is arrived.
     */
    void signalCardArrived();
    
    /*!
     * Selected a card.
     * 
     * @param svgCard The selected card.
     */
    void signalSelectedCard( SvgCard* svgCard );
    
    /*!
     * Selected a card.
     * 
     * @param id The id of selected card.
     * @param durationTime The duration time of selected card.
     */
    void signalSelectedCardId( int id, int durationTime );
    
    /*!
     * Hide showed cards.
     */
    void signalHideCards();
    
};

#endif //CARDS_GROUP_H
