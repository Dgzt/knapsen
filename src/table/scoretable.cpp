#include <QtGui/QFont>
#include <QtGui/QFontMetrics>
#include <QtGui/QPainter>
#include <KDE/KLocalizedString>
#include <KDE/KDebug>
#include "table/animation.h"
#include "table/scoretable.h"

const QString TRICKS_TEXT = i18n( "Tricks:" );
const QString SCORE_TEXT = i18n( "Score:" );

ScoreTable::ScoreTable( int animationTime )
{
    mTricksItem = new QGraphicsTextItem( this );
    mScoresItem = new QGraphicsTextItem( this );
    
    slotTricksChanged( 0 );
    slotScoresChanged( 0 );
    
    mTricksItem->setPos( 0, 0 );
    mScoresItem->setPos( 0, mTricksItem->boundingRect().height() );
    
    mAnimation = new Animation( this, animationTime ); 
}

ScoreTable::~ScoreTable()
{
    delete mAnimation;
}

QRectF ScoreTable::boundingRect() const
{
    int width = 0;
    int height = 0;
    
    if( mTricksItem->boundingRect().width() > mScoresItem->boundingRect().width() ){
        width = mTricksItem->boundingRect().width();
    }else{
        width = mScoresItem->boundingRect().width();
    }
    
    height = mTricksItem->boundingRect().height() + mScoresItem->boundingRect().height();
    
    QRectF ret( QPointF( 0, 0 ), QSize( width, height ) );
    
    return ret;
}

void ScoreTable::slotTricksChanged( int tricks )
{
    mTricksItem->setPlainText( TRICKS_TEXT+" "+QString::number( tricks ) );
    
    update();
}

void ScoreTable::slotScoresChanged( int scores )
{
    mScoresItem->setPlainText( SCORE_TEXT+" "+QString::number( scores ) );
    
    update();
}

void ScoreTable::paint(QPainter * painter, const QStyleOptionGraphicsItem *, QWidget * )
{
    painter->fillRect( boundingRect(), Qt::white );
}
