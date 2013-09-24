#ifndef GUIDOParse_H
#define GUIDOParse_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003, 2004, 1013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <istream>
#include "GUIDOExport.h"
#include "GuidoParser.h"
#include "GuidoStream.h"

struct NodeAR;
struct NodeGR;
typedef struct NodeAR *  ARHandler;
typedef struct NodeGR *  GRHandler;
typedef const struct NodeAR *  CARHandler;
typedef const struct NodeGR *  CGRHandler;

using namespace guido;

enum GuidoErrCode;


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
	GUIDOAPI(guido::GuidoParser *)		GuidoOpenParser (GuidoStream *s);

	/*!
		\brief Close a guido parser and releases all the associated ressources
		\param p a parser previously opened with GuidoOpenParser
		\return a Guido error code.
	*/
	GUIDOAPI(GuidoErrCode)		GuidoCloseParser (GuidoParser *p);

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
	GUIDOAPI(ARHandler)			GuidoStream2AR (GuidoParser *p, const GuidoStream* stream);

	/*!
		\brief Try to convert the current parser state to AR
		\param p a parser previously opened with GuidoOpenParser
		\return a ARHandler or 0 in case of error.
	*/
	GUIDOAPI(ARHandler)			GuidoParser2AR (GuidoParser *p);


	/*!
		\brief Try to convert the current parser state to AR
		\param p a parser previously opened with GuidoOpenParser
		\param line a reference that will contains a line number in case of syntax error
		\param col a reference that will contains a column number in case of syntax error
		\return a Guido error code.
	*/
	GUIDOAPI(GuidoErrCode)		GuidoParserGetErrorCode (GuidoParser *p, int& line, int& col);

	/*!
		\brief Open a guido stream
		
		Guido streams are intended to implement real-time input to the parser.
		In particular, streams allow to retrieve an AR in while the stream is still opened.
		\return a guido stream.
	*/
	GUIDOAPI(GuidoStream*)		GuidoOpenStream ();

	/*!
		\brief Close a guido stream
		\param s a GuidoStream
		\return a Guido error code.
	*/
	GUIDOAPI(GuidoErrCode)		GuidoCloseStream (GuidoStream *s);

	/*!
		\brief Write data to the stream
		
		Writing data to a stream may be viewed as writing gmn code by portion. 
		The sequence of all the data written to the stream should always be valid gmn code, 
		provided that a valid gmn sequence may be writtent to properly close the stream.
		When a syntax error occurs when writting data to the stream, the stream becomes invalid
		and should be closed. Further attempts to write data will always result in a syntax error.
		
		\param s a GuidoStream previoulsy opened with GuidoOpenStream
		\param str a string containing a portion of gmn code
		\return a Guido error code.
	*/
	GUIDOAPI(GuidoErrCode)		GuidoWriteStream (GuidoStream *stream, const char* str);


//inline GuidoStream* operator <<(GuidoStream* stream, const char* str)	{ GuidoWriteStream(stream, str)); return stream; } //REM: décommenter

/*! @} */


#ifdef __cplusplus
}
#endif

#endif
