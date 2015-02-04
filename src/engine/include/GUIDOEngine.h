#ifndef GUIDOEngine_H
#define GUIDOEngine_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003, 2004  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <ostream>
#include <string>
#include <vector>
#include "GUIDOExport.h"


#ifdef WIN32
    #define GUIDOAPI_deprecated
#else
    #define GUIDOAPI_deprecated __attribute__((deprecated))
#endif


class VGDevice;

struct NodeAR;
struct NodeGR;
typedef struct NodeAR *  ARHandler;
typedef struct NodeGR *  GRHandler;
typedef const struct NodeAR *  CARHandler;
typedef const struct NodeGR *  CGRHandler;

/** \brief The GuidoInitDesc data structure contains all information
	required by GuidoInit()
*/
struct GuidoInitDesc
{
	//! a graphic device pointer, if null a default device is used
	VGDevice	*		graphicDevice;

    void *              reserved;

	//! the music font name, defaults to "guido" font when null
	const char *		musicFont;
	//! a text font name, defaults to "times" font when null
	const char *		textFont;
};

/** \brief A structure to keep information about clipping and redrawing regions.
*/
struct GPaintStruct
{
    //! a flag to ignore the following rect and to redraw everything
    bool	erase;
    //! Absolute Guido virtual coordinates of the clipping rectangle.
    //! Only systems that intersect with this rectangle will be drawn.
    int		left;
	int		top;
	int		right;
	int		bottom;
};


/*! @} */

/**
    A Guido date is expressed as a fractional value where 1/1 represents
    the whole note.
*/
typedef struct
{
	//! the date numerator
    int num;
	//! the date denominator
    int denom;

} GuidoDate;

/** \brief Contains all graphic-related information required by GuidoOnDraw()

	Used to render a page of score into a device context.
*/
struct GuidoOnDrawDesc
{
	//! A Guido handler to a graphic representation
    GRHandler handle;

    //! A graphic device context.
    VGDevice * hdc;
	//! The page number. Starts from 1.
    int page;
    //! Indicates what to (re)draw.
	GPaintStruct updateRegion;

	/*! Indicates the coordinates of the score point that will appear at the graphic
        origin. Typical values are 0. Non null values have the effect of moving a window
        over the score page, like scroll bars that move a page view.
        Units are internal units.
    */
	int scrollx, scrolly;

	/*! Indicates the size of the drawing zone. The size is expressed
        in graphic device units (pixels for a screen for example)
    */
    float reserved;

    int sizex, sizey;

	//! If true, the engine ignores scroll, zoom and sizes parameters.
    //! If false, the engine draws a white background in the graphic device.
    int isprint;
};


/** \brief Bounding boxes drawing control constants.
*/
enum  {	kNoBB, kPageBB, 
		kSystemsBB = 2, 
		kSystemsSliceBB = 4,
		kStavesBB = 8, 
		kMeasureBB = 0x10, 
		kEventsBB = 0x20,
};


enum GuidoInternalDevice
{
  guido_svg_with_font_spec = 0,
  guido_abstract = 1,
  guido_binary = 2
};

/*!
\addtogroup Errors Error codes
@{
*/
/** \brief The guido error codes list.

	This is the list of possible error codes returned by GuidoEngine and
	GuidoFactory APIs
*/
enum GuidoErrCode
{
	//! null is used to denote no error
    guidoNoErr					= 0,
	//! error while parsing the Guido format
	guidoErrParse				= -1,
	//! memory allocation error
	guidoErrMemory				= -2,
	//! error while reading or writing a file
	guidoErrFileAccess			= -3,
	//! the user cancelled the action
	guidoErrUserCancel			= -4,
	//! the music font is not available
	guidoErrNoMusicFont			= -5,
	//! the text font is not available
	guidoErrNoTextFont			= -6,
	//! bad parameter used as argument
	guidoErrBadParameter		= -7,
	//! invalid handler used
	guidoErrInvalidHandle		= -8,
	//! required initialisation has not been performed
	guidoErrNotInitialized		= -9,
	//! the action failed
	guidoErrActionFailed		= -10
};
/*! @} */


