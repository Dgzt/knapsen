#ifndef WAITINGFOROPPONENTDIALOG_H
#define WAITINGFOROPPONENTDIALOG_H

#include <KDE/KDialog>

class Ui_WaitingWidgetUi;

class WaitingForOpponentDialog : public KDialog
{
    Q_OBJECT
    
    Ui_WaitingWidgetUi *waitingWidgetUi;
    
private slots:
    void waitingProgressBarTimeoutSlot();
    
public:
    WaitingForOpponentDialog( QWidget* parent = 0 );

};

#endif //WAITINGFOROPPONENTDIALOG_H
