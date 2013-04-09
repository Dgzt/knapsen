#ifndef NEWGAMEDIALOG_H
#define NEWGAMEDIALOG_H

#include <KDE/KPageDialog>
#include "enums.h"

class Ui_NewGameWidgetUi_local;
class Ui_NewGameWidgetUi_client;
class Ui_NewGameWidgetUi_server;

class NewGameDialog : public KPageDialog
{
    Q_OBJECT
    
    Ui_NewGameWidgetUi_local *mLocalWidgetUi;
    Ui_NewGameWidgetUi_client *mClientWidgetUi;
    Ui_NewGameWidgetUi_server *mServerWidgetUi;
    
protected:
    /*!
     * Get the actual difficulty from settings
     * and set the local dialog.
     */
    void setDifficulty();
    
protected slots:
    /*!
     * Save the difficulty when accept the dialog at local mode.
     */
    void slotAccepted();

public:
    NewGameDialog();
    
    //GameMode getGameMode() const;
    Knapsen::GameStatus getGameMode() const;
    
    //-Local mode-
    Knapsen::GameDifficulty getGameDifficulty();
    
    //-Client mode-
    
    //Return the server address
    QString getClient_ServerAddress() const;
    //Return the server port
    int getClient_ServerPort() const;
    
    //-Server mode-
    
    //Return the selected server port
    int getServer_ServerPort() const;

};

#endif //NEWGAMEDIALOG_H
