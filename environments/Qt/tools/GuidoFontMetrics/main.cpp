/*
 * main.cpp
 *
 * Copyright 2014 Grame. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.

 * Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 * research@grame.fr
 */

#include <QApplication>
#include <QFont>
#include <QFontMetrics>
#include <QVariant>
#include <QString>

#include <iostream>
using namespace std;

#define GUIDO_FONT		"Guido2"
#define MAXGLYPH		256

#ifdef QTFONTMETRICS_511
#define WIDTH(m, arg) m.horizontalAdvance(arg)
#else
#define WIDTH(m, arg) m.width(arg)
#endif

static void fontwidth(QFontMetrics& m, int max)
{
//    cout << "static const int kGuidoFontWidth[] = {" << endl;
    for (int i=0; i <= max; i++) {
        cout << "	fExtends[" << i << "] = " << WIDTH(m, QString(QChar(i))) << ";" << endl;
        }
//	cout << "};" << endl;
}

static void fontheight(QFontMetrics& m, int max)
{
    cout << "static const int kGuidoFontHeight = " << m.height() << ";" << endl;
//    for (int i=0; i <= max; i++) {
//        cout << "	" << m.height() << ",	// " << i << endl;
//        }
//	cout << "};" << endl;
}

static void infos(int size)
{
	cout << "//---------------------------------------------" << endl;
	cout << "// Font metrics generated for the Guido2 font" << endl;
	cout << "// Font size is " << size << endl;
	cout << "//---------------------------------------------" << endl << endl;
	cout << "const int kFontSize = " << size << ";" << endl << endl;
}

int main(int argc, char *argv[])
{
	QApplication app( argc , argv );	// required by Qt
	const int SIZE = 200;
    QFont guidoFont	= QFont("Guido2" , SIZE); // This size is in point but we'll set a size in pixels anyway
    guidoFont.setPixelSize( SIZE );
	QFontMetrics metrics (guidoFont);
	infos (SIZE);
	fontheight (metrics, MAXGLYPH);
	cout << endl;
	fontwidth (metrics, MAXGLYPH);
	return 0;
}
