#include <KDE/KDebug>
#include "card.h"

static const QString CARDS_NAME[ Card::CARD_NUMBERS_WITH_9 ] = {
    "1_heart", "king_heart", "queen_heart", "jack_heart", "10_heart", //heart
    "1_diamond", "king_diamond", "queen_diamond", "jack_diamond", "10_diamond", //diamond
    "1_spade", "king_spade", "queen_spade", "jack_spade", "10_spade", //spade
    "1_club", "king_club", "queen_club", "jack_club" , "10_club", //club
    "9_heart",  "9_diamond",  "9_spade", "9_club" //nines
};

Card::Card( int value ) : 
    mValue( value ),
    mSelectable( false )
{}

QString Card::getCardText(/* Knapsen::TypeOfCards typeOfCards */) const
{	
    return CARDS_NAME[ mValue ];
}

int Card::getCardPoint() const
{
    int ret = 0;

    switch( getType() ){
        case Card::Ace : ret = 11; break;
        case Card::King : ret = 4; break;
        case Card::Queen : ret = 3; break;
        case Card::Jack : ret  = 2; break;
        case Card::Ten : ret = 10; break;
        case Card::Nine : ret = 0; break;
        default : kDebug() << "ERROR! Fail card type!";
    }
    
    return ret;
}

Card::Suit Card::getSuit() const
{
    Suit ret;
    
    if( mValue < 5 || mValue == 20 ){
        ret = Card::Heart;
    }else if( (5 <= mValue && mValue < 10) || mValue == 21 ){
        ret = Card::Diamond;
    }else if( (10 <= mValue && mValue < 15) || mValue == 22 ){
        ret = Card::Spade;
    }else{ // (15 <= mValue && mValue < 20) || mValue == 23
        ret = Card::Club;
    }
    
    return ret;
}

Card::Type Card::getType() const
{
    Type ret;
    
    if( mValue == 0 || mValue == 5 || mValue == 10 || mValue == 15 ){ //Ace
        ret = Card::Ace;
    }else if( mValue == 1 || mValue == 6 || mValue == 11 || mValue == 16 ){ //King
        ret = Card::King;
    }else if ( mValue == 2 || mValue == 7 || mValue == 12 || mValue == 17 ){ //Queen
        ret = Card::Queen;
    }else if( mValue == 3 || mValue == 8 || mValue == 13 || mValue == 18 ){ // Jack
        ret = Card::Jack;
    }else if( mValue == 4 || mValue == 9 || mValue == 14 || mValue == 19 ){ //10
        ret = Card::Ten;
    }else{ // mValue == 20 || mValue == 21 || mValue == 22 || mValue == 23 // 9
        ret = Card::Nine;
    }
    
    return ret;
}
