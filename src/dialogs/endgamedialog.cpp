#include "ui_endgamewidget.h"
#include "endgamedialog.h"

EndGameDialog::EndGameDialog( QWidget* parent, QString winnerName ): KDialog( parent )
{
	QWidget* mainWidget = new QWidget( this );
	
	Ui::EndGameWidgetUi *ui = new Ui::EndGameWidgetUi;
	
	ui->setupUi( mainWidget );
	
	ui->wonGameNameLabel->setText( winnerName );
	
	setMainWidget( mainWidget );
	
	setButtons( KDialog::Ok | KDialog::Cancel );
	setButtonText( KDialog::Ok, i18n( "New game" ) );
	setButtonText( KDialog::Cancel, i18n( "Close" ) );
	
	setWindowTitle( i18n( "Gane end" ) );
}
