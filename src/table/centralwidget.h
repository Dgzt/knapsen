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

#include <QDeclarativeView>
#include "enums.h"

class CentralWidget : public QDeclarativeView
{
    Q_OBJECT

public:
    /*!
     * The constructor.
     * 
     * @param parent The parent widget.
     */
    CentralWidget( QWidget* parent = 0 );
    
    
public slots:
    void slotInitialize( QString, QString, Knapsen::TypeOfCards );
     
};

#endif //centralwidget_h
