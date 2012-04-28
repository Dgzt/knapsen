#ifndef BOT_H
#define BOT_H

#include "client.h"

class Bot : public Client
{
	Q_OBJECT
	
private slots:
	void slotOpponentDisconnected();
	
	void slotInAction();
	
	void slotSelectCard();
	
public:
    Bot( QObject* parent = 0 );
	
};

#endif //BOT_H
