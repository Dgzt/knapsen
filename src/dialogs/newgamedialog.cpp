#include <KDE/KLocalizedString>
#include "dialogs/newgamedialog.h"

#include "ui_newgamewidget_client.h"
#include "ui_newgamewidget_server.h"

const QString LOCAL_STRING = i18n( "Local" );
const QString CLIENT_STRING = i18n( "Client" );
const QString SERVER_STRING = i18n( "Server" );

NewGameDialog::NewGameDialog( QWidget* parent, Qt::WFlags flags ): KPageDialog( parent, flags )
{
	setWindowTitle( i18n( "New game dialog" ) );
	resize( 400, 300 );
	
	//Local
	KPageWidgetItem *localPageWidgetItem = new KPageWidgetItem( new QWidget, LOCAL_STRING );
	localPageWidgetItem->setIcon( KIcon( "localmode.png" ) );
	
	addPage( localPageWidgetItem );

	//Client
	mClientWidgetUi = new Ui_NewGameWidgetUi_client;
	
	QWidget *clientWidget = new QWidget;
	mClientWidgetUi->setupUi( clientWidget );
	
	KPageWidgetItem *clientPageWidgetItem = new KPageWidgetItem( clientWidget, CLIENT_STRING );
	clientPageWidgetItem->setIcon( KIcon( "clientmode.png" ) );
	
	addPage( clientPageWidgetItem );
	
	//Server
	mServerWidgetUi = new Ui_NewGameWidgetUi_server;
	
	QWidget *serverWidget = new QWidget;
	mServerWidgetUi->setupUi( serverWidget );
	
	KPageWidgetItem *serverPageWidgetItem = new KPageWidgetItem( serverWidget, SERVER_STRING );
	serverPageWidgetItem->setIcon( KIcon( "servermode.png" ) );
	
	addPage( serverPageWidgetItem );
}

NewGameDialog::GameMode NewGameDialog::getGameMode() const
{
	if( currentPage()->name() == LOCAL_STRING ){
		return LocalMode;
	}else if( currentPage()->name() == CLIENT_STRING ){
		return ClientMode;
	}//else
	return ServerMode;
}

QString NewGameDialog::getClient_ServerAddress() const
{
	return mClientWidgetUi->client_ipAddressLineEdit->text();
}

int NewGameDialog::getClient_ServerPort() const
{
	return mClientWidgetUi->client_portSpinBox->value();
}

int NewGameDialog::getServer_ServerPort() const
{
	return mServerWidgetUi->server_portSpinBox->value();
}
