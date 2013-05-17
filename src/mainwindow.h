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
 * The MainWindow class is the main window.
 */

#ifndef mainwindow_h
#define mainwindow_h

#include <KXmlGuiWindow>
#include "enums.h"
#include "engine/client.h"

class CentralWidget;
class KAction;
class Server;
class EndRoundDialog;
class EndGameDialog;

class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT
    
    Knapsen::GameStatus mGameStatus;
    
    KAction* mCloseGameAction;
    
    CentralWidget* mCWidget;
    
    Server* mServer;
    
    Client* mClient;
    
    //
    EndRoundDialog *endRoundDialog;
    EndGameDialog *endGameDialog;
    //
    
protected:
    /*!
     * Check and initialize the data's path.
     */
    void initializePaths();
    
    /*!
     * Setup the actions.
     */
    void setupActions();
    
    //
    void setupConnects();
    //
    
    /*!
     * Create the server and initialize the server parameters from settings.
     * 
     * @return The server.
     */
    Server* createServer();
    
protected slots:
    /*!
     * Show the new game dialog.
     */
    void slotNewGame();
    
    /*!
     * Close the actual game.
     */
    void closeGameSlot();
    
    /*!
     * Show the settings dialog.
     */
    void optionsPreferences();
    
    /*!
     * Start the game.
     */
    void slotStartGame();
    
    /*!
     * Delete the server.
     */
    void slotServerEmpty();
    
    /*!
     * Show the socket error message.
     * 
     * @param socketError The socket error type.
     */
    void slotSocketError( QAbstractSocket::SocketError socketError );
    
    /*!
     * Show the game error message.
     * 
     * @param gameErrorType The game error type.
     */
    void slotGameError( Client::GameErrorType gameErrorType );
    
    /*!
     * Setup the round winner dialog.
     * 
     * @param name The round winner's name.
     * @param scores The round winner's new scores.
     */
    void slotEndRound( QString name, int scores );
    
    /*!
     * Show the round winner dialog.
     */
    void slotEndRoundExec();
    
    /*!
     * Setup the game end dialog.
     * 
     * @param name The game winner's name.
     */
    void slotEndGame( QString name );
    
    /*!
     * Show the game end dialog.
     */
    void slotEndGameExec();
    
public:
    /*!
     * The constructor.
     * 
     * @param parent The parent widget.
     */
    MainWindow(QWidget* parent = 0);
    
    /*!
     * The destructor.
     */
    ~MainWindow();

};

#endif // mainwindow_h
