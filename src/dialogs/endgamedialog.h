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
 * The EndGameDialog class represents the dialog when end of the game.
 */

#ifndef ENDGAMEDIALOG_H
#define ENDGAMEDIALOG_H

#include <KDE/KDialog>

class EndGameDialog : public KDialog
{

public:
    /*!
     * The constructor.
     * 
     * @param parent The parent widget.
     * @param winnerName The name of the game winner.
     */
    EndGameDialog(QWidget* parent = 0, QString winnerName = "" );

};

#endif //ENDGAMEDIALOG_H
