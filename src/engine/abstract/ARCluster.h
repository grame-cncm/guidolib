#ifndef ARCluster_H
#define ARCluster_H

/*
  GUIDO Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <string>

#include "ARMTParameter.h"
#include "ARPositionTag.h"

/*@mkdoc

@group:Notes

@tagname:\cluster
@tagalias:
@tagtype:R
@tagnotation:transform 2 notes chords into clusters
@tagdesc
The **\cluster** should be applied to a 2 notes chord and transforms the corresponding range into a cluster
@tagend

@params:
@param:hdx:unit:displacement relative to the chord:0hs:true
@param:hdy:unit:displacement relative to the chord:0hs:true
@paramdesc
See the [notes](@EXAMPLES/notes/) example.
@paramend

*/

class ARNote;

/** \brief not yet documented
*/
class ARCluster : public ARMTParameter, public ARPositionTag
{		
	public:
						 ARCluster();
						 ARCluster(const ARCluster *inCopyCluster);
		virtual		 	~ARCluster() {}

		virtual const char*	getParamsStr() const	{ return kARClusterParams; };
		virtual const char*	getTagName() const		{ return "ARCluster"; };
		virtual std::string getGMNName() const		{ return "\\cluster"; };

		virtual void	setTagParameters (const TagParameterMap& params);

		ARNote *getFirstNote()  const { return fFirstNote; }
		ARNote *getSecondNote() const { return fSecondNote; }

		float getahdx() const                        { return fHdx; }
		float getahdy() const                        { return fHdy; }
		int   getNoteCount() const                   { return fNoteCount; }
		bool  getIsThereOnlyOneNoteInCluster() const { return fSingleNote; }

		void  setOnlyOneNoteInCluster() { fSingleNote = true;}
		void  setARNote(ARNote *arNote);

	protected:
		float fHdx;
		float fHdy;
		int   fNoteCount;
		bool  fSingleNote;


		ARNote *fFirstNote;
		ARNote *fSecondNote;
};

#endif
