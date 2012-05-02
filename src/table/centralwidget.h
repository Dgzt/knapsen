#ifndef centralwidget_h
#define centralwidget_h

#include <QtGui/QGraphicsView>
#include "enums.h"


class QGraphicsSvgItem;
class MySvgItem;
class QSvgRenderer;
class MyTextItem;
class ScoreTable;
//class QPushButton;

class CentralWidget : public QGraphicsView
{
	Q_OBJECT
	
	bool mGameIsRunning;
	
	QSizeF mCardSize;
	
	MyTextItem* mOpponentName;
	MyTextItem* mPlayerName;
	
	QSvgRenderer* mRenderer;
	QGraphicsSvgItem* mDeck;
	
	int mNumberOfCardsInHand;
	
	QGraphicsSvgItem* mOpponentCards;
	MySvgItem* mPlayerCards;
	
	ScoreTable* mOpponentScoreTable;
	ScoreTable* mPlayerScoreTable;
	
	MySvgItem* mTrumpCard;
	
	QGraphicsSvgItem* mCentralCards;
	
	//QGraphicsProxyWidget* closeButton;
	QGraphicsProxyWidget* mTwentyButton;
	QGraphicsProxyWidget* mFortyButton;
	
	void setInGamePositions();
	//void setTrumpCardPosition();
	
public:
	CentralWidget( QWidget* parent = 0 );
    ~CentralWidget();
	
	void clearWidget();
	
protected:
	void resizeEvent( QResizeEvent* );

private slots:
	void slotClick( int );
	//void closeButtonSlot();
	//void twentyButtonSlot(){ emit signalTwenty(); }
	void slotTwentyButtonClicked(){ emit signalTwentyButtonClicked(); }
	void slotFortyButtonClicked(){ emit signalFortyButtonClicked(); }
	//void fortyButtonSlot(){ emit signalForty(); }
	//void selectedTrumpCardSlot(){ emit signalSelectedTrumpCard(); }
	
public slots:
	
	void slotInitialize( QString, QString, Knapsen::TypeOfCards, int );
	
	void slotNewPlayerCard( int, QString );
	void slotNewOpponentCardId( int );
	
	void slotNewTrumpCard( QString );
	void slotTrumpCardHide();
	
	void slotPlayerCardSelectableChanged( int, bool );

	void slotCentralCardChanged( int, QString );
	
	void slotOpponentSelectedCardId( int );
	
	void slotOpponentTricksChanged( int );
	
	void slotPlayerTricksChanged( int );
	
	void slotDeckVisible( bool );
	
	void slotTwentyButtonVisible( bool );
	void slotFortyButtonVisible( bool );
	
	void slotShowOpponentCards( int, QString, int, QString );
	
	void slotStartGame();
	
	//void opponentScoresChanged( int );
	//void playerScoresChanged( int );

	//void changedDeckVisibleSlot( bool );
	
	//void enabledCloseButtonSlot( bool );
	
	//void enabledTwentyButtonSlot( bool );
	
	//void enabledFortyButtonSlot( bool );
	
	//void selectableTrumpCard( bool );
	
signals:
	void signalSelectedCardId( int );
	//void signalCloseDeck();
	void signalTwentyButtonClicked();
	void signalFortyButtonClicked();
	//void signalSelectedTrumpCard();
	
};


#endif //centralwidget_h
