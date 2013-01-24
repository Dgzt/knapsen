#include <QtCore/QTimer>
#include "ui_waitingwidget.h"
#include "waitingforopponent.h"


WaitingForOpponentDialog::WaitingForOpponentDialog( QWidget* parent ): KDialog( parent )
{
    setWindowTitle( i18n( "Waiting for opponent" ) );
    
    QWidget* mainWidget = new QWidget;
    
    waitingWidgetUi = new Ui_WaitingWidgetUi;
    
    waitingWidgetUi->setupUi( mainWidget );
    
    setMainWidget( mainWidget );
    
    setButtons( NULL );
    
    QTimer *timer = new QTimer( this );
    timer->setInterval( 10 );
    
    connect( timer, SIGNAL( timeout() ), this, SLOT( waitingProgressBarTimeoutSlot() ) );
    
    timer->start();
}

void WaitingForOpponentDialog::waitingProgressBarTimeoutSlot()
{
    int num = waitingWidgetUi->waitintProgressBar->value();

    if(num == 100){
        num = 0;
    }

    num++;
    waitingWidgetUi->waitintProgressBar->setValue( num );
}
