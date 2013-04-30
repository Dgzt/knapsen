#include <KDE/KDebug>
#include <KDE/KLocalizedString>
#include "dialogs/newgamedialog.h"

#include "settings.h"

#include "ui_newgamewidget_local.h"
#include "ui_newgamewidget_client.h"
#include "ui_newgamewidget_server.h"

const char LOCAL_STRING[] = I18N_NOOP( "Local" );
const char CLIENT_STRING[] = I18N_NOOP( "Client" );
const char SERVER_STRING[] = I18N_NOOP( "Server" );

NewGameDialog::NewGameDialog() : 
    KPageDialog()
{
    setWindowTitle( i18n( "New Game" ) );
    resize( 400, 300 );
    
    //Local
    mLocalWidgetUi = new Ui_NewGameWidgetUi_local;
    
    QWidget *localWidget = new QWidget;
    mLocalWidgetUi->setupUi( localWidget );
    
    setDifficulty();
    
    KPageWidgetItem *localPageWidgetItem = new KPageWidgetItem( localWidget, ki18n( LOCAL_STRING ).toString() );
    localPageWidgetItem->setIcon( KIcon( "localmode.png" ) );
    
    addPage( localPageWidgetItem );
    
    //Client
    mClientWidgetUi = new Ui_NewGameWidgetUi_client;
    
    QWidget *clientWidget = new QWidget;
    mClientWidgetUi->setupUi( clientWidget );
    
    KPageWidgetItem *clientPageWidgetItem = new KPageWidgetItem( clientWidget, ki18n( CLIENT_STRING ).toString() );
    clientPageWidgetItem->setIcon( KIcon( "clientmode.png" ) );
    
    addPage( clientPageWidgetItem );
    
    //Server
    mServerWidgetUi = new Ui_NewGameWidgetUi_server;
    
    QWidget *serverWidget = new QWidget;
    mServerWidgetUi->setupUi( serverWidget );
    
    KPageWidgetItem *serverPageWidgetItem = new KPageWidgetItem( serverWidget, ki18n( SERVER_STRING ).toString() );
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
    if( currentPage()->name() == ki18n( LOCAL_STRING ).toString() ){
        return Knapsen::LocalMode;
    }else if( currentPage()->name() == ki18n( CLIENT_STRING ).toString() ){
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
