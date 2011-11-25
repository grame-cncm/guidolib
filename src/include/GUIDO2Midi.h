#ifndef __GUIDO2MIDI__
#define __GUIDO2MIDI__

/*
	GUIDO Library
	Copyright (C) 2011  Grame

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

#include "GUIDOEngine.h"

#include <map>

/*!
\addtogroup midi MIDI Export
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

#ifdef MIDIEXPORT

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


#endif  // MIDIEXPORT
#endif
