#ifndef GUIDOFactory_H
#define GUIDOFactory_H

/*
	GUIDO Library
	Copyright (C) 2003, 2004  Grame

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 */

#include "GUIDOExport.h"
#include "GUIDOEngine.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
\addtogroup Factory GUIDO Factory
	The GUIDO Factory API provides a set of functions to create a GUIDO
    abstract representation from scratch and to convert it into a graphical representation.
    
    The GUIDO Factory is a state machine that operates on implicit current elements:
    for example, once you open a voice (\p GuidoFactoryOpenVoice()), it becomes the current
    voice and all subsequent created events are implicitly added to this current voice. 
    The elements of the factory state are:
    - the current score: modified by \p GuidoFactoryOpenMusic() and \p GuidoFactoryCloseMusic()
    - the current voice: modified by \p GuidoFactoryOpenVoice() and \p GuidoFactoryCloseVoice()
    - the current chord: modified by \p GuidoFactoryOpenChord() and \p GuidoFactoryCloseChord()
    - the current event: modified by \p GuidoFactoryOpenEvent() and \p GuidoFactoryCloseEvent()
    - the current tag: modified by \p GuidoFactoryOpenTag() and \p GuidoFactoryCloseTag()

    Some elements of the factory state reflects the GUIDO format specification:
    - the current register: modified by \p GuidoFactorySetRegister()
    - the current duration: modified by \p GuidoFactorySetDuration()
