#include <KDE/KDebug>
#include "centralcards.h"

const int MAX_CENTRALCARDS = 2;

CentralCards::CentralCards()
{
    kDebug() << "Begin.";
    
    mCards = new Card*[ MAX_CENTRALCARDS ];
    for( int i = 0; i < MAX_CENTRALCARDS; ++i ){
        mCards[i] = 0;
    }
    
    kDebug() << "End.";
}

CentralCards::~CentralCards()
{
    kDebug() << "Begin.";
    
    for( int i = 0; i < MAX_CENTRALCARDS; ++i ){
        if( mCards[i] != 0 ){
            delete mCards[i];
        }
    }
    
    delete mCards;
    
    kDebug() << "End.";
}

int CentralCards::add( Card *card )
{
    kDebug() << "Begin.";
    
    //Default value only for the compiler
    int pos = -1;
    
    for( int i = 0; i < MAX_CENTRALCARDS; ++i ){
        if( mCards[i] == 0 ){
            mCards[i] = card;
            pos = i;
            break;
        }
    }
    
    kDebug() << "End.";
    return pos;
}

void CentralCards::clear()
{
    kDebug() << "Begin.";
    
    for( int i = 0; i < MAX_CENTRALCARDS; ++i ){
        if( mCards[i] != 0 ){
            delete mCards[i];
            mCards[i] = 0;
        }
    }
    
    kDebug() << "End.";
}

bool CentralCards::isEmpty()
{
    kDebug() << "Begin.";
    
    bool empty = true;
    
    for( int i = 0; i < MAX_CENTRALCARDS; ++i ){
        if( mCards[i] != 0 ){
            empty = false;
        }
    }
    
    kDebug() << "End.";
    return empty;
}

bool CentralCards::isFull()
{
    kDebug() << "Begin.";
    
    bool full = true;
    
    for( int i = 0; i < MAX_CENTRALCARDS; ++i ){
        if( mCards[i] == 0 ){
            full = false;
        }
    }
    
    kDebug() << "End.";
    return full;
}

Card* CentralCards::getCard( int pos )
{
    return mCards[pos];
}
