#ifndef __MIDI2GUIDO__
#define __MIDI2GUIDO__

/*
  GUIDO Library
  Copyright (C) 2014  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GUIDOEngine.h"

#include <map>

/*!
\addtogroup midi MIDI support
@{
*/

#ifdef __cplusplus
extern "C" {
#endif

class MIDI2GMNConverter;

	/**	\brief Import a MIDI file

		\param ar: Guido AR handler
		\param filename: the MIDI file file name
		\param tracks: a list of tracks to be converted. Multiple tracks are converted into separate voices.
		       The value "all" convert all the tracks. A range is specified in the form "n-m". 
			   A list is a comma separated list of integers.
		\return a Guido error code.
	*/
// voir pour un préambule
	GUIDOAPI GuidoErrCode GuidoMIDIFile2AR (const char* filename, int tracknum, std::string& outgmn);


// voir pour un préambule
	/**	\brief Creates a midi 2 guido converter
		\return a converter.
		
		A midi converter is designed to handle real-time midi streams. It should be release using GuidoMIDI2GMNStop.
	*/
	GUIDOAPI MIDI2GMNConverter* GuidoMIDI2GMNStart ();

	/**	\brief Closes a midi 2 guido converter
		\param converter: a converter previously created with GuidoMIDI2GMNStart
		\param date: a time expressed in midi ticks (mls could be used with the default tpqn)
	*/
	GUIDOAPI std::string GuidoMIDI2GMNStop (MIDI2GMNConverter* converter, int date);

	/**	\brief Converts a MIDI pitch to GMN
		\param converter: a converter previously created with GuidoMIDI2GMNStart
		\param date: a time expressed in midi ticks (mls could be used with the default tpqn)
		\param pitch: a MIDI pitch
	*/
	GUIDOAPI std::string GuidoMIDINoteOn2GMN (MIDI2GMNConverter* converter, int date, int pitch);

	/**	\brief Fix the duration of the note previously opened with GuidoMIDINoteOn2GMN
		\param converter: a converter previously created with GuidoMIDI2GMNStart
		\param date: a time expressed in midi ticks (mls could be used with the default tpqn)
		\param pitch: a MIDI pitch
	*/
	GUIDOAPI std::string GuidoMIDINoteOff2GMN (MIDI2GMNConverter* converter, int date, int pitch);

#ifdef __cplusplus
}
#endif

/*! @} */

#endif
