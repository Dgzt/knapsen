#include <KDE/KDebug>
#include <QtCore/QTimeLine>
#include <engine/card.h>
#include "table/mytextitem.h"
#include "table/scoretable.h"
#include "table/svgcard.h"
#include "table/cardsgroup.h"
#include "table/animation.h"
/*
const int ANIMATION_TIME = 200;

const int CARD_ANIMATION_TIME = 500; //200
const int MYTEXTITEM_ANIMATION_TIME = 1000; //1000
const int SCORE_TABLE_ANIMATION_TIME = 1000; //100
*/

/*Animation::Animation( MyTextItem* gItem )
{
    setItem( gItem );
    
    setupTimeLine( MYTEXTITEM_ANIMATION_TIME );
    
    mCard = 0;
}

Animation::Animation( ScoreTable* scoreTable )
{
    setItem( scoreTable );
    
    setupTimeLine( SCORE_TABLE_ANIMATION_TIME );
    
    mCard = 0;
}

Animation::Animation( SvgCard* svgCard )
{
    setItem( svgCard );
    
    setupTimeLine( CARD_ANIMATION_TIME );
    
    mCard = 0;
}*/

Animation::Animation( QGraphicsItem* graphicsItem, int animationTime ) : 
    mNewCardText( "" )
{
    setItem( graphicsItem );
    
    setupTimeLine( animationTime );
    
    //mCard = 0;
}

Animation::~Animation()
{
    kDebug() << "Deleting.";
}

void Animation::setupTimeLine( int time )
{
    QTimeLine* timeLine = new QTimeLine( time );
    timeLine->setFrameRange( 0, time );
    setTimeLine( timeLine );
    
    connect( timeLine, SIGNAL( finished() ), this, SLOT( slotTimeLineTimeOut() ) );
}

void Animation::setEndPosition( QPointF endPos )
{
    //The start position is the actual position
    setPosAt( 0.0, item()->pos() );
    
    setPosAt( 1.0, endPos );
}

/*void Animation::setCard( Card* card )
{
    mCard = card;
}*/

void Animation::startAnimation()
{
    timeLine()->start();
}

void Animation::slotTimeLineTimeOut()
{
    /*if( mCard ){
        ( (SvgCard*)item() )->setElementId( mCard->getCardText() );
    }*/
    if( !mNewCardText.isEmpty() ){
        ( (SvgCard*)item() )->setElementId( mNewCardText );
        mNewCardText.clear();
    }
    
    emit signalAnimationEnd();
}
