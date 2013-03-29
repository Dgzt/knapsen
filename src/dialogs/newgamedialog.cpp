#include <KDE/KDebug>
#include <KDE/KLocalizedString>
#include "dialogs/newgamedialog.h"

#include "settings.h"

#include "ui_newgamewidget_local.h"
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
    mLocalWidgetUi = new Ui_NewGameWidgetUi_local;
    
    QWidget *localWidget = new QWidget;
    mLocalWidgetUi->setupUi( localWidget );
    
    //
    setDifficulty();
    //
    
    KPageWidgetItem *localPageWidgetItem = new KPageWidgetItem( localWidget, LOCAL_STRING );
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
    
    connect( this, SIGNAL( accepted() ), this, SLOT( slotAccepted() ) );
}

void NewGameDialog::setDifficulty()
{   
    switch( Settings::difficulty() ){
        case 0 : mLocalWidgetUi->easyRadioButton->setChecked( true ); break;
        case 1 : mLocalWidgetUi->normalRadioButton->setChecked( true ); break;
        case 2 : mLocalWidgetUi->hardRadioButton->setChecked( true ); break;
    }
}

void NewGameDialog::slotAccepted()
{
    if( getGameMode() == Knapsen::LocalMode ){
        
        if( mLocalWidgetUi->easyRadioButton->isChecked() ){
            Settings::setDifficulty( 0 );
        }
        
        if( mLocalWidgetUi->normalRadioButton->isChecked() ){
            Settings::setDifficulty( 1 );
        }
        
        if( mLocalWidgetUi->hardRadioButton->isChecked() ){
            Settings::setDifficulty( 2 );
        }
        
        Settings::self()->writeConfig();
    }
}

Knapsen::GameStatus NewGameDialog::getGameMode() const
{
    if( currentPage()->name() == LOCAL_STRING ){
        return Knapsen::LocalMode;
    }else if( currentPage()->name() == CLIENT_STRING ){
        return Knapsen::ClientMode;
    }//else
    return Knapsen::ServerMode;
}

Knapsen::GameDifficulty NewGameDialog::getGameDifficulty()
{
    if( mLocalWidgetUi->easyRadioButton->isChecked() ){
        return Knapsen::Easy;
    }else if( mLocalWidgetUi->normalRadioButton->isChecked() ){
        return Knapsen::Medium;
    } //else mLocalWidgetUi->hardRadioButton->isChecked()
    
    return Knapsen::Hard;
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
