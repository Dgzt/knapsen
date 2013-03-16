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
 * The Text class have a text on the white background.
 */

#ifndef TEXT_H
#define TEXT_H

#include <QtGui/QGraphicsTextItem>
#include <QtCore/QPointer>

class Animation;

class Text : public QGraphicsTextItem
{
    QPointer< Animation > mAnimation;
    
protected:
    /*!
     * Reimplemented function. Draw white rect and show text in the body.
     */
    virtual void paint( QPainter * , const QStyleOptionGraphicsItem * , QWidget * );
    
public:
    /*!
     * Setup the string and the animation time.
     * 
     * @param text The text.
     * @param animationTime The animation time.
     */
    Text( const QString& text, int animationTime = 0 );
    
    /*!
     * Clear the object.
     */
    ~Text();
    
    /*!
     * Get the animation.
     * 
     * @return The animation.
     */
    Animation* getAnimation(){ return mAnimation; }
};

#endif //TEXT_H
