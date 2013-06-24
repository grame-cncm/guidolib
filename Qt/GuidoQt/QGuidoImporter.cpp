/*
 * Copyright 2009 Grame. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.

 * Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 * research@grame.fr
 *
 */

#include <QLibrary>

#include "QGuidoImporter.h"

#if __APPLE__
# define musicxmllib	"libmusicxml2.framework/libmusicxml2"
#elif defined(WIN32)
# define musicxmllib	"libmusicxml2"
#else
# define musicxmllib	"libmusicxml2"
#endif

//-------------------------------------------------------------------------
bool QGuidoImporter::mMusicXMLSupported = QGuidoImporter::initialize();

QGuidoImporter::musicxml2guido QGuidoImporter::mMusicXMLFileConverter = 0;
QGuidoImporter::musicxml2guido QGuidoImporter::mMusicXMLStringConverter = 0;
QGuidoImporter::musicxmlversion QGuidoImporter::mMusicXMLVersion = 0;
QGuidoImporter::musicxmlversion QGuidoImporter::mMusicXML2GuidoVersion = 0;

//-------------------------------------------------------------------------
bool QGuidoImporter::initialize()
{
	mMusicXMLFileConverter = mMusicXMLStringConverter = 0;
	QLibrary libmxml(musicxmllib);
	if (libmxml.load()) {
		mMusicXMLFileConverter	 = (musicxml2guido)libmxml.resolve("musicxmlfile2guido");
		mMusicXMLStringConverter = (musicxml2guido)libmxml.resolve("musicxmlstring2guido");
		mMusicXMLVersion		 = (musicxmlversion)libmxml.resolve("musicxmllibVersionStr");
		mMusicXML2GuidoVersion	 = (musicxmlversion)libmxml.resolve("musicxml2guidoVersionStr");
	}
	return (mMusicXMLFileConverter && mMusicXMLStringConverter);
}

//-------------------------------------------------------------------------
const char* QGuidoImporter::musicxmlVersion()
{
	return mMusicXMLVersion ? mMusicXMLVersion() : "not available";
}

//-------------------------------------------------------------------------
const char* QGuidoImporter::musicxml2guidoVersion()
{
	return mMusicXML2GuidoVersion ? mMusicXML2GuidoVersion() : "not available";
}

//-------------------------------------------------------------------------
bool QGuidoImporter::musicxmlSupported()
{
	return mMusicXMLSupported;
}

//-------------------------------------------------------------------------
bool QGuidoImporter::musicxmlFile2Guido (const char *file, bool generateBars, std::ostream& out)
{
	return mMusicXMLFileConverter ? (mMusicXMLFileConverter(file, generateBars, out) == 0) : false;
}

//-------------------------------------------------------------------------
bool QGuidoImporter::musicxmlString2Guido (const char *str, bool generateBars, std::ostream& out)
{
	return mMusicXMLStringConverter ? (mMusicXMLStringConverter(str, generateBars, out) == 0) : false;
}

