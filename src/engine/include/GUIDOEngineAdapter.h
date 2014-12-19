#ifndef GUIDOENGINEADAPTER_H
#define GUIDOENGINEADAPTER_H
/*
  GUIDO Library
  Copyright (C) 2014	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <string>
#include "GUIDOEngine.h"

class GuidoParser;
class GuidoStream;

/*!
 * \brief A structure for parser error.
 */
typedef struct {
	//! line a reference that will contain a line number in case of syntax error
	int line;
	//!	col a reference that will contain a column number in case of syntax error
	int col;
	//!	msg a string that will contain the error message
	std::string msg;
} ParserError;

/*!
 * \brief A structure for guido version, composed of 3 digit.
 */
typedef struct {
	//! major major number version
	int major;
	//! minor minor number version
	int minor;
	//! sub sub number version
	int sub;
	//! String representation of guido version
	std::string str;
} GuidoVersion;

/*!
 * \addtogroup APICplusplus C++ interface
 * @{
 * \defgroup EngineAdapter Guido Engine Adapter
 * \brief A C++ interface to the Guido Engine API
 *
 * A C++ class to manupilate Guido Engine.
 */
class_export GuidoEngineAdapter
{
    public:
		/*!
		 * \brief GuidoEngineAdapter
		 */
		GuidoEngineAdapter();
		/*!
		 * \brief ~GuidoEngineAdapter
		 */
		virtual ~GuidoEngineAdapter();


		/*!
			\addtogroup engine2 Building abstract and graphic representations
			@{
			\ingroup EngineAdapter

		*/
		/*!
			\brief Initialises the Guido Engine. Must be called before any attempt
			to read a Guido file or to use the Guido Factory
			\param desc the graphic environment description.
			\return a Guido error code.

			WARNING: the caller must ensure desc maintains a constant reference on a
			valid VGDevice, because Guido keeps it internally (to calculate fonts, etc.)
		*/
		GuidoErrCode init(GuidoInitDesc * desc);

		/*!
		 * \brief Initialise the Guido Engine with a internal SVGSystem and SVGDevice.
		 * This system and device have embind font metrics. They are used to calculate score
		 * in environnement without font metrics like javascript.
		 *
		 */
		GuidoErrCode init();

		/*!
			\brief Guido Engine shutdown.

			Actually release the font allocated by the engine.
			Anyway, the fonts are release when the client application exit but
			the function provides control over the time of the release.
		*/
		void	 shutdown();

		/*!
			\brief Transforms a Guido abstract representation into a Guido graphic representation.
			The engine applies layout algorithms according to the settings given as argument.
			Default settings are applied.
			\note You can safely free the AR after the transformation.

			\param ar the handler to the abstract representation.
			\return a Guido opaque handle to a graphic music representation.
					It's the caller responsability to free the handle using freeGR.
		*/
		GRHandler ar2gr(ARHandler ar);

		/*!
			\brief Transforms a Guido abstract representation into a Guido graphic representation.
			The engine applies layout algorithms according to the settings given as argument.

			\note You can safely free the AR after the transformation.

			\param ar the handler to the abstract representation.
			\param settings a pointer to the settings for the graphic layout.
			\return a Guido opaque handle to a graphic music representation.
					It's the caller responsability to free the handle using freeGR.
		*/
		GRHandler ar2gr(ARHandler ar, const GuidoLayoutSettings &settings);

		/*!
			\brief Applies new layout settings to an existing Guido graphic representation.
			Default	settings are applied.
			\param gr the handler to the graphic representation.
			\return a Guido error code.
		*/
		GuidoErrCode updateGR(GRHandler gr);

		/*!
			\brief Applies new layout settings to an existing Guido graphic representation.
			\param gr the handler to the graphic representation.
			\param settings the settings for the graphic layout.
			\return a Guido error code.
		*/
		GuidoErrCode updateGR(GRHandler gr, const GuidoLayoutSettings &settings);

		/*!
			\brief Releases a Guido abstract representation.
			This method must be called after freeGR.
			\param ar the handler to the abstract representation.
		*/
		void freeAR(ARHandler ar);

		/*!
			\brief Releases a Guido graphic representation.
			Don't release the ARHandler refenrece in the GRHandler before release the GR.
			\param gr the handler to the graphic representation.
		*/
		void freeGR(GRHandler gr);

		/*!
			\brief Gives a textual description of a Guido error code.

			\param errCode a Guido error code.
			\return a string describing the error.
		*/
		std::string getErrorString(GuidoErrCode errCode);

		/*!
			\brief Gives the default values of the layout settings.

			\return the settings fill with default values.
		*/
		GuidoLayoutSettings getDefaultLayoutSettings();
		/*! @} */

		/*!
			\addtogroup Pages2 Browsing music pages
			\ingroup EngineAdapter
			The Guido Engine produces pages of music and therefore, the graphic representation
			consists in a collection of pages.
			The following functions are intended to access these pages by page number as well
			as by date. Page numbers start at 1.
		@{
		*/

		/** \brief Gives the number of score pages of the graphic representation.

			\param inHandleAR a Guido opaque handle to a AR structure.
			\return the number of voices or a guido error code.
		*/
		int countVoices(CARHandler inHandleAR);

		/** \brief Gives the number of score pages of the graphic representation.

			\param inHandleGR a Guido opaque handle to a GR structure.
			\return a number of pages or a guido error code.
		*/
		int getPageCount(CGRHandler inHandleGR);

		/** \brief Gives the number of systems on a given page.

			\param inHandleGR a Guido opaque handle to a GR structure.
			\param page a page number (starts at 1).
			\return the systems count on the given page or a guido error code.
		*/
		int getSystemCount(CGRHandler inHandleGR, int page);

		/** \brief Returns the music duration of a score.

			The duration is expressed as a fractional value where 1 represents
			a whole note.

			\param inHandleGR a Guido opaque handle to a GR structure.
			\return the duration expressed as a fractional value.
		*/
		GuidoDate duration(CGRHandler inHandleGR);

		/** \brief Finds the page which has an event (note or rest) at a given date.

			\bug returns page + 1 when input date falls on the last system.
			\param inHandleGR a Guido opaque handle to a GR structure.
			\param date the target date.
			\return a page number if greater than 0,
					0 if no page found,
		*/
		int findEventPage(CGRHandler inHandleGR, const GuidoDate& date);

		/** \brief Finds the page which contain a given date.

			\bug returns page + 1 when input date falls on the last system.
			\param inHandleGR a Guido opaque handle to a GR structure.
			\param date the target date.
			\return a page number if greater than 0,
					0 if no page found,
		*/
		int findPageAt(CGRHandler inHandleGR, const GuidoDate& date);

		/** \brief Gives the time location of a Page.

			\param inHandleGR a Guido opaque handle to a GR structure.
			\param pageNum a page number (starts at 1).
			\return a GuidoDate
		*/
		GuidoDate getPageDate(CGRHandler inHandleGR, int pageNum);
		/*! @} */

		/*!
			\addtogroup Format2 Score drawing and pages formating
			\ingroup EngineAdapter
			The GuidoEngine makes use of internal units for graphic operations.
			The functions that query or set graphic dimensions always makes use of
			this internal unit. Conversion functions are provided to convert to standard
			units.
		@{
		*/

		/** \brief Draws one page of score into a graphic device.

			\param desc informations about what to draw and how to draw.
			\return a Guido error code
		*/
		GuidoErrCode onDraw(GuidoOnDrawDesc * desc);

		/** \brief Exports one page of score to SVG.

			\param handle a graphic representation.
			\param page the page number.
			\param out the output stream.
			\param embedFont if true, it use a embbed guido font and add it to the SVG.
			\param font path of the guido svg font file or an svg font in text representation. Can be use if embedFont is set to false else it is unnecessary.
			If the library is compiled with INDEPENDENTSVG option, don't use a font file. The library cannot calculate font metric. You have better to use embedded font.
			\param mappingMode the mapping mode (see mapping mode enum).
			\return a Guido error code
		 */
		GuidoErrCode gr2SVG( const GRHandler handle, int page, std::ostream& out, bool embedFont = true, const char* font = 0, const int mappingMode = 0 );

		/** \brief Exports one page of score to SVG.

			\param handle a graphic representation.
			\param page the page number.
			\param embedFont if true, it use a embbed guido font and add it to the SVG.
			\param mappingMode the mapping mode (see mapping mode enum).
			\return a Guido error code
		 */
		std::string gr2SVG(const GRHandler handle, int page, bool embedFont = true, const int mappingMode = 0);

		/** \brief Exports an abstract representation of GUIDO draw commands.

			\param handle a graphic representation.
			\param page the page number.
			\param out the output stream.
			\return a Guido error code
		*/
		GuidoErrCode abstractExport(const GRHandler handle, int page, std::ostream& out);

		/** \brief Exports an abstract representation of GUIDO draw commands.

			\param handle a graphic representation.
			\param page the page number.
			\return the export in a string
		*/
		std::string abstractExport(const GRHandler handle, int page);

		/** \brief Exports an representation of GUIDO draw commands in a data-reduced dsl

			\param handle a graphic representation.
			\param page the page number.
			\param out the output stream.
			\return a Guido error code
		*/
		GuidoErrCode binaryExport(const GRHandler handle, int page, std::ostream& out);

		/** \brief Exports an representation of GUIDO draw commands in a data-reduced dsl

			\param handle a graphic representation.
			\param page the page number.
			\return a Guido error code
		*/
		std::string binaryExport(const GRHandler handle, int page);

#ifdef CANVASSYSTEM
		/** \brief Exports one page of score on html canvas.

			\param handle a graphic representation.
			\param page the page number.
			\return a Guido error code
		*/
		GuidoErrCode javascriptExport(const GRHandler handle, int page);
#endif
		/** \brief Control bounding boxes drawing.

			\param bbMap a bits field indicating the set of bounding boxes to draw (default to none).
		*/
		void setDrawBoundingBoxes(int bbMap);

		/** \brief Gives bounding boxes drawing state.
		 *	\return the bit field.
		*/
		int getDrawBoundingBoxes();

		/** \brief Gives a score page format.

			\param inHandleGR a Guido opaque handle to a GR structure.
			\param pageNum a page number.
			\return the page format
		*/
		GuidoPageFormat getPageFormat(CGRHandler inHandleGR, int pageNum);

		/** \brief Sets the default score page format.

			The default page format is used when no \p \\pageFormat tag is present.
			Parameters are Guido internal units. Default values for the default page
			format are:
			- paper size: A4
			- left margin: 2cm
			- right margin: 2cm
			- top margin: 5cm
			- bottom margin: 3cm

			\param format the page format
		*/
		void setDefaultPageFormat(const GuidoPageFormat &format);

		/** \brief Gives the default score page format.

			\return the page format
		*/
		GuidoPageFormat getDefaultPageFormat();


		/** \brief Converts internal Guido units into centimeters.

			\param val the value to be converted
			\return the converted value
		*/
		float unit2CM(float val);

		/** \brief Converts centimeters into internal Guido units.

			\param val the value to be converted
			\return the converted value
		*/
		float cm2Unit(float val);

		/** \brief Converts internal Guido units into inches.

			\param val the value to be converted
			\return the converted value
		*/
		float unit2Inches(float val);

		/** \brief Converts inches into internal Guido units.

			\param val the value to be converted
			\return the converted value
		*/
		float inches2Unit(float val);

		/** \brief Resize the page sizes to the music size.

			\param inHandleGR a Guido opaque handle to a GR structure.
			\return a Guido error code.
		*/
		GuidoErrCode resizePageToMusic( GRHandler inHandleGR );
		/*! @} */


		/*!
		\addtogroup Misc2 Miscellaneous
		\ingroup EngineAdapter
		Includes various functions for version management and for conversions.
		The number of version functions is due to historical reasons.
		@{
		*/
		/**	\brief Gives the library version number as three integers.

			Version number format is  MAJOR.MINOR.SUB

			\return a GuidoVersion structure.
		*/
		GuidoVersion getVersion();

		/**	\brief Checks a required library version number.

			\param major the major revision number.
			\param minor the minor revision number.
			\param sub the sub revision number.
			\return noErr if the library version number is greater or equal to the version number
				passed as argument.
			\return otherwise guidoErrActionFailed.
		*/
		GuidoErrCode checkVersionNums(int major, int minor, int sub);


		/** \brief Gives the distance between two staff lines.

			This value is constant (= 50). It does not depend on the context, it will
			probably never change in future versions of the library.

			\return the distance between two lines of staff, in Guido internal units.
		*/
		float getLineSpace();


		/** \brief Gives a color to all notes of a voice between a given time interval.

			\note Introduced for GUIDO/MIR; Allows the user to see where
					a musical theme appears in a voice.

			\param inHandleAR a Guido opaque handle to an AR structure.
			\param voicenum index of the voice to mark, starting from 1
			\param date the date where the color-marking must begin (whole note = 1)
			\param duration the duration that must be covered by the color marking.
			\param red the red component of the marking color, from 0 to 255.
			\param green green color component.
			\param blue blue color component.
			\return a Guido error code.
		*/
		GuidoErrCode markVoice( ARHandler inHandleAR, int voicenum,
												const GuidoDate & date, const GuidoDate & duration,
												unsigned char red, unsigned char green, unsigned char blue );


		/**	\brief Makes the correspondance between an ARMusic and a path.

			\param inHandleAR the destination ARHandler.
			\param inPaths the path to associate.
			\return noErr if the association has been made with success
			\return otherwise guidoErrActionFailed.
		*/
		GuidoErrCode setSymbolPath(ARHandler inHandleAR, const std::vector<std::string> &inPaths);


		/**	\brief Returns the path corresponding to an AR.

			\param inHandleAR the handle given to extract its path.
			\return the vector of path.
		*/
		std::vector<std::string> getSymbolPath(const ARHandler inHandleAR);

		/*! @} */


		/*!
		\addtogroup time2 Timing measurements
		Includes functions to query the time spent by the main Guido Engine operations.
		\ingroup EngineAdapter
		@{
		*/
		/*!
			\brief Gets parsing time
			\param ar the ar handler given to extract the parsing time
			\return the time spent on building the AR representation (in msl) or -1 for invalid handlers
		*/
		long getParsingTime (const ARHandler ar);

		/** \brief Gets AR to GR procedure time

			\param gr the gr handler given to extract the AR2GR time
			\return the time spent to convert the AR representation to GR (in msl) or -1 for invalid handlers
		*/
		long getAR2GRTime(const GRHandler gr);

		/** \brief Gets GR drawing procedure time

			\param gr the gr handler given to extract the drawing time
			\return the time spent on the last OnDraw call (in msl) or -1 if OnDraw has not yet been called or for invalid handlers
		*/
		long getOnDrawTime(const GRHandler gr);

		/*! @} */
		/*!
		\addtogroup parser Parsing GMN files, strings and guido streams
		@{
		\ingroup EngineAdapter
		*/
		/*!
			\brief Creates a new parser
			\return a guido parser.
		*/
		GuidoParser * openParser();

		/*!
			\brief Close a guido parser and releases all the associated ressources
			\param p a parser previously opened with openParser
			\return a Guido error code.
		*/
		GuidoErrCode closeParser(GuidoParser *p);

		/*!
			\brief Parse a file and create the corresponding AR
			\param parser a parser previously opened with openParser
			\param file the file to parse.
			\return a ARHandler or 0 in case of error.
		*/
		ARHandler file2AR(GuidoParser *parser, const std::string &file);

		/*!
			\brief Parse a string and create the corresponding AR
			\param parser a parser previously opened with openParser
			\param gmnCode the string to parse.
			\return a ARHandler or 0 in case of error.
		*/
		ARHandler string2AR(GuidoParser *parser, const std::string &gmnCode);

		/*!
			\brief returns the string of the stream
			\param gStream a stream
			\return a std::string.
		*/
		std::string getStream(GuidoStream *gStream);

		/*!
			\brief Parse a stream and create the corresponding AR

			\param p a parser previously opened with openParser
			\param stream the stream to parse.
			\return a ARHandler or 0 in case of error.
		*/
		ARHandler stream2AR(GuidoParser *p, GuidoStream* stream);

		/*!
			\brief Get the error syntax line/column/message
			\param p a parser previously opened with openParser
			\return a ParserError structure.
		*/
		ParserError parserGetErrorCode(GuidoParser *p);

		/*!
			\brief Open a guido stream

			Guido streams are intended to implement real-time input to the parser.
			In particular, streams allow to retrieve an AR in while the stream is still opened.
			\return a guido stream.
		*/
		GuidoStream * openStream ();

		/*!
			\brief Close a guido stream
			\param s a stream
			\return a Guido error code.
		*/
		GuidoErrCode closeStream (GuidoStream *s);

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

			\param s a stream previoulsy opened with openStream
			\param str a string containing a portion of gmn code
			\return a Guido error code.
		*/
		GuidoErrCode writeStream (GuidoStream *s, const std::string &str);

		/*!
			\brief Erase all stream content in order to reuse it

			\param s a stream previoulsy opened with openStream
			\return a Guido error code.
		*/
		GuidoErrCode resetStream (GuidoStream *s);

		/*! @} */
};
/*! @} */

#endif // GUIDOENGINEADAPTER_H
