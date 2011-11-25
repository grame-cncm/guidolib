/*
 * Copyright 2009 Grame. All rights reserved.
 *
 * This file may be used under the terms of the GNU General Public
 * License version 2.0 as published by the Free Software Foundation
 * and appearing in the file LICENSE.GPL included in the packaging of
 * this file.  Please review the following information to ensure GNU
 * General Public Licensing requirements will be met:
 * http://trolltech.com/products/qt/licenses/licensing/opensource/
 *
 * If you are unsure which license is appropriate for your use, please
 * review the following information:
 * http://trolltech.com/products/qt/licenses/licensing/licensingoverview
 * or contact the sales department at sales@trolltech.com.
 *
 * In addition, as a special exception, Trolltech gives you certain
 * additional rights. These rights are described in the Trolltech GPL
 * Exception version 1.0, which can be found at
 * http://www.trolltech.com/products/qt/gplexception/ and in the file
 * GPL_EXCEPTION.txt in this package.
 *
 * In addition, as a special exception, Trolltech, as the sole copyright
 * holder for Qt Designer, grants users of the Qt/Eclipse Integration
 * plug-in the right for the Qt/Eclipse Integration to link to
 * functionality provided by Qt Designer and its related libraries.
 *
 * Trolltech reserves all rights not expressly granted herein.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
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

