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

	virtual void 	print() const;
	virtual void	print (std::ostream & os) const;
	virtual std::ostream & output(std::ostream & os, bool isauto = true) const;

	virtual void 	resetGRRepresentation();
	virtual void	getTimeMap (TimeMapCollector& f) const;

			void 	removeAutoTags();

    const NVstring  & getName() const { return mName; }
    void  setName(const char * in)    { mName = in; }
    
    const std::vector<std::string> &getPath() const  { return mPaths; }  
    void  setPath( std::vector<std::string> inPaths) { mPaths = inPaths; }

    void setParseTime(int time) { mParseTime = time; }
    int  getParseTime()         { return mParseTime; }

    long       mMaxTagId;
    static int mRefCount;

protected:

    void doAutoBreaks();

    NVstring mName;
    std::vector<std::string> mPaths;
    int mParseTime;
};

inline std::ostream & operator<<(std::ostream & os, const ARMusic* m)	{ m->print(os); return os; }


#endif



