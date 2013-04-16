/*
 * main.cpp
 *
 * Created by Christophe Daudin on 12/05/09.
 * Copyright 2009 Grame. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.

 * Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 * research@grame.fr
 */
#include <QApplication>
#include <QFileOpenEvent>
#include <QString>
#include <QFontDatabase>
#include <QFile>

#include "MainWindow.h"

#define GUIDO_FONT_FILE "guido2.ttf"

#ifdef Q_WS_MAC
#include "/Developer/Headers/FlatCarbon/CFURL.h"
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




