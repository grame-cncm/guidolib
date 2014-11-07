#ifndef __GUIDO2MIDI__
#define __GUIDO2MIDI__

/*
  GUIDO Library
  Copyright (C) 2011  Grame

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



/** \brief The GuidoInitDesc data structure contains all information
	required by GuidoInit()
*/
typedef struct Guido2MidiParams
{
	int		fTempo;				///< default tempo in quarter per minute - default value: 120
	int		fTicks;				///< ticks per quarternote - default value: 960 (64*3*5)
	int		fChan;				///< the default Midi channel - default value: 1

	float	fIntensity;			///< default intensity [0.0 ... 1.0] - default value: 0.8
	float	fAccentFactor;		///< accent intensity factor - default value: 1.1
	float	fMarcatoFactor;		///< marcato intensity factor - default value: 1.2

	float	fDFactor;			///< default duration factor [0.0 ... 1.0] - default value: 0.8
	float	fStaccatoFactor;	///< staccato duration factor - default value: 0.5
	float	fSlurFactor;		///< legato duration factor  - default value: 1.0
	float	fTenutoFactor;		///< tenuto duration factor  - default value: 0.90
	float	fFermataFactor;		///< fermata duration factor - default value: 2.0
	
	std::map<int, int> fVChans;	///< a map between voice numbers and MIDI channels (all indexed from 1)

} Guido2MidiParams;

#ifdef __cplusplus
extern "C" {
#endif


	/**	\brief Export to a MIDI file

		\param ar: Guido AR handler
		\param filename: the output file name
		\param params: conversions parameters.
		\return a Guido error code.
	*/
	GUIDOAPI(GuidoErrCode) GuidoAR2MIDIFile(const ARHandler ar, const char* filename, const Guido2MidiParams* params);

#ifdef __cplusplus
}
#endif

/*! @} */

#endif
