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

#include <iostream>
#include <vector>
using namespace std;

#define MAXGLYPH		256

typedef struct FontInfo {
	const char *	fName;
	int				fSize;
	QFont			fFont;
	QFontMetrics	fMetrics;
	
	FontInfo (const char* name, int size)
		: fName(name), fSize(size), fFont(name, size), fMetrics(fFont) {}
} FontInfo;

const int SIZE = 12;

//--------------------------------------------------------------------------
static void usage(const char * name)
{
	cerr << "usage: " << name << "<font list>" << endl;
	exit(1);
}

static void header(const vector<FontInfo>& l, const char* what, int size)
{
	cout << "//---------------------------------------------" << endl;
	cout << "// Font " << what << " generated for size " << size << endl;
	for (unsigned int i=0; i<l.size(); i++)
		cout << l[i].fName << "	";
	cout << "mean	var" << endl;
}

static void fontwidth(const vector<FontInfo>& l, int max)
{
	header (l, "width", SIZE);
	for (int i=0; i < max; i++) {
		float sum = 0;
		int min = 9999; int max = 0;
		for (unsigned int j=0; j<l.size(); j++) {
			QRect rect = l[j].fMetrics.boundingRect( QChar(i) );
			cout << rect.width() << "	";
			sum += rect.width();
			if (rect.width() < min) min = rect.width();
			if (rect.width() > max) max = rect.width();
		}
		cout << (sum / l.size()) << "	" << (max - min) << endl;
	}
}

static void fontheight(const vector<FontInfo>& l, int max)
{
	header (l, "height", SIZE);
	for (int i=0; i < max; i++) {
		float sum = 0;
		int min = 9999; int max = 0;
		for (unsigned int j=0; j<l.size(); j++) {
			QRect rect = l[j].fMetrics.boundingRect( QChar(i) );
			cout << rect.height() << "	";
			sum += rect.height();
			if (rect.height() < min) min = rect.height();
			if (rect.height() > max) max = rect.height();
		}
		cout << (sum / l.size()) << "	" << (max - min) << endl;
	}
}

//--------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	if (argc < 2) usage(argv[0]);

	QApplication app( argc , argv );	// required by Qt
	vector<FontInfo> fontlist;
	for (int i=1; i<argc; i++) {
		FontInfo fi(argv[i], SIZE);
		fontlist.push_back(fi);
	}
	fontwidth (fontlist, MAXGLYPH);
	fontheight (fontlist, MAXGLYPH);
	return 0;
}
