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
 * The WaitForClientDialog class represents the dialog when wait for client.
 */

#ifndef WAITFORCLIENTDIALOG_H
#define WAITFORCLIENTDIALOG_H

#include <KDE/KDialog>

class Ui_WaitForClientWidgetUi;

class WaitForClientDialog : public KDialog
{
    Q_OBJECT
    
    Ui_WaitForClientWidgetUi *waitForClientWidgetUi;
    
    QString mAdminName;

public:
    /*!
     * The constructor.
     */
    WaitForClientDialog();
    
    /*!
     * Set the admin name, becouse won't show when the admin connected to server.
     */
    void setAdminName( QString adminName ){ mAdminName = adminName; }

public slots:
    /*!
     * Show the connected player's name on the dialog.
     * 
     * @param name The connected player's name.
     */
    void slotPlayerConnected( QString name );
    
    /*!
     * Remove the player's name on the dialog.
     * 
     * @param name The disconnected player's name.
     */
    void slotPlayerDisconnectedSlot( QString name );
    
    /*!
     * Enable the OK button.
     */
    void slotServerFull();

};

#endif //WAITFORCLIENTDIALOG_H
