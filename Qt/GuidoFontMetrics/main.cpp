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

static void fontwidth(QFontMetrics& m, int max)
{
	cout << "static const int kGuidoFontWidth = {" << endl;
	for (int i=0; i < max; i++) {
	        QRect rect = m.boundingRect( QChar(i) );
		cout << "	" << rect.width() << ",	// " << i << endl;
        }
        QRect rect = m.boundingRect( QChar(max) );
	cout << "	" << rect.width() << "	// " << max << endl;
	cout << "};" << endl;
}

static void fontheight(QFontMetrics& m, int max)
{
	cout << "static const int kGuidoFontHeight = {" << endl;
	for (int i=0; i < max; i++) {
	        QRect rect = m.boundingRect( QChar(i) );
		cout << "	" << rect.height() << ",	// " << i << endl;
        }
        QRect rect = m.boundingRect( QChar(max) );
	cout << "	" << rect.height() << "	// " << max << endl;
	cout << "};" << endl;
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
	QFont guidoFont	= QFont("Guido2" , SIZE);
	QFontMetrics metrics (guidoFont);
	infos (SIZE);
	fontwidth (metrics, MAXGLYPH);
	cout << endl;
	fontheight (metrics, MAXGLYPH);
	return 0;
}
