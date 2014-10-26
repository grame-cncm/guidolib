#ifndef GUIDOParse_H
#define GUIDOParse_H

/*
  GUIDO Library
  Copyright (C) 2014 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GUIDOEngine.h"
#include "GUIDOExport.h"

class GuidoParser;
class GuidoStream;

#ifdef __cplusplus
extern "C" {
#endif

/*!
\addtogroup Parser Parsing GMN files, strings and guido streams
@{
*/

	//---------------------------------------------------------------------------
	// the new API
	//---------------------------------------------------------------------------

	/*!
		\brief Creates a new parser
		\return a guido parser.
	*/
	GUIDOAPI(GuidoParser *)		GuidoOpenParser ();

	/*!
		\brief Close a guido parser and releases all the associated ressources
		\param p a parser previously opened with GuidoOpenParser
		\return a Guido error code.
	*/
	GUIDOAPI(GuidoErrCode)		GuidoCloseParser (GuidoParser *p);

	/*!
		\brief returns the string of the GuidoStream
		\param gStream a GuidoStream
        \return a std::string.
	*/
    GUIDOAPI(const char *)      GuidoGetStream (GuidoStream * gStream);

	/*!
		\brief Parse a file and create the corresponding AR
		\param p a parser previously opened with GuidoOpenParser
		\param file the file to parse.
		\return a ARHandler or 0 in case of error.
	*/
	GUIDOAPI(ARHandler)			GuidoFile2AR (GuidoParser *p, const char* file);

	/*!
		\brief Parse a string and create the corresponding AR
		\param p a parser previously opened with GuidoOpenParser
		\param str the string to parse.
		\return a ARHandler or 0 in case of error.
	*/
	GUIDOAPI(ARHandler)			GuidoString2AR (GuidoParser *p, const char* str);

	/*!
		\brief Parse a GuidoStream and create the corresponding AR

		\param p a parser previously opened with GuidoOpenParser
		\param stream the GuidoStream to parse.
		\return a ARHandler or 0 in case of error.
	*/
	GUIDOAPI(ARHandler)			GuidoStream2AR (GuidoParser *p, GuidoStream* stream);

	/*!
		\brief Get the error syntax line/column
		\param p a parser previously opened with GuidoOpenParser
		\param line a reference that will contain a line number in case of syntax error
		\param col a reference that will contain a column number in case of syntax error
        \param msg a string that will contain the error message
		\return a Guido error code.
	*/
	GUIDOAPI(GuidoErrCode)		GuidoParserGetErrorCode (GuidoParser *p, int& line, int& col, const char ** msg );

	/*!
		\brief Open a guido stream
		
		Guido streams are intended to implement real-time input to the parser.
		In particular, streams allow to retrieve an AR in while the stream is still opened.
		\return a guido stream.
	*/
	GUIDOAPI(GuidoStream *)		GuidoOpenStream ();

	/*!
		\brief Close a guido stream
		\param s a GuidoStream
		\return a Guido error code.
	*/
	GUIDOAPI(GuidoErrCode)		GuidoCloseStream (GuidoStream *s);

	/*!
		\brief Write data to the stream
		
		Writing data to a stream may be viewed as writing gmn code by portion.
        Syntax errors concerning music/voice/tag/event/parameter non-closure won't be declared
        as such (GuidoWriteStream uses an automatic-closure mechanism).
		When a syntax error (other than a non-closure) occurs when writting data to the stream, the stream becomes invalid
		and should be closed. Further attempts to write data will always result in a syntax error.

        Regarding syntax errors, allowed incomplete constructs are :
			- opened music i.e. { without closing }
			- opened voice i.e. [ without closing ]
			- opened range tag i.e. ( without closing )
			- opened range parameter i.e. < without closing > but with at least one parameter
			- opened chord i.e. ( without closing ) but with at least one note
		\note for incomplete chords and range parameters, the ',' separator must always be followed by a note or a parameter.
		For example, don't write "{a," and then "b}" but "{a" and then ",b}".

		\param s a GuidoStream previoulsy opened with GuidoOpenStream
		\param str a string containing a portion of gmn code
		\return a Guido error code.
	*/
	GUIDOAPI(GuidoErrCode)		GuidoWriteStream (GuidoStream *s, const char* str);

    /*!
		\brief Erase all stream content in order to reuse it

		\param s a GuidoStream previoulsy opened with GuidoOpenStream
		\return a Guido error code.
	*/
	GUIDOAPI(GuidoErrCode)		GuidoResetStream (GuidoStream *s);

/*! @} */


#ifdef __cplusplus
}
#endif

#endif	/* GUIDOParse_H */