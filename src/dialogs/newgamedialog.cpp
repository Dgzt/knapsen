#include <QtGui/QListWidget>
#include "ui_newgamewidget.h"
#include "newgamedialog.h"


NewGameDialog::NewGameDialog( QWidget* parent ): KDialog( parent )
{
	setWindowTitle( i18n( "New Game" ) );
	
	mainWidgetUi = new Ui_NewGameWidgetUi;
	
	QWidget *mainWidget = new QWidget;
	
	mainWidgetUi->setupUi( mainWidget );
	
	setMainWidget( mainWidget );
}

NewGameDialog::GameMode NewGameDialog::getGameMode() const
{
	if( mainWidgetUi->listWidget->currentRow() == 0 ){
		return NewGameDialog::LocalMode;
	}else if( mainWidgetUi->listWidget->currentRow() == 1 ){
		return NewGameDialog::ClientMode;
	}//else
	
	return NewGameDialog::ServerMode;
}

QString NewGameDialog::getClient_ServerAddress() const
{
	return mainWidgetUi->client_ipAddressLineEdit->text();
}

int NewGameDialog::getClient_ServerPort() const
{
	return mainWidgetUi->client_portSpinBox->value();
}

int NewGameDialog::getServer_ServerPort() const
{
	return mainWidgetUi->server_portSpinBox->value();
}