/** \brief Mapping mode for SVG export
*/
enum {
    kNoMapping     =  0,
    kVoiceMapping  =  1,
    kStaffMapping  =  1<<1,
    kSystemMapping =  1<<2
};


enum { kAutoDistrib = 1, kAlwaysDistrib = 2, kNeverDistrib = 3 };

/**
    Settings for the graphic score layout.
*/
typedef struct GuidoLayoutSettings
{
    /*! Control distance between systems, distance is in internal units
		(default value: 75)
	*/
	float systemsDistance;

    /*! control systems distribution.
		Possible values: kAutoDistrib (default), kAlwaysDistrib, kNeverDistrib
	*/
	int systemsDistribution;

	/*! Maximum distance allowed between two systems, for automatic distribution mode.
		Distance is relative to the height of the inner page.
		Default value: 0.25 (that is: 1/4 of the page height)
	*/
	float systemsDistribLimit;

	/*! force value of the Space-Force function
        typical values range from 400 to 1500. Default value: 750
    */
	float   force;

    /*! the spring parameter
        typical values range from 1 to 5. Default value: 1.1
    */
    float   spring;

    /** boolean value to tell the engine to use the Neighborhood spacing algorithm or not
  		(default value: 0)
	*/
	 int neighborhoodSpacing;

	/** boolean value to tell the engine to use the optimal page fill algorithm or not
		(default value: 1)
	*/
	int optimalPageFill;

    /** boolean value to tell the engine to resize page to music
		(default value: 1)
	*/
	int resizePage2Music;

    /** float value to tell the engine what is the force multiplicator applied to proportional rendering
        If value is 0, proportional mode is not enabled, otherwise value corresponds to the force multiplicator value
		(default value: 0)
	*/
	float proportionalRenderingForceMultiplicator;
	
} GuidoLayoutSettings;

/**
    The page format parameters

	Page format should be given in internal units. To convert from cm or inches
	you should use \c GuidoCM2Unit or \c GuidoInches2Unit
 */
typedef struct
{
    float width;
    float height;
    float marginleft;
    float margintop;
    float marginright;
    float marginbottom;
} GuidoPageFormat;


/**
	The Graphic representation parameters.

	This data structure contains a GuidoLayoutSettings and a GuidoPageFormat.
  */
typedef struct
{
	GuidoLayoutSettings layoutSettings;
	GuidoPageFormat pageFormat;
} GuidoGrParameters;

