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
 * The ScoreTable class show the tricks and scores.
 */

#ifndef scoretable_h
#define scoretable_h

#include <QtGui/QGraphicsObject>

class Animation;

class ScoreTable : public QGraphicsObject
{
    Q_OBJECT
    
    Animation* mAnimation;
    
    QGraphicsTextItem* mTricksItem;
    QGraphicsTextItem* mScoresItem;
    
public:
    /*!
     * Setup the tricks and scores items and the animation.
     * 
     * @param animationTime The animation time.
     */
    ScoreTable( int animationTime );
    
    /*!
     * Clear the object.
     */
    ~ScoreTable();
    
    /*!
     * Reimplemented function. Return with the bounding rect of score table.
     * 
     * @return The bounding rect.
     */
    virtual QRectF boundingRect() const;
    
    /*!
     * Reimplemented function. Paint the score table.
     */
    virtual void paint( QPainter * , const QStyleOptionGraphicsItem * , QWidget * );
    
    /*!
     * Get the animation of the score table.
     * 
     * @return The animation.
     */
    Animation* getAnimation(){ return mAnimation; }
    
public slots:
    /*!
     * Show the new tricks.
     * 
     * @param tricks The new tricks value.
     */
    void slotTricksChanged( int tricks );
    
    /*!
     * Show the new scores.
     * 
     * @param scores The new scores value.
     */
    void slotScoresChanged( int scores );
    
};

#endif //scoretable_h
