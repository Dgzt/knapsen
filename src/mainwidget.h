#ifndef mainwindow_h
#define mainwindow_h

#include <KXmlGuiWindow>
#include "enums.h"
#include "engine/client.h"

class CentralWidget;
class KAction;

class Server;
//class Client;

class EndRoundDialog;
class EndGameDialog;
//

class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT
    
    Knapsen::GameStatus mGameStatus;
    
    KAction* mCloseGameAction;
    
    CentralWidget* cWidget;
    
    Server* server;
    
    Client* client;
    
    //
    EndRoundDialog *endRoundDialog;
    EndGameDialog *endGameDialog;
    //
    
    //Initialize the data path
    void initializePaths();
    
    //Setup the actions
    void setupActions();
    
    void setServerConfig();
    
    void setGameSignals();
    
private slots:
    void newGameSlot();
    void closeGameSlot();
    void optionsPreferences();
    
    void slotStartGame();
    
    void slotServerEmpty();
    
    void slotSocketError( QAbstractSocket::SocketError );
    void slotGameError( Client::GameErrorType );
    
    void slotEndRound( QString, int );
    void slotEndRoundExec();
    
    void slotEndGame( QString );
    void slotEndGameExec();
    
public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

};

#endif // mainwindow_h
