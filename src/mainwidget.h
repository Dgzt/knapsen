#ifndef mainwindow_h
#define mainwindow_h


#include <KXmlGuiWindow>

class CentralWidget;
class KAction;

class Server;
class Client;
class Bot;

class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT
    
    KAction* closeGameAction;
    
    CentralWidget* cWidget;
	
	Server* server;
	
	Client* client;
	
	Bot* bot;
	
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
	
	void slotOpponentDisconnected();
	
	void slotEndRound( QString, int );
	
	/*void endRoundSlot( QString, int );
	void endGameSlot( QString );*/
	
	
public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

};

#endif // mainwindow_h
