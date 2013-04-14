#include <QtCore/QTimer>
#include <KDE/KLocalizedString>
#include <KDE/KDebug>
#include "ui_waitingwidget.h"
#include "waitforserverdialog.h"

WaitForServerDialog::WaitForServerDialog(): KDialog()
{
    setWindowTitle( i18n( "Waiting for server" ) );
    
    connectionError = false;
    
    QWidget* mainWidget = new QWidget;
    
    waitingWidgetUi = new Ui_WaitingWidgetUi;
    
    waitingWidgetUi->setupUi( mainWidget );
    
    setMainWidget( mainWidget );
    
    setButtons( KDialog::Cancel );
    
    QTimer *timer = new QTimer( this );
    timer->setInterval( 10 );
    
    connect( timer, SIGNAL( timeout() ), this, SLOT( waitingProgressBarTimeoutSlot() ) );
    
    timer->start();
}

void WaitForServerDialog::waitingProgressBarTimeoutSlot()
{
    int num = waitingWidgetUi->waitintProgressBar->value();

    if(num == 100){
        num = 0;
    }

    num++;
    waitingWidgetUi->waitintProgressBar->setValue( num );
}

bool WaitForServerDialog::haveConnectionError() const
{
    return connectionError;
}
