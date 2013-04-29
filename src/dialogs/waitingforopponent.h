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
 * The WaitingForOpponentDialog class represents the dialog when wait for opponent.
 */

#ifndef WAITINGFOROPPONENTDIALOG_H
#define WAITINGFOROPPONENTDIALOG_H

#include <KDE/KDialog>

class Ui_WaitingWidgetUi;

class WaitingForOpponentDialog : public KDialog
{
    Q_OBJECT
    
    Ui_WaitingWidgetUi *waitingWidgetUi;
    
protected slots:
    /*!
     * Update the progress bar on the dialog.
     */
    void waitingProgressBarTimeoutSlot();
    
public:
    /*!
     * The constructor.
     * 
     * @param parent The parent widget.
     */
    WaitingForOpponentDialog( QWidget* parent = 0 );

};

#endif //WAITINGFOROPPONENTDIALOG_H
