#include <QtCore/QTimeLine>
#include "engine/card.h"
#include "table/svgcard.h"
#include "table/cards.h"
#include "table/animation.h"

Animation::Animation( SvgCard* svgCard, Cards* endCardsGroup, Card* card )
{
    setItem( svgCard );
    mEndCardsGroup = endCardsGroup;
    mCard = card;
    
    setupTimeLine();
    
    setPosAt( 1.0, endCardsGroup->pos() + endCardsGroup->getNewCardPos() );
}

Animation::Animation( SvgCard* svgCard, QPointF endPoint, Card* card )
{
    setItem( svgCard );
    
    mCard = card;
    
    setupTimeLine();
    
    setPosAt( 1.0, endPoint );
}

void Animation::setupTimeLine()
{
    mTimeLine = new QTimeLine( 1000 );
    mTimeLine->setFrameRange( 0, 100 );
    setTimeLine( mTimeLine );
    
    connect( mTimeLine, SIGNAL( finished() ), this, SLOT( slotTimeLineTimeOut() ) );
}

void Animation::startAnimation()
{
    mTimeLine->start();
}

void Animation::slotTimeLineTimeOut()
{
    SvgCard* svgCard = (SvgCard*)item();
    if( mCard ){
        svgCard->setElementId( mCard->getCardText() );
    }
    
    emit signalCardArrived();
    emit signalCardArrived( svgCard, mEndCardsGroup );
}
