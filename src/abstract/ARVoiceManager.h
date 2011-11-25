#ifndef ARVoiceManager_H
#define ARVoiceManager_H

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

#include "ARMusicalVoiceState.h"

class ARMusic;
class ARMusicalVoice;

/** \brief This class is used to handle preliminary break-detection. 

	The voice is traversed with the Iterate Function note the parallel 
	version of the GRvoice-manager which really creates graphical elements
	 whereas this function is only for traversal.
*/

class ARVoiceManager
{
		friend class ARMusic;

	public:

		enum {  CURTPBIGGER_ZEROFOLLOWS = 1,
				CURTPBIGGER_EVFOLLOWS = 2,
				DONE_EVFOLLOWS = 3,
				DONE_ZEROFOLLOWS = 4,
				DONE = 5,
				MODEERROR = 6,
				NEWSYSTEM = 7,
				NEWPAGE = 8,
				ENDOFVOICE = 9 };

						ARVoiceManager( ARMusicalVoice * arVoice );
		virtual 		~ARVoiceManager();

		
				float CheckBreakPosition( const TYPE_TIMEPOSITION & tp );
				int InsertBreak( const TYPE_TIMEPOSITION &tp, int breaktype,float value = 0.0f );


				int Iterate( TYPE_TIMEPOSITION &tp, int filltagmode );



		ARMusicalVoiceState mCurrVoiceState;
		TYPE_TIMEPOSITION mLastBreakTimePos;

	protected:

		ARMusicalVoice * mVoice;

};

#endif
