#ifndef ARMusic_H
#define ARMusic_H

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

#include "MusicalVoiceList.h"
#include "ARMusicalEvent.h"
#include "ARMusicalVoiceState.h"// for MusicalVoiceList
#include "GUIDOScoreMap.h"
#include "kf_ilist.h"	// for MusicalVoiceList

class ARMusic;

extern ARMusic * gCurArMusic;

/** \brief Translation of a GUIDO Segment into a class.
*/

class ARMusic : public MusicalVoiceList, public ARMusicalEvent 
{
  public:

      					ARMusic();
     virtual 			~ARMusic();

	  		void 	MarkVoice(int voicenum,int fromnum,
		  					int fromdenom,int lengthnum,int lengthdenom);
	 		void 	MarkVoice(int voicenum,float from,float length);
	  		void 	doAutoStuff();


      		GuidoPos AddTail(ARMusicalVoice * newMusicalVoice);

      		void 	adjustDuration(TYPE_DURATION newDuration);

	virtual void 	print() const;
	virtual std::ostream & operator<<(std::ostream & os) const;
	virtual std::ostream & output(std::ostream & os, bool isauto = true) const;

	virtual void 	resetGRRepresentation();
	virtual void	getTimeMap (TimeMapCollector& f) const;

			void 	removeAutoTags();

			const NVstring & getName() const	{ return mName; }
	  		void 	setName(const char * in)	{ mName = in; }

	  long mMaxTagId;

	static int mRefCount;

  protected:

  	  void doAutoBreaks();

	  NVstring mName;
};

#endif



