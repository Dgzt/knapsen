#ifndef NEWGAMEDIALOG_H
#define NEWGAMEDIALOG_H

#include <KDE/KDialog>

class Ui_NewGameWidgetUi;

class NewGameDialog : public KDialog
{
	
	Ui_NewGameWidgetUi *mainWidgetUi;
	
public:
	enum GameMode { LocalMode, ClientMode, ServerMode };
	
    NewGameDialog( QWidget* parent = 0 );
	
	//Return the selected game mode
	GameMode getGameMode() const;
	
	//-Client mode-
	//Return the server address
	QString getClient_ServerAddress() const;
	int getClient_ServerPort() const;
	
	//-Server mode-
	//Return the selected server port - Server mode
	int getServer_ServerPort() const;
	
};

#endif //NEWGAMEDIALOG_H
