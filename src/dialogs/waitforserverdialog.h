#ifndef WAITFORSERVERDIALOG_H
#define WAITFORSERVERDIALOG_H

#include <KDE/KDialog>

class Ui_WaitingWidgetUi;

class WaitForServerDialog : public KDialog
{
	Q_OBJECT
	
	Ui_WaitingWidgetUi* waitingWidgetUi;
	
	bool connectionError;
	
private slots:
	void waitingProgressBarTimeoutSlot();
	
public:
    WaitForServerDialog();
	
	bool haveConnectionError() const;
	
};

#endif //WAITFORSERVERDIALOG_H
