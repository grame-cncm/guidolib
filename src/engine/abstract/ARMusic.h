#ifndef ARMusic_H
#define ARMusic_H

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

	  		void 	MarkVoice(int voicenum,int fromnum, int fromdenom, int lengthnum, int lengthdenom, unsigned char red, unsigned char green, unsigned char blue);
	 		void 	MarkVoice(int voicenum, float from, float length, unsigned char red, unsigned char green, unsigned char blue);
	 		int 	countVoices() const;
	  		void 	doAutoStuff();

      		GuidoPos AddTail(ARMusicalVoice * newMusicalVoice);

      		void 	adjustDuration(TYPE_DURATION newDuration);

	virtual void 	resetGRRepresentation();
	virtual void	getTimeMap (TimeMapCollector& f) const;

			void 	removeAutoTags();

    const NVstring  & getName() const { return mName; }
    void  setName(const char * in)    { mName = in; }
    
    const std::vector<std::string> &getPath() const  { return mPaths; }  
    void  setPath( std::vector<std::string> inPaths) { mPaths = inPaths; }

    void setParseTime(long time) { mParseTime = time; }
    long  getParseTime()         { return mParseTime; }

    long       mMaxTagId;
    static int mRefCount;

    /* Visitor design pattern */
    void goThrough(BaseVisitor *visitor) const;

protected:

    void doAutoBreaks();

    NVstring mName;
    std::vector<std::string> mPaths;
    
    long mParseTime;
};

#endif
