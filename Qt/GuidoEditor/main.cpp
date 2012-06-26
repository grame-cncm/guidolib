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

#include "MainWindow.h"

#define GUIDO_FONT_FILE "guido2.ttf"

#ifdef Q_WS_MAC
#include "/Developer-3.2.5/Headers/FlatCarbon/CFURL.h"
static QString getGuidoFontPath()
{
	CFURLRef appUrlRef = CFBundleCopyBundleURL(CFBundleGetMainBundle());
	CFStringRef macPath = CFURLCopyFileSystemPath(appUrlRef, kCFURLPOSIXPathStyle);
	const char *bundlePath = CFStringGetCStringPtr(macPath, CFStringGetSystemEncoding());
	CFRelease(appUrlRef);
	CFRelease(macPath );
	
	QString guidoFontPath = QString(bundlePath) + "/Contents/Resources/" + QString(GUIDO_FONT_FILE);
	
	if ( QFile::exists(guidoFontPath) )
		return guidoFontPath ;
	else
	{
		if ( QFile::exists(GUIDO_FONT_FILE) )
			return GUIDO_FONT_FILE;
		else return "";
	}
}
#else
static QString getGuidoFontPath()
{
	return GUIDO_FONT_FILE;
}
#endif

void installGuidoFont()
{
	QString fileName = getGuidoFontPath();
	if ( !fileName.isEmpty() )
		QFontDatabase::addApplicationFont ( fileName );
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	installGuidoFont();
	MainWindow mainWin;
	QApplication::instance()->installEventFilter( &mainWin );
	mainWin.show();

	if ( argc >= 2 )
	{
		QString fileName( argv[1] );
		QFileOpenEvent event(fileName);
		QApplication::sendEvent( &mainWin , &event );
	}
	return app.exec();
}




