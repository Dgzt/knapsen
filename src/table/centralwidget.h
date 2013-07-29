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
#include "engine/card.h"
#include "enums.h"

//class Card;

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
    
    void clear();
    
public slots:
    void slotInitialize( QString, QString, Knapsen::TypeOfCards );
    
    void slotStartGame();
    
    void slotNewRound();
    
    void slotNewPlayerCard( bool lastCard, Card* card );
    
    void slotNewPlayerCardTrumpCard();
    
    void slotNewOpponentCard( bool lastCard );
    
    void slotNewOpponentCardTrumpCard();
    
    void slotNewTrumpCard( Card* trumpCard );
    
    void slotPlayerCardSelectableChanged( int id, bool enabled );
    
    void slotOpponentSelectedCard( int id, Card* card );
    
    void slotOpponentGetCentralCards();
    
    void slotPlayerGetCentralCards();
    
    void slotCloseButtonVisibleChanged( bool visible );
    
    void slotCloseDeck();
    
    void slotFortyButtonVisibleChanged( bool visible );
     
    void slotTwentyButtonVisibleChanged( bool visible );
    
    void slotSchnapsenButtonVisibleChanged( bool visible );
    
    void slotOpponentTricksChanged( int tricks );
    
    void slotOpponentScoresChanged( int scores );
    
    void slotPlayerTricksChanged( int tricks );
    
    void slotPlayerScoresChanged( int scores );
    
    void slotTrumpCardSelectableChanged( bool selectable );
    
    void slotPlayerChangeTrumpCard( int id );
    
    void slotOpponentChangeTrumpCard( int id, Card* newTrumpCard );
    
    void slotShowOpponentCards( int card1Id, Card* card1, int card2Id, Card* card2 );
    
    void slotOpponentTwentyButtonClicked();
    
    void slotOpponentFortyButtonClicked();
    
    void slotOpponentSchnapsenButtonClicked();
    
    void slotOpponentInAction();
    
    void slotPlayerInAction();
    
signals:
    void signalAnimationEnd();
    void signalSelectedCard( int id, int delay );
    void signalCloseButtonClicked();
    void signalFortyButtonClicked();
    void signalTwentyButtonClicked();
    void signalSchnapsenButtonClicked();
    void signalTrumpCardClicked();

};

#endif //centralwidget_h
