/*#include <KDE/KDebug>
#include <QtCore/QTimeLine>
#include "table/mytextitem.h"
#include "engine/card.h"
#include "table/svgcard.h"
#include "table/cardsgroup.h"
#include "table/animation.h"

Animation::Animation( MyTextItem* textItem, QPointF endPoint )
{
    setItem( textItem );
    
    setupTimeLine();
    
    setPosAt( 1.0, endPoint );
    
    mCard = 0;
}

Animation::Animation( SvgCard* svgCard, QPointF endPoint, Card* card )
{
    setItem( svgCard );
        
    setupTimeLine();
    
    setPosAt( 1.0, endPoint );
    
    mCard = card;
}

Animation::Animation( CardsGroup* cardsGroup, QPointF endPos )
{
    setItem( cardsGroup );
    
    setupTimeLine();
    
    setPosAt( 1.0, endPos );
    
    mCard = 0;
}

void Animation::setupTimeLine()
{
    mTimeLine = new QTimeLine( 2000 );
    mTimeLine->setFrameRange( 0, 100 );
    setTimeLine( mTimeLine );
    
    connect( mTimeLine, SIGNAL( finished() ), this, SLOT( slotTimeLineTimeOut() ) );
}

void Animation::slotStartAnimation()
{
    mTimeLine->start();
}

void Animation::slotTimeLineTimeOut()
{
    SvgCard* svgCard = (SvgCard*)item();
    
    if( mCard ){
        svgCard->setElementId( mCard->getCardText() );
    }
    
    emit signalAnimationEnd();
    
    emit signalCardArrived();
    emit signalCardArrived( svgCard );
    
    //disconnect( this, 0, 0, 0 );
    //deleteLater();
}*/
#include <KDE/KDebug>
#include <QtCore/QTimeLine>
#include <engine/card.h>
#include <table/mytextitem.h>
#include "table/svgcard.h"
#include "table/cardsgroup.h"
#include "table/animation.h"

const int ANIMATION_TIME = 2000;

Animation::Animation( MyTextItem* gItem )
{
    setItem( gItem );
    
    setupTimeLine();
    
    mCard = 0;
}

Animation::Animation( SvgCard* svgCard )
{
    setItem( svgCard );
    
    setupTimeLine();
    
    mCard = 0;
}

Animation::Animation( CardsGroup* cardsGroup )
{
    setItem( cardsGroup );
    
    setupTimeLine();
    
    mCard = 0;
}

Animation::~Animation()
{
    kDebug() << "---------------------------------------------------";
}

void Animation::setupTimeLine()
{
    QTimeLine* timeLine = new QTimeLine( ANIMATION_TIME );
    timeLine->setFrameRange( 0, 200 );
    setTimeLine( timeLine );
    
    connect( timeLine, SIGNAL( finished() ), this, SLOT( slotTimeLineTimeOut() ) );
}

void Animation::setEndPosition( QPointF endPos )
{
    //The start position is the actual position
    setPosAt( 0.0, item()->pos() );
    
    setPosAt( 1.0, endPos );
}

void Animation::setCard( Card* card )
{
    mCard = card;
}

void Animation::startAnimation()
{
    timeLine()->start();
}

void Animation::slotTimeLineTimeOut()
{
    if( mCard ){
        ( (SvgCard*)item() )->setElementId( mCard->getCardText() );
    }
    
    emit signalAnimationEnd();
}
