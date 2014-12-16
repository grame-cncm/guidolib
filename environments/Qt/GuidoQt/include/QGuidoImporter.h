/*
 * Copyright 2009 Grame. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.

 * Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 * research@grame.fr
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
