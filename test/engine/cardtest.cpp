#include <QtTest>
#include "../src/engine/card.h"
#include "cardtest.h"

void TestCard::testValue(){
  Card *testCard = new Card(1);
  QCOMPARE( testCard->getValue(), 1 );
}

QTEST_MAIN( TestCard )
