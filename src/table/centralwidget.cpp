//#include <KDE/KGlobal>
#include <KDE/KStandardDirs>
//#include <KDE/KDebug>
//#include <KDE/KLocalizedString>
#include "table/centralwidget.h"


CentralWidget::CentralWidget( QWidget* parent ): 
    QDeclarativeView( parent )
{
  setResizeMode( QDeclarativeView::SizeRootObjectToView );
  
  QString path = KStandardDirs::locate("appdata", "qml/main.qml");
  setSource(QUrl::fromLocalFile(path));
}
