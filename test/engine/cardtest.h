#ifndef TESTCARD_H
#define TESTCARD_H
#include <QtCore>

class TestCard : public QObject
{

  Q_OBJECT
  
private slots:
  void testValue();
  
};

#endif //TESTCARD_H
