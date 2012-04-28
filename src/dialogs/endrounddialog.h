#ifndef ENDROUNDDIALOG_H
#define ENDROUNDDIALOG_H

#include <KDE/KDialog>

class EndRoundDialog : public KDialog
{

public:
    EndRoundDialog( QWidget* parent = 0, QString winnerName = "", int winnerAcquiredScores = 0 );
	
};

#endif //ENDROUNDDIALOG_H
