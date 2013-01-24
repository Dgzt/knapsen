#ifndef CENTRALCARDS_H
#define CENTRALCARDS_H

#include <QtCore/QList>
#include "card.h"

class CentralCards
{
    Card** mCards;
    
public:
    CentralCards();
    ~CentralCards();
    
    int add( Card* );
    void clear();
    
    bool isEmpty();
    bool isFull();
    
    Card* getCard( int );

};

#endif //CENTRALCARDS_H
