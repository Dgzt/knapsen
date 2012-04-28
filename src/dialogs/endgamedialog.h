#ifndef ENDGAMEDIALOG_H
#define ENDGAMEDIALOG_H

#include <KDE/KDialog>

class EndGameDialog : public KDialog
{

public:
    EndGameDialog(QWidget* parent = 0, QString winnerName = "" );
	
};

#endif //ENDGAMEDIALOG_H