@{
*/

typedef void * ARFactoryHandler;


// ----------------------------------------------------------------------------
/** \brief Opens the Guido Factory.

    Must be called before any other call to the Guido Factory API.
    \return an integer that is an error code if not null.
*/
	GUIDOAPI(GuidoErrCode)	GuidoFactoryOpen( ARFactoryHandler * outFactory );

// ----------------------------------------------------------------------------
/** \brief Closes the Guido Factory.

    Must be called to release the factory associated resources.
*/
	GUIDOAPI(void)	GuidoFactoryClose( ARFactoryHandler inFactory );

// ----------------------------------------------------------------------------
/** \brief Creates and opens a new music score. 

    The function modifies the factory state: the new score becomes
    the current factory score. 
    It fails if a music score is already opened.
    A music score has to be closed using \p GuidoFactoryCloseMusic()
    \return an integer that is an error code if not null.
*/
	GUIDOAPI(GuidoErrCode)	GuidoFactoryOpenMusic( ARFactoryHandler inFactory );

// ----------------------------------------------------------------------------
/** \brief Closes the current music score.
	
    The function modifies the factory state if a music score is currently opened: 
    the current factory score is set to null.
    It fails if no music score is 
    opened.
	You must not have pending events nor pending voice at this point.
    
	The logicical music layout (conversion from abstract to abstract representation)
    is part of the function operations.
    Next, the caller is expected to call \p GuidoFactoryMakeGR() with the returned value
    in order to proceed with the graphical score layout.

    \return a GUIDO handler to the new AR structure, or 0.
	\sa \p GuidoFactoryMakeGR()
*/
	GUIDOAPI(ARHandler)	GuidoFactoryCloseMusic( ARFactoryHandler inFactory );

// ----------------------------------------------------------------------------
/*
\brief Converts an abstract music representation to a graphic music representation.

    Performs the graphical score layout: applies the spacing and page breaking
    algorithms.

    \return a GUIDO handler to the new GR structure, or 0.
	\sa \p GuidoMakeGRMusic()

	
	replaced by GuidoAR2GR 
*/
//	GUIDOAPI(GRHandler) GuidoFactoryMakeGR_Obsolete( ARHandler inHandleAR );

// ----------------------------------------------------------------------------
/** \brief Creates and opens a new voice.

    The function modifies the factory state: 
    the new voice becomes the current factory voice. 
    It fails if a voice is already opened.
    A voice has to be closed using \p GuidoFactoryCloseVoice()
    Voices are similar to sequence is GMN.

    \return an error code
*/
	GUIDOAPI(GuidoErrCode)	GuidoFactoryOpenVoice( ARFactoryHandler inFactory );

// ----------------------------------------------------------------------------
/** \brief Closes the current voice.
	
    The function modifies the factory state if a voice is currently opened: 
    the current factory voice is set to null.
    It fails if no voice is opened.
	You must not have pending events at this point.
    The voice is first converted to its normal form and next added to the current score.

    \return an error code
*/
	GUIDOAPI(GuidoErrCode)	GuidoFactoryCloseVoice( ARFactoryHandler inFactory );

// ----------------------------------------------------------------------------
/** \brief Creates and open a new chord.

    The function modifies the factory state: 
    the new chord becomes the current factory chord. 
    It fails if a chord is already opened.
    A chord has to be closed using \p GuidoFactoryCloseChord()

    \return an error code
*/
	GUIDOAPI(GuidoErrCode)	GuidoFactoryOpenChord( ARFactoryHandler inFactory );

// ----------------------------------------------------------------------------
/** \brief Closes the current chord.
	
    The function modifies the factory state if a chord is currently opened: 
    the current factory chord is set to null.
    It fails if no chord is opened.
    The chord is added to the current voice.
    \return an error code
*/
	GUIDOAPI(GuidoErrCode)	GuidoFactoryCloseChord( ARFactoryHandler inFactory );

// ----------------------------------------------------------------------------
/** \brief Begins a new chord note commata.

	Called to tell the factory that a new chord-voice
	 is beginning. This is important for the ranges that need to
	  be added (dispdur and shareStem)

\todo check semantic and rename
    \return an error code
*/
	GUIDOAPI(GuidoErrCode)	GuidoFactoryInsertCommata( ARFactoryHandler inFactory ); // is it a correct name ?

// ----------------------------------------------------------------------------
/** \brief Creates and opens a new event (note or rest).

    The function modifies the factory state: 
    the new event becomes the current factory event. 
    It fails if an event is already opened.
    An event has to be closed using \p GuidoFactoryCloseEvent()
    \return an error code
*/
	GUIDOAPI(GuidoErrCode)	GuidoFactoryOpenEvent( ARFactoryHandler inFactory, const char * inEventName );

// ----------------------------------------------------------------------------
/** \brief Closes the current event.

    The function modifies the factory state if an event is currently opened: 
    the current factory event is set to null.
    It fails if no event is opened.
    The event is added to the current voice.

    \return an error code
*/	
	GUIDOAPI(GuidoErrCode)	GuidoFactoryCloseEvent( ARFactoryHandler inFactory );

// ----------------------------------------------------------------------------
/** \brief Adds a sharp to the current event. 

    The current event must be a note.
    \return an error code
*/
	GUIDOAPI(GuidoErrCode)	GuidoFactoryAddSharp( ARFactoryHandler inFactory );

// ----------------------------------------------------------------------------
/** \brief Add a flat to the current event. 

    The current event must be a note.
	\return an error code.
*/
	GUIDOAPI(GuidoErrCode)	GuidoFactoryAddFlat( ARFactoryHandler inFactory );

// ----------------------------------------------------------------------------
/** \brief Sets the number of dots the current event.
    \param inFactory a handler to a Guido Factory (created with GuidoFactoryOpen)
    \param dots the number of dots to be carried by the current event.
	\return an error code.
*/
	GUIDOAPI(GuidoErrCode) 	GuidoFactorySetEventDots( ARFactoryHandler inFactory, int dots );

// ----------------------------------------------------------------------------
/** \brief Sets the accidentals of the current event.
    \param inFactory a handler to a Guido Factory (created with GuidoFactoryOpen)
    \param accident: positive values are used for sharp and negative values for flats 
	\return an error code.
*/
	GUIDOAPI(GuidoErrCode) 	GuidoFactorySetEventAccidentals( ARFactoryHandler inFactory, int accident );

// ----------------------------------------------------------------------------
/** \brief Sets the register (octave) of the current event.

	The current  event must be a note.
	The register becomes the current register ie  next notes will carry this
    register until otherwise specified.
	
    \param inFactory a handler to a Guido Factory (created with GuidoFactoryOpen)
    \param octave is an integer value indicating the octave of the note where 
    \e a1 is \e A 440Hz. All octaves start with the pitch class \e c.
	\return an error code.
*/
	GUIDOAPI(GuidoErrCode)	GuidoFactorySetOctave( ARFactoryHandler inFactory, int octave );

// ----------------------------------------------------------------------------
/** \brief Sets the duration of the current event.

	Durations are expressed as fractional value of a whole note: for example, 
    a quarter note duration is 1/4.
    The duration becomes the current duration ie  next notes will carry this
    duration until otherwise specified.
    \param inFactory a handler to a Guido Factory (created with GuidoFactoryOpen)
    \param numerator: the rational duration numerator
    \param denominator: the rational duration denominator
	\return an error code.
*/	
	GUIDOAPI(GuidoErrCode) 	GuidoFactorySetDuration( ARFactoryHandler inFactory, int numerator, int denominator );

// ----------------------------------------------------------------------------
/* \brief Add a tag to the current voice.

    \param inFactory a handler to a Guido Factory (created with GuidoFactoryOpen)
    \param name: the tag name
    \param tagID: is the number that the parser generates for advanced GUIDO
	\return an error code.
*/
	GUIDOAPI(GuidoErrCode) 	GuidoFactoryOpenTag( ARFactoryHandler inFactory, const char * name, long tagID );

// ----------------------------------------------------------------------------
/* \brief Add a tag to the current voice.

	\param inFactory a handler to a Guido Factory (created with GuidoFactoryOpen)
	\param name: the tag name
    \param tagID: is the number that the parser generates for advanced GUIDO
	\return an error code.
*/
	GUIDOAPI(GuidoErrCode) 	GuidoFactoryOpenRangeTag( ARFactoryHandler inFactory, const char * name, long tagID );


// ----------------------------------------------------------------------------
/** \brief Indicates the end of a range tag.

    The function is applied to the current tag.
    It must be called when the end of a tag's range
	has been reached. 
    \param inFactory a handler to a Guido Factory (created with GuidoFactoryOpen)
	\return an error code.
*/
	GUIDOAPI(GuidoErrCode) 	GuidoFactoryEndTag( ARFactoryHandler inFactory );

// ----------------------------------------------------------------------------
/** \brief Closes the current tag.

    The function is applied to the current tag.
    Must be called after parameter and before the range.
\n  With the following examples:
- tag<1,2,3>(c d e ) 	: call the function before parsing c
- tag<1,2> c d          : call the function before parsing c

    \param inFactory a handler to a Guido Factory (created with GuidoFactoryOpen)
	\return an error code.
*/
	GUIDOAPI(GuidoErrCode) 	GuidoFactoryCloseTag( ARFactoryHandler inFactory );

// ----------------------------------------------------------------------------
/** \brief Adds a new string parameter to the current tag.

    \param inFactory a handler to a Guido Factory (created with GuidoFactoryOpen)
    \param val: the string parameter value
	\return an error code.
*/
	GUIDOAPI(GuidoErrCode) 	GuidoFactoryAddTagParameterString( ARFactoryHandler inFactory, const char * val );

// ----------------------------------------------------------------------------
/** \brief  Adds a new integer parameter to the current tag.

    \param inFactory a handler to a Guido Factory (created with GuidoFactoryOpen)
    \param val: the parameter value
	\return an error code.
*/
	GUIDOAPI(GuidoErrCode) 	GuidoFactoryAddTagParameterInt( ARFactoryHandler inFactory, int val );

// ----------------------------------------------------------------------------
/** \brief Adds a new floating-point parameter to the current tag.

    \param inFactory a handler to a Guido Factory (created with GuidoFactoryOpen)
    \param val: the parameter value
	\return an error code.
*/
	GUIDOAPI(GuidoErrCode) 	GuidoFactoryAddTagParameterFloat( ARFactoryHandler inFactory, double val );

// ----------------------------------------------------------------------------
/** \brief Defines the name (when applicable) of the last added tag-parameter

    \param inFactory a handler to a Guido Factory (created with GuidoFactoryOpen)
    \param name: the tag parameter name
	\return an error code.
*/
	GUIDOAPI(GuidoErrCode) 	GuidoFactorySetParameterName( ARFactoryHandler inFactory, const char * name );

// ----------------------------------------------------------------------------
/** \brief Defines the unit of the last added tag-parameter

    \param inFactory a handler to a Guido Factory (created with GuidoFactoryOpen)
    \param unit: a string defining the unit. The following units are supported:
- \p m - meter
- \p cm - centimeter
- \p mm - millimeter
- \p in - inch
- \p pt - point (= 1/72.27 inch)
- \p pc - pica (= 12pt)
- \p hs - halfspace (half of the space between  two lines of the current staff)
- \p rl - relative measure in percent  (used for positioning on score page) 
	\return an error code.
*/
	GUIDOAPI(GuidoErrCode) 	GuidoFactorySetParameterUnit( ARFactoryHandler inFactory, const char * unit );
/*! @} */


#ifdef __UseOldGUIDOFactory__
// ----------------------------------------------------------------------------
/** \brief Add a tag to the current voice.

    \param inFactory a handler to a Guido Factory (created with GuidoFactoryOpen)
    \param name: the tag name
    \param tagID: is the number that the parser generates for advanced GUIDO ?????
	\return an error code.
*/
	GUIDOAPI(GuidoErrCode) 	GuidoFactoryCreateTag( ARFactoryHandler inFactory, const char * inTagName, long tagID );

// ----------------------------------------------------------------------------
/** \brief Indicates that the current tag is a range tag.
    \param inFactory a handler to a Guido Factory (created with GuidoFactoryOpen)
	\return an error code.
*/	
	GUIDOAPI(GuidoErrCode) 	GuidoFactoryIsRangeTag( ARFactoryHandler inFactory );

#endif

#ifdef __cplusplus
}
#endif

#endif
