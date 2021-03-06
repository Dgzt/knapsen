#include <KDE/KLocalizedString>
#include <KDE/KPushButton>
#include <KDE/KDebug>
#include "ui_waitforclientwidget.h"
#include "waitforclientdialog.h"

WaitForClientDialog::WaitForClientDialog(): KDialog()
{
    setWindowTitle( i18n( "Waiting for client" ) );
    
    QWidget* mainWidget = new QWidget;
    
    waitForClientWidgetUi = new Ui_WaitForClientWidgetUi;
    waitForClientWidgetUi->setupUi( mainWidget );
    
    setMainWidget( mainWidget );
    
    button( KDialog::Ok )->setEnabled( false );
}

void WaitForClientDialog::slotPlayerConnected( QString name )
{
    if( name != mAdminName ){
        waitForClientWidgetUi->opponentNameLineEdit->setText( name );
        //button( KDialog::Ok )->setEnabled( true );
    }
}

void WaitForClientDialog::slotPlayerDisconnectedSlot( QString name )
{
    Q_UNUSED( name );
    
    waitForClientWidgetUi->opponentNameLineEdit->clear();
    button( KDialog::Ok )->setEnabled( false );
}

void WaitForClientDialog::slotServerFull()
{
    button( KDialog::Ok )->setEnabled( true );
}
