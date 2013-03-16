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
 * The SvgCard class represents the image of card.
 */

#ifndef SVGCARD_H
#define SVGCARD_H

#include <QtSvg/QGraphicsSvgItem>

class QSvgRenderer;
class Animation;

class SvgCard : public QGraphicsSvgItem
{
    Q_OBJECT
    
    //The animation of card
    Animation* mAnimation;
    
    //The selection of card
    bool mSelectable;
    
    //The size of card.
    QSizeF mSize;
    
private:
    /*!
     * The mouse press event.
     */
    virtual void mousePressEvent( QGraphicsSceneMouseEvent * );
    
    /*!
     * The mouse enter to the item.
     */
    virtual void hoverEnterEvent( QGraphicsSceneHoverEvent * );
    
    /*!
     * The mouse leave the item.
     */
    virtual void hoverLeaveEvent( QGraphicsSceneHoverEvent * );
    
public:
    /*!
     * The constructor. Setup the card with back of card.
     * 
     * @param renderer The SVG renderer.
     * @param height The height of card.
     * @param animationTime The time of animation.
     */
    SvgCard( QSvgRenderer* renderer, qreal height, int animationTime );
    
    /*!
     * Delete variables.
     */
    ~SvgCard();
    
    /*!
     * Get the animation of SVG card.
     * 
     * @return The animation of card.
     */
    Animation* getAnimation(){ return mAnimation; }
    
    /*!
     * Set the selectable of card.
     * 
     * @param selectable The selectable of card.
     */
    void setSelectable( bool selectable );
    
    /*!
     * If the card is selectable then return with true else false.
     * 
     * @return True/False.
     */
    bool isSelectable() const{ return mSelectable; }
    
    /*!
     * The text of back of card.
     * 
     * @return The text of back of card.
     */
    static QString backCardText(){ return "back"; }
    
    /*!
     * Reimplement function. Return with the bounding rect of card.
     * 
     * @return The bounding rect of card.
     */
    virtual QRectF boundingRect() const;
    
    /*!
     * Reimplement function. Paint the card to the view.
     * 
     * @param painter The painter.
     */
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
    
signals:
    /*!
     * The mouse enter to the card.
     * 
     * @param svgCard This card.
     */
    void signalMouseEnter( SvgCard* svgCard );
    
    /*!
     * The mouse leave the card.
     * 
     * @param svgCard This card.
     */
    void signalMouseLeave( SvgCard* svgCard );
    
    /*!
     * The mouse click to the card.
     * 
     * @param svgCard This card.
     */
    void signalClick( SvgCard* svgCard );
    
};

#endif //SVGCARD_H
