/*
 * main.cpp
 *
 * Copyright 2019 Grame. All rights reserved.
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

#define GUIDO_FONT		"Bravura"
#define MAXGLYPH		256
#define STARTGLYPH 		0xE000
// This is the end of the Bravura range
#define ENDGLYPH		0xEE3F
// Below is the end of the reserved range, includes optional characters
// #define ENDGLYPH		0xF8FF

static void fontwidth(QFontMetrics& m, int start, int end)
{
	cout << "const int kFontTblOffset = 0x" << std::hex << std::uppercase << STARTGLYPH << ";" << std::dec << endl << endl;
    for (int i=start; i <= end; i++) {
        cout << "	fExtends[" << (i-start) << "] = " << m.width(QString(QChar(i))) << ";" << endl;
	}
}

static void fontheight(QFontMetrics& m)
{
    cout << "static const int kFontHeight = " << m.height() << ";" << endl;
}

static void infos(int size)
{
	cout << "//---------------------------------------------" << endl;
	cout << "// Font metrics generated for the Bravura font" << endl;
	cout << "// Font size is " << size << endl;
	cout << "//---------------------------------------------" << endl << endl;
	cout << "const int kFontSize = " << size << ";" << endl << endl;
}

int main(int argc, char *argv[])
{
	QApplication app( argc , argv );
	const int SIZE = 200;
    QFont font	= QFont("Bravura" , SIZE);
    font.setPixelSize( SIZE );
	QFontMetrics metrics (font);
	infos (SIZE);
	fontheight (metrics);
	cout << endl;
	fontwidth (metrics, STARTGLYPH, ENDGLYPH);
	return 0;
}
