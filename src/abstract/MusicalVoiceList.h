#ifndef MusicalVoiceList_H
#define MusicalVoiceList_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

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
//#include "ARMusicalVoiceState.h"
//#include "kf_ilist.h"

/** \brief A list of musical voices
*/

// typedef CList<ARMusicalVoice*,ARMusicalVoice*> MusicalVoiceList;
class ARMusicalVoice;
template <class T> class KF_IPointerList;
typedef KF_IPointerList<ARMusicalVoice> MusicalVoiceList;



#endif


