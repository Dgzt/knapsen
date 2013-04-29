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
 * The NewGameDialog class represents the dialog when new game.
 */

#ifndef NEWGAMEDIALOG_H
#define NEWGAMEDIALOG_H

#include <KDE/KPageDialog>
#include "enums.h"

class Ui_NewGameWidgetUi_local;
class Ui_NewGameWidgetUi_client;
class Ui_NewGameWidgetUi_server;

class NewGameDialog : public KPageDialog
{
    Q_OBJECT
    
    Ui_NewGameWidgetUi_local *mLocalWidgetUi;
    Ui_NewGameWidgetUi_client *mClientWidgetUi;
    Ui_NewGameWidgetUi_server *mServerWidgetUi;
    
protected:
    /*!
     * Get the actual difficulty from settings
     * and set the local dialog.
     */
    void setDifficulty();
    
protected slots:
    /*!
     * Save the difficulty when accept the dialog at local mode.
     */
    void slotAccepted();

public:
    /*!
     * The constructor.
     */
    NewGameDialog();
    
    /*!
     * Get the selected game mode.
     * 
     * @return The selected game mode.
     */
    Knapsen::GameStatus getGameMode() const;
    
    //-Local mode-
    
    /*!
     * Get the local game difficulty.
     * 
     * @return The difficulty.
     */
    Knapsen::GameDifficulty getGameDifficulty();
    
    //-Client mode-
    
    /*!
     * Get the server address.
     * 
     * @return The server address.
     */
    QString getClient_ServerAddress() const;
    
    /*!
     * Get the server port.
     * 
     * @return The number of port of server.
     */
    int getClient_ServerPort() const;
    
    //-Server mode-
    
    /*!
     * Get the selected server port.
     * 
     * @return The number of port of server.
     */
    int getServer_ServerPort() const;

};

#endif //NEWGAMEDIALOG_H
