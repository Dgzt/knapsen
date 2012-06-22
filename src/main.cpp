#include <QtCore/QTime>
#include <KDE/KApplication>
#include <KDE/KAboutData>
#include <KDE/KCmdLineArgs>
#include <KDE/KLocale>
#include "mainwidget.h"

static const char description[] =
    I18N_NOOP("Knapsen is a Sixty-six card game for KDE.");

static const char version[] = "1.0.0";

int main(int argc, char **argv)
{
	//To the random number generator
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
	
    KAboutData about("knapsen", 
					 0, 
					 ki18n("Knapsen"), 
					 version, 
					 ki18n(description),
                     KAboutData::License_GPL_V3, 
					 ki18n("(C) 2012 Zsuró Tibor")//,
					 //KLocalizedString(), 
					 //0, 
					 //"zsurotibor" "@" "gmail" "." "com"
					);
	
    //about.addAuthor( ki18n("Zsuró Tibor"), KLocalizedString(), "zsurotibor" "@" "gmail" "." "com" );
    
	KCmdLineArgs::init(argc, argv, &about);

    KApplication app;

    MainWindow *mainWindow = new MainWindow;
	mainWindow->show();

    return app.exec();
}
