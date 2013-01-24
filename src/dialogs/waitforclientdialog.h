#ifndef WAITFORCLIENTDIALOG_H
#define WAITFORCLIENTDIALOG_H

#include <KDE/KDialog>

class Ui_WaitForClientWidgetUi;

class WaitForClientDialog : public KDialog
{
    Q_OBJECT
    
    Ui_WaitForClientWidgetUi *waitForClientWidgetUi;
    
    QString mAdminName;

public:
    WaitForClientDialog();
    
    //Set admin name, becouse not show when player connected to server
    void setAdminName( QString adminName ){ mAdminName = adminName; }

public slots:
    void slotPlayerConnected( QString );
    void slotPlayerDisconnectedSlot( QString );
    void slotServerFull();

};

#endif //WAITFORCLIENTDIALOG_H
