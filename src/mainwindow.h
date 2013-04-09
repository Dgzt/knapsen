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
    
    CentralWidget* mCWidget;
    
    Server* mServer;
    
    Client* mClient;
    
    //
    EndRoundDialog *endRoundDialog;
    EndGameDialog *endGameDialog;
    //
    
    //Initialize the data path
    void initializePaths();
    
    //Setup the actions
    void setupActions();
    
    /*!
     * Create the server and initialize the server parameters from settings.
     * 
     * @return The server.
     */
    Server* createServer();
    
    //void setGameSignals();
    
private slots:
    void slotNewGame();
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
