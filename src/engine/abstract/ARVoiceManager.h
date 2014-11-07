#ifndef ARVoiceManager_H
#define ARVoiceManager_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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

		
				float CheckBreakPosition(const TYPE_TIMEPOSITION & tp) const;
				int InsertBreak( const TYPE_TIMEPOSITION &tp, int breaktype,float value = 0.0f );


				int Iterate( TYPE_TIMEPOSITION &tp, int filltagmode );



		ARMusicalVoiceState mCurrVoiceState;
		TYPE_TIMEPOSITION mLastBreakTimePos;

	protected:

		ARMusicalVoice * mVoice;

};

#endif
