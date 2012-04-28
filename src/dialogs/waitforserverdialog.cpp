#include <QtCore/QTimer>
#include <KDE/KLocalizedString>
#include <KDE/KDebug>
#include "ui_waitforserverwidget.h"
#include "waitforserverdialog.h"

WaitForServerDialog::WaitForServerDialog(): KDialog()
{
	setWindowTitle( i18n( "Wait for server" ) );
	
	connectionError = false;
	
	QWidget* mainWidget = new QWidget;
	
	waitForServerWidgetUi = new Ui_WaitForServerWidgetUi;
	
	waitForServerWidgetUi->setupUi( mainWidget );
	
	setMainWidget( mainWidget );
	
	setButtons( KDialog::Cancel );
	
	QTimer *timer = new QTimer( this );
	timer->setInterval( 10 );
	
	connect( timer, SIGNAL( timeout() ), this, SLOT( waitingProgressBarTimeoutSlot() ) );
	
	timer->start();
}

void WaitForServerDialog::waitingProgressBarTimeoutSlot()
{
	int num = waitForServerWidgetUi->waitintProgressBar->value();

    if(num == 100){
        num = 0;
    }

    num++;
    waitForServerWidgetUi->waitintProgressBar->setValue( num );
}

bool WaitForServerDialog::haveConnectionError() const
{
	return connectionError;
}

void WaitForServerDialog::slotStartGame()
{
	accept();
}