#ifdef __cplusplus
extern "C" {
#endif

/*!
\addtogroup Engine Building abstract and graphic representations
The Guido Engine operates on two kinds of music representation:
- an abstract representation that corresponds to the Guido Music Notation elements.
An important part of the layout - the logical layout, that decides on the stems direction
for example - is applied to the abstract representation.
- a graphic representation that corresponds to a graphic instantiation of an
abstract representation. Algorithms such as spacing and line breaking are applied
to the graphic representation.

The functions described in this section are intended to build abstract and graphic
representations.
@{
*/
	/*!
        Initialises the Guido Engine. Must be called before any attempt
        to read a Guido file or to use the Guido Factory
        \param desc the graphic environment description.
        \return a Guido error code.

	  	WARNING: the caller must ensure desc maintains a constant reference on a
        valid VGDevice, because Guido keeps it internally (to calculate fonts, etc.)
    */
    GUIDOAPI(GuidoErrCode)	GuidoInit(GuidoInitDesc * desc);

	/*!
        Initialises the Guido Engine with an independent SVG device, avoiding
        the need to pass a description. Useful for contexts (like javascript)
        where structures cannot be easily manipulated.
    */
    GUIDOAPI(GuidoErrCode)      GuidoInitWithIndependentSVG();
	/*!
        Guido Engine shutdown
		
        Actually release the font allocated by the engine. 
		Anyway, the fonts are release when the client application exit but
		the function provides control over the time of the release.
    */
    GUIDOAPI(void)	GuidoShutdown();

	/*!
        Parses a Guido Music Notation (.gmn) file and builds the corresponding
        abstract representation.
		\param filename the file to parse.
		\param ar on output: a Guido opaque handle to an abstract music representation.
                It's the caller responsability to free the handle using GuidoFreeAR.
		\return a Guido error code.
    */

#ifdef WIN32
    __declspec(deprecated("Deprecated function (will be erased soon) : use GUIDOAPI(ARHandler) GuidoFile2AR (GuidoParser *parser, const char * file) instead."))
#endif
    GUIDOAPI(GuidoErrCode)	GuidoParseFile(const char * filename, ARHandler* ar) GUIDOAPI_deprecated;

	/*!
        Parses a buffer and builds the corresponding abstract representation.
		The buffer if expected to contain gmn code.
		\param str the null terminated buffer to parse.
		\param ar on output: a Guido opaque handle to an abstract music representation.
                It's the caller responsability to free the handle using GuidoFreeAR.
		\return a Guido error code.
    */
#ifdef WIN32
    __declspec(deprecated("Deprecated function (will be erased soon) : use GUIDOAPI(ARHandler) GuidoString2AR (GuidoParser *parser, const char * str) instead."))
#endif
    GUIDOAPI(GuidoErrCode)	GuidoParseString(const char * str, ARHandler* ar) GUIDOAPI_deprecated;

	/*!
        Transforms a Guido abstract representation into a Guido graphic representation.
        The engine applies layout algorithms according to the settings given as argument.

		\note You can safely free the AR after the transformation.

		\param ar the handler to the abstract representation.
		\param settings a pointer to the settings for the graphic layout. If null, default
        settings are applied.
		\param gr on output: a Guido opaque handle to a graphic music representation
                It's the caller responsability to free the handle using GuidoFreeGR.
		\return a Guido error code.
    */
    GUIDOAPI(GuidoErrCode)	GuidoAR2GR(ARHandler ar, const GuidoLayoutSettings* settings, GRHandler* gr);

	/*!
	 * \brief GuidoAR2GRParameterized. Transforms a Guido abstract representation into a Guido graphic representation.
	 * It use given GuidoGrParameters with a layout settings and a page format. If a page format is used in gmn code,
	 * it has priority over the GuidoGrParameters's page format.
	 * \param ar a Abstract representation handler.
	 * \param gp a GuidoGrParameters structure. If null default layout settings and page format are used.
	 * \return a Graphic representation handler or null if an error occurs.
	 */
	GUIDOAPI(GRHandler) GuidoAR2GRParameterized(ARHandler ar, const GuidoGrParameters* gp);

	/*!
		Applies new layout settings to an existing Guido graphic representation.
		\param gr the handler to the graphic representation.
		\param settings a pointer to the settings for the graphic layout. If null, default
        settings are applied.
		\return a Guido error code.
    */
    GUIDOAPI(GuidoErrCode)	GuidoUpdateGR( GRHandler gr, const GuidoLayoutSettings* settings);

	/*!
		Applies new layout settings and page format to an existing Guido graphic representation.
		\param gr the handler to the graphic representation.
		\param settings a pointer to the settings for the graphic representation. If null, default
		settings are applied.
		\return a Guido error code.
	*/
	GUIDOAPI(GuidoErrCode)	GuidoUpdateGRParameterized( GRHandler gr, const GuidoGrParameters* settings);

	/*!
        Releases a Guido abstract representation.
		\param ar the handler to the abstract representation.
    */
    GUIDOAPI(void)	GuidoFreeAR (ARHandler ar);

	/*!
        Releases a Guido graphic representation.
		\param gr the handler to the graphic representation.
    */
    GUIDOAPI(void)	GuidoFreeGR (GRHandler gr);

	/*!
		Gives a textual description of a Guido error code.

		\param errCode a Guido error code.
		\return a string describing the error.
	*/
	GUIDOAPI(const char *) GuidoGetErrorString( GuidoErrCode errCode );

	/*!
        Gives the line of a Guido script where the last parse error has occured.
		\return a line number.
	*/

#ifdef WIN32
    __declspec(deprecated("Deprecated function (will be erased soon) : use GUIDOAPI(GuidoErrCode) GuidoParserGetErrorCode (GuidoParser* p, int& line, int& col) instead."))
#endif
    GUIDOAPI(int)   GuidoGetParseErrorLine() GUIDOAPI_deprecated;

	/*!
        Gives the default values of the layout settings.

		\param settings on output, a pointer to the settings to be filled with default values.
    */
    GUIDOAPI(void)	GuidoGetDefaultLayoutSettings (GuidoLayoutSettings *settings);
/*! @} */


/*!
\addtogroup Pages Browsing music pages

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
	GUIDOAPI(int) 	GuidoCountVoices( CARHandler inHandleAR );
	
	/** \brief Gives the number of score pages of the graphic representation.

		\param inHandleGR a Guido opaque handle to a GR structure.
		\return a number of pages or a guido error code.
	*/
	GUIDOAPI(int) 	GuidoGetPageCount( CGRHandler inHandleGR );
	
	/** \brief Gives the number of systems on a given page.

		\param inHandleGR a Guido opaque handle to a GR structure.
		\param page a page number (starts at 1).
		\return the systems count on the given page or a guido error code.
	*/
	GUIDOAPI(int) 	GuidoGetSystemCount( CGRHandler inHandleGR, int page );

	/** \brief Returns the music duration of a score.

		The duration is expressed as a fractional value where 1 represents
        a whole note.

		\param inHandleGR a Guido opaque handle to a GR structure.
		\param date on output: the duration expressed as a fractional value
		\return a Guido error code.
	*/
	GUIDOAPI(GuidoErrCode)	GuidoDuration( CGRHandler inHandleGR, GuidoDate * date );

	/** \brief Finds the page which has an event (note or rest) at a given date.

		\bug returns page + 1 when input date falls on the last system.
		\param inHandleGR a Guido opaque handle to a GR structure.
		\param date the target date.
		\return a page number if greater than 0,
                0 if no page found,
	*/
	GUIDOAPI(int)	GuidoFindEventPage( CGRHandler inHandleGR, const GuidoDate& date );

	/** \brief Finds the page which contain a given date.

		\bug returns page + 1 when input date falls on the last system.
		\param inHandleGR a Guido opaque handle to a GR structure.
		\param date the target date.
		\return a page number if greater than 0,
                0 if no page found,
	*/
	GUIDOAPI(int) GuidoFindPageAt( CGRHandler inHandleGR, const GuidoDate& date );

	/** \brief Gives the time location of a Page.

		\param inHandleGR a Guido opaque handle to a GR structure.
		\param pageNum a page number (starts at 1).
		\param date on output: the page date if the page number is valid
		\return a Guido error code.
	*/
	GUIDOAPI(GuidoErrCode) GuidoGetPageDate( CGRHandler inHandleGR, int pageNum, GuidoDate* date);
/*! @} */


/*!
\addtogroup Format Score drawing and pages formating
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
    GUIDOAPI(GuidoErrCode) 	GuidoOnDraw( GuidoOnDrawDesc * desc );

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
	GUIDOAPI(GuidoErrCode) GuidoGR2SVG( const GRHandler handle, int page, std::ostream& out, bool embedFont, const char* font, const int mappingMode = 0 );

	/** \brief Exports one page of score to SVG.

		\param handle a graphic representation.
		\param page the page number.
		\param out the output stream.
		\param fontfile path of the guido svg font file.
		\param mappingMode the mapping mode (see mapping mode enum).
		\return a Guido error code
	*/
	#ifdef WIN32
		__declspec(deprecated("Deprecated function (will be erased soon) : use GUIDOAPI(GuidoErrCode) GuidoGR2SVG( const GRHandler handle, int page, std::ostream& out, bool embedFont, const char* font, const int mappingMode = 0 ) instead."))
	#endif
	GUIDOAPI(GuidoErrCode) 	GuidoSVGExport( const GRHandler handle, int page, std::ostream& out, const char* fontfile, const int mappingMode = 0 ) GUIDOAPI_deprecated;

    /** \brief Exports one page of score to SVG.
	 *  If fontfile or fontspec are set, the font is added to svg. The fontfile has priority over the fontspec.

		\param handle a graphic representation.
        \param page the page number.
        \param out the output stream.
        \param fontfile path of the guido svg font file.
		\param fontspec an svg font in text representation.
		\param mappingMode the mapping mode (see mapping mode enum).
        \return a Guido error code
    */
	#ifdef WIN32
		__declspec(deprecated("Deprecated function (will be erased soon) : use GUIDOAPI(GuidoErrCode) GuidoGR2SVG( const GRHandler handle, int page, std::ostream& out, bool embedFont, const char* font, const int mappingMode = 0 ) instead."))
	#endif
	GUIDOAPI(GuidoErrCode) 	GuidoSVGExportWithFontSpec( const GRHandler handle, int page, std::ostream& out, const char* fontfile, const char* fontspec, const int mappingMode = 0 ) GUIDOAPI_deprecated;

	/** \brief Exports an abstract representation of GUIDO draw commands.

		\param handle a graphic representation.
		\param page the page number.
		\param out the output stream.
		\return a Guido error code
	*/
    GUIDOAPI(GuidoErrCode) 	GuidoAbstractExport( const GRHandler handle, int page, std::ostream& out);

	/** \brief Exports an representation of GUIDO draw commands in a data-reduced dsl

		\param handle a graphic representation.
		\param page the page number.
		\param out the output stream.
		\return a Guido error code
	*/
    GUIDOAPI(GuidoErrCode) 	GuidoBinaryExport( const GRHandler handle, int page, std::ostream& out);

	/** \brief Control bounding boxes drawing.

		\param bbMap a bits field indicating the set of bounding boxes to draw (default to none).
	*/
	GUIDOAPI(void) 	GuidoDrawBoundingBoxes(int bbMap);

	/** \brief Gives bounding boxes drawing state.
	*/
	GUIDOAPI(int) 	GuidoGetDrawBoundingBoxes();

	/** \brief Gives a score page format.

		\param inHandleGR a Guido opaque handle to a GR structure.
		\param pageNum a page number.
		\param format on output: the page format
	*/
	GUIDOAPI(void) 	GuidoGetPageFormat(	CGRHandler inHandleGR, int pageNum, GuidoPageFormat* format );

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
	GUIDOAPI(void) 	GuidoSetDefaultPageFormat( const GuidoPageFormat* format);

	/** \brief Gives the default score page format.

		\param format on output: the page format
	*/
	GUIDOAPI(void) 	GuidoGetDefaultPageFormat( GuidoPageFormat* format );

	/** \brief Converts internal Guido units into centimeters.

		\param val the value to be converted
		\return the converted value
	*/
	GUIDOAPI(float) 	GuidoUnit2CM(float val);

	/** \brief Converts centimeters into internal Guido units.

		\param val the value to be converted
		\return the converted value
	*/
	GUIDOAPI(float) 	GuidoCM2Unit(float val);

	/** \brief Converts internal Guido units into inches.

		\param val the value to be converted
		\return the converted value
	*/
	GUIDOAPI(float) 	GuidoUnit2Inches(float val);

	/** \brief Converts inches into internal Guido units.

		\param val the value to be converted
		\return the converted value
	*/
	GUIDOAPI(float) 	GuidoInches2Unit(float val);


	/** \brief Resize the page sizes to the music size.

		\param inHandleGR a Guido opaque handle to a GR structure.
		\return a Guido error code.
	*/
    GUIDOAPI(GuidoErrCode) 	GuidoResizePageToMusic( GRHandler inHandleGR );


/*! @} */

/*!
\addtogroup Misc Miscellaneous
Includes various functions for version management and for conversions.
The number of version functions is due to historical reasons.
@{
*/
	/**	\brief Gives the library version number as three integers

        Version number format is  MAJOR.MINOR.SUB

		\param major on output: the major revision number.
		\param minor on ouput: the minor revision number.
		\param sub on ouput: the sub revision number.
		\return a Guido error code.
	*/
	GUIDOAPI(void) GuidoGetVersionNums(int * major, int * minor, int * sub);

	/**	\brief Gives the library version number as a string
		\return the version numebr as a string.
	*/
	GUIDOAPI(const char*) GuidoGetVersionStr();


	/**	\brief Checks a required library version number.

		\param major the major revision number.
		\param minor the minor revision number.
		\param sub the sub revision number.
		\return noErr if the library version number is greater or equal to the version number
			passed as argument.
		\return otherwise guidoErrActionFailed.
	*/
	GUIDOAPI(GuidoErrCode) GuidoCheckVersionNums(int major, int minor, int sub);


	/** \brief Gives the distance between two staff lines.

		This value is constant (= 50). It does not depend on the context, it will
		probably never change in future versions of the library.

		\return the distance between two lines of staff, in Guido internal units.
	*/
	GUIDOAPI(float) GuidoGetLineSpace();


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
	GUIDOAPI(GuidoErrCode) GuidoMarkVoice( ARHandler inHandleAR, int voicenum,
											const GuidoDate & date, const GuidoDate & duration,
											unsigned char red, unsigned char green, unsigned char blue );


    /**	\brief Makes the correspondance between an ARMusic and a path.

		\param inHandleAR the destination ARHandler.
		\param inPaths the path to associate.
		\return noErr if the association has been made with success
		\return otherwise guidoErrActionFailed.
	*/
    GUIDOAPI(GuidoErrCode) GuidoSetSymbolPath(ARHandler inHandleAR, const std::vector<std::string> &inPaths);


    /**	\brief Returns the path corresponding to an AR.

		\param inHandleAR the handle given to extract its path.
		\param inPathVector the vector to be filled
		\return the returned path.
        \return noErr if the association has been made with success
		\return otherwise guidoErrActionFailed.
	*/
    GUIDOAPI(GuidoErrCode) GuidoGetSymbolPath(const ARHandler inHandleAR, std::vector<std::string> &inPathVector);

    /*! @} */


/*!
\addtogroup time Timing measurements
Includes functions to query the time spent by the main Guido Engine operations.
@{
*/
    /*!
		\brief Gets parsing time
		\param ar the ar handler given to extract the parsing time
		\return the time spent on building the AR representation (in msl) or -1 for invalid handlers
	*/
    GUIDOAPI(long)  GuidoGetParsingTime (const ARHandler ar);

    /** \brief Gets AR to GR procedure time

		\param gr the gr handler given to extract the AR2GR time
		\return the time spent to convert the AR representation to GR (in msl) or -1 for invalid handlers
	*/
    GUIDOAPI(long) 	GuidoGetAR2GRTime(const GRHandler gr);

    /** \brief Gets GR drawing procedure time

		\param gr the gr handler given to extract the drawing time
		\return the time spent on the last OnDraw call (in msl) or -1 if OnDraw has not yet been called or for invalid handlers
	*/
    GUIDOAPI(long) 	GuidoGetOnDrawTime(const GRHandler gr);

/*! @} */

    void AddGGSOutput(const char *s);
void AddGuidoOutput(const char *s);
#ifdef __cplusplus
}
#endif


#endif	/* GUIDOEngine_H */
