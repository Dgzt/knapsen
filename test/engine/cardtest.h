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
 * The TestCard class test the Card class.
 */

#ifndef TESTCARD_H
#define TESTCARD_H
#include <QtCore>

class TestCard : public QObject
{

  Q_OBJECT
  
private slots:
  /*!
   * Test the 'getValue' function.
   */
  void testValue();
  
  /*!
   * Test the 'setSelectable' and 'isSelectable' functions.
   */
  void testSelectable();
  
  /*!
   * Test the 'getCardText' function.
   */
  void testCardText();
  
  /*!
   * Test the 'getCardPoint' function.
   */
  void testCardPoint();
  
  /*!
   * Test the 'getCardSuit' function.
   */
  void testCardSuit();
  
  /*!
   * Test the 'getCardType' function.
   */
  void testCardType();
  
};

#endif //TESTCARD_H
