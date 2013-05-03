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
 * The Animation class moves the objects on the scene.
 */

#ifndef ANIMATION_H
#define ANIMATION_H

#include <QtGui/QGraphicsItemAnimation>

class MyTextItem;
class ScoreTable;
class SvgCard;
class CardsGroup;

class Animation : public QGraphicsItemAnimation
{
    Q_OBJECT

    //
    QString mNewCardText;
    
protected:
    /*!
     * Setup the time line.
     * 
     * @param time The time of animation.
     */
    void setupTimeLine( int time );
    
protected slots:
    /*!
     * The animation end.
     */
    void slotTimeLineTimeOut();
    
public:
    /*!
     * The constructor.
     * 
     * @param graphicsItem The graphics item.
     * @param animationTime The animation time.
     */
    Animation( QGraphicsItem* graphicsItem, int animationTime );
    
    /*!
     * Set the animation time.
     * 
     * @param animationTime The time of animation.
     */
    void setAnimationTime( int animationTime );
    
    /*!
     * Set the end position.
     * 
     * @param endPos The end position.
     */
    void setEndPosition( QPointF endPos );
    
    /*!
     * Set the end position.
     * 
     * @param x The value of the x coordinate.
     * @param y The value of the y coordinate.
     */
    void setEndPosition( qreal x, qreal y ){ setEndPosition( QPointF( x, y ) ); }
    
    /*!
     * Get the end position.
     * 
     * @return The end position.
     */
    QPointF getEndPosition(){ return posAt( 1.0 ); }
    
    /*!
     * Set the new card text. 
     * The item must be a card.
     * When the animation end then change the card text to this.
     * 
     * @param cardText The new card text.
     */
    void setNewCardText( QString cardText ){ mNewCardText = cardText; }
    
    /*!
     * Start the animation.
     */
    void startAnimation();
    
signals:
    /*!
     * The animation end.
     */
    void signalAnimationEnd();
    
};

#endif //ANIMATION_H
