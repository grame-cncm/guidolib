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
#include <QPainter>
#include <QImage>

#include "QGuidoWidget.h"

//#define __UseOldGUIDOFactory__
#include "GuidoFactory.h"

static void alter (ARFactoryHandler f, float detune)
{
	GuidoFactoryOpenRangeTag (f, "alter", 0);
	GuidoFactoryAddTagParameterFloat (f, detune);
	GuidoFactorySetParameterName (f, "detune");
	GuidoFactoryCloseTag (f);
}

static void note (ARFactoryHandler f, const char* name, int accidental)
{
	GuidoFactoryOpenEvent (f, name);
	if (accidental > 0) GuidoFactoryAddSharp (f);
	GuidoFactoryCloseEvent (f);
}

static ARHandler factory ()
{
    ARFactoryHandler f;
	GuidoFactoryOpen(&f);               // open the GUIDO Factory first
	
	GuidoFactoryOpenMusic(f);           // and create a new score	
   
	GuidoFactoryOpenVoice(f);           // open a new voice
	note (f, "c", 0);
	note (f, "e", 1);
	alter (f, -0.5);
	note (f, "f", 0);
	GuidoFactoryEndTag(f);
	GuidoFactoryCloseVoice(f);          // close the voice which is now part of the opened score

	GuidoFactoryOpenVoice(f);           // open a second voice
	GuidoFactoryOpenTag (f, "meter", 0);
	GuidoFactoryAddTagParameterString (f, "3/4");
	GuidoFactoryCloseTag (f);

	GuidoFactoryOpenTag (f, "i", 0);
	GuidoFactoryAddTagParameterString (f, "ff");
	GuidoFactoryCloseTag (f);
	
	GuidoFactoryOpenRangeTag (f, "slur", 0);
	GuidoFactoryCloseTag (f);

	note (f, "a", 0);
	alter (f, 0.5);
	note (f, "d", 0);
	GuidoFactoryEndTag(f);
	note (f, "e", 0);
	GuidoFactoryEndTag(f);

	GuidoFactoryCloseVoice(f);          // close the voice which is now part of the opened score
	
	ARHandler ar = GuidoFactoryCloseMusic(f);     // extract the GAR handle
	GuidoFactoryClose (f);
	return ar;
}

int main(int argc , char* argv[])
{
	QApplication app(argc, argv);

	QGuidoPainter::startGuidoEngine();

	QGuidoWidget w;
	w.resize (300, 200);
	w.setARHandler( factory() );	
	w.show();

	int result = app.exec();
	QGuidoPainter::stopGuidoEngine();
	
	return result;
}
