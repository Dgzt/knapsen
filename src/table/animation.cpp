#include <KDE/KDebug>
#include <QtCore/QTimeLine>
#include <engine/card.h>
#include "table/mytextitem.h"
#include "table/scoretable.h"
#include "table/svgcard.h"
#include "table/cardsgroup.h"
#include "table/animation.h"

Animation::Animation( QGraphicsItem* graphicsItem, int animationTime ) : 
    mNewCardText( "" )
{
    setItem( graphicsItem );
    
    setupTimeLine( animationTime );
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

void Animation::setAnimationTime( int animationTime )
{
    timeLine()->setDuration( animationTime );
}

void Animation::setEndPosition( QPointF endPos )
{
    //The start position is the actual position
    setPosAt( 0.0, item()->pos() );
    
    setPosAt( 1.0, endPos );
}

void Animation::startAnimation()
{
    timeLine()->start();
}

void Animation::slotTimeLineTimeOut()
{
    if( !mNewCardText.isEmpty() ){
        ( (SvgCard*)item() )->setElementId( mNewCardText );
        mNewCardText.clear();
    }
    
    emit signalAnimationEnd();
}
