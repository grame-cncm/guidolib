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

#include <iostream>

#include "VGDevice.h"
#include "VGFont.h"
#include "GSystemOSX.h"

using namespace std;


#define GUIDO_FONT		"Guido2"
#define MAXGLYPH		256

static void fontwidth(const VGFont* font, int max, VGDevice* dev)
{
	float width, height;
    for (int i=0; i <= max; i++) {
		font->GetExtent( i, &width, &height, dev);
        cout << "	fExtends[" << i << "] = " << width << ";" << endl;
	}
}

static void fontheight(const VGFont* font, int max, VGDevice* dev)
{
	float width, height;
	font->GetExtent( 66, &width, &height, dev); // arbitrary value
    cout << "static const int kGuidoFontHeight = " << height << ";" << endl;
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
	const int SIZE = 200;
	
	GSystemOSX sys(0,0);
	const VGFont* guidoFont = sys.CreateVGFont( "Guido2" , SIZE, 0 );
	VGDevice* dev = sys.CreateMemoryDevice (50,50);
	
	infos (SIZE);
	fontheight (guidoFont, MAXGLYPH, dev );
	cout << endl;
	fontwidth (guidoFont, MAXGLYPH, dev);
	return 0;
}
