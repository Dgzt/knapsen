#include <QtTest>
#include "../../src/engine/card.h"
#include "cardtest.h"

void TestCard::testValue(){
  int cardValue = 1;
  
  Card *testCard = new Card( cardValue );
  QCOMPARE( testCard->getValue(), cardValue );
}

void TestCard::testSelectable(){
  int cardValue = 1;
  
  Card *testCard = new Card( cardValue );
  testCard->setSelectable( true );
  
  QVERIFY( testCard->isSelectable() );
  
  testCard->setSelectable( false );
  
  QVERIFY( !testCard->isSelectable() );
}

void TestCard::testCardText(){
  int cardValue = 5;
  QString cardText("1_diamond");
  
  Card *testCard = new Card( cardValue );

  QCOMPARE( testCard->getCardText(), cardText );
}

void TestCard::testCardPoint()
{
  int cardValue = 3;
  int cardPoint = 2;
  
  Card *testCard = new Card( cardValue );
  
  QCOMPARE( testCard->getCardPoint(), cardPoint );
}

void TestCard::testCardSuit()
{
  int cardValue = 11;
  Card::Suit cardSuit = Card::Spade;
  
  Card *testCard = new Card( cardValue );
  QCOMPARE( testCard->getSuit(), cardSuit );
}

void TestCard::testCardType()
{
  int cardValue = 21;
  Card::Type cardType = Card::Nine;
  
  Card *testCard = new Card( cardValue );
  QCOMPARE( testCard->getType(), cardType );
}

QTEST_MAIN( TestCard )
