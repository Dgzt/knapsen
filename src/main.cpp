/***************************************************************************
 *   Copyright (C) %{CURRENT_YEAR} by %{AUTHOR} <%{EMAIL}>                            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/


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
