/*
 * main.cpp
 *
 * Created by Christophe Daudin on 12/05/09.
 * Copyright 2009 Grame. All rights reserved.
 *
 * GNU Lesser General Public License Usage
 * Alternatively, this file may be used under the terms of the GNU Lesser
 * General Public License version 2.1 as published by the Free Software
 * Foundation and appearing in the file LICENSE.LGPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU Lesser General Public License version 2.1 requirements
 * will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
#include <QApplication>
#include <QFileOpenEvent>
#include <QString>
#include <QFontDatabase>
#include <QFile>
#include <QtDebug>

#include "MainWindow.h"

#ifdef GUIDO_CALCULUS_APP
#define APP_NAME "GuidoCalculus"
#else
#define APP_NAME "GuidoSceneComposer"
#endif
#define COMPANY_NAME "GRAME"


#define GUIDO_FONT_FILE "guido2.ttf"
#ifdef Q_WS_MAC
#include <CoreFoundation/CoreFoundation.h>
static QString getGuidoFontPath()
{
	QString guidoFontPath = QString(APP_NAME) + ".app/Contents/Resources/" + GUIDO_FONT_FILE;

	if ( QFile::exists(guidoFontPath) )
		return guidoFontPath ;
	else
	{
		if ( QFile::exists(GUIDO_FONT_FILE) )
			return GUIDO_FONT_FILE;
		else
			return  "";
	}
}
#else
static QString getGuidoFontPath()
{
	return GUIDO_FONT_FILE;
}
#endif


static void installGuidoFont()
{
	QString fileName = getGuidoFontPath();
	if ( !fileName.isEmpty() )
		QFontDatabase::addApplicationFont ( fileName );
}

#define Stringify(x) #x

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	Q_INIT_RESOURCE( application );
#ifdef USES_GUIDO_AR
	Q_INIT_RESOURCE( guidoar );
#endif

 	QCoreApplication::setOrganizationName(COMPANY_NAME);
	QCoreApplication::setApplicationName(APP_NAME);

	installGuidoFont();

	GraphicsSceneMainWindowSettings s;
	s.mLanguageNameLong = "Guido Music Notation";
	s.mLanguageNameShort = "Guido";
	s.mLanguageFileExtension = "gmn";
	s.mSceneFileExtension = "gsc";
	s.mDefaultLanguageCode = "[\\meter<\"4/4\"> c2 b1 a g f e d c]";
	s.mLanguageCommandsFile = ":/GuidoCommands.xml";
#ifdef USES_GUIDO_AR
	s.mHasStorage = true;
	s.mHasHistory = true;
#else
	s.mHasStorage = false;
	s.mHasHistory = false;
#endif
	s.mMaxItemSize = QSize(200,200);
	
	GraphicsSceneMainWindow::initApplicationSettings( s );
	MainWindow mainWin;
	QApplication::instance()->installEventFilter( &mainWin );

	if ( argc >= 2 )
	{
		QString fileName( argv[1] );
		QFileOpenEvent fileOpenEvent( fileName );
		QApplication::sendEvent( &mainWin , &fileOpenEvent );
	}

	mainWin.show();
	int result = app.exec();
	return result;
}
