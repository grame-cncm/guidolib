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
#ifndef GUIDO_IMPORTER_H
#define GUIDO_IMPORTER_H

/**
*	\brief An importer to support the MusicXML format.

	The QGuidoImporter is a static object. When initialized, it checks for the libmusicml2
	library, and when present, it loads the library and resolves musicxml to guido conversion
	entry points.

*/
class QGuidoImporter
{
	public:
		typedef int (* musicxml2guido)(const char *file, bool generateBars, std::ostream& out);
		typedef const char* (* musicxmlversion)();

		/// \return true when the conversion methods are available
		static bool musicxmlSupported();

		/// \return the musicxml lib version as a string
		static const char* musicxmlVersion();

		/// \return the musicxml to guido converter version as a string
		static const char* musicxml2guidoVersion();

		/*! \brief converts a musicxml file to guido
		
			\param file the musicxml file name
			\param generateBars a boolean to force or inhibit measures bar generation
			\param out the output stream
			\return true when the conversion is successful
		*/
		static bool musicxmlFile2Guido   (const char *file, bool generateBars, std::ostream& out);

		/*! \brief converts a musicxml string to guido
		
			\param str the musicxml string
			\param generateBars a boolean to force or inhibit measures bar generation
			\param out the output stream
			\return true when the conversion is successful
		*/
		static bool musicxmlString2Guido (const char *str, bool generateBars, std::ostream& out);

	private:
		static musicxmlversion mMusicXMLVersion;
		static musicxmlversion mMusicXML2GuidoVersion;
		static musicxml2guido mMusicXMLFileConverter;
		static musicxml2guido mMusicXMLStringConverter;
		static bool mMusicXMLSupported;
		static bool initialize();
};

#endif //GUIDO_WIDGET_H
