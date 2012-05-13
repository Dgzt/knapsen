#ifndef WAITINGFOROPPONENT_H
#define WAITINGFOROPPONENT_H

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
	
public slots:
	void slotNewRound();
	
};

#endif //WAITINGFOROPPONENT_H
