#ifndef WAITFORSERVERDIALOG_H
#define WAITFORSERVERDIALOG_H

#include <KDE/KDialog>

class Ui_WaitForServerWidgetUi;

class WaitForServerDialog : public KDialog
{
	Q_OBJECT
	
	Ui_WaitForServerWidgetUi* waitForServerWidgetUi;
	
	bool connectionError;
	
private slots:
	void waitingProgressBarTimeoutSlot();
	
public:
    WaitForServerDialog();
	
	bool haveConnectionError() const;
	
public slots:
	void slotStartGame();
	
};

#endif //WAITFORSERVERDIALOG_H
