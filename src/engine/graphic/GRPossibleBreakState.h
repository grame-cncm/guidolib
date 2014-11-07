#ifndef GRPossibleBreakState_H
#define GRPossibleBreakState_H

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

#include "GRStaff.h"	// for GRStaffState

template <class T> class KF_IVector;
template <class T> class KF_IPointerList;

class GRRod;
class GRStaff;
class GRVoiceManager;
class GRVoice;
class GRStaffManager;
class GRTag;
#ifdef OLDSPFACTIVE
class GRSpaceForceFunction;
#endif
class GRSpaceForceFunction2;

/** \brief GRPossibleBreakState saves the states of all structures that are 
	important, if it is decided by the Break-Routine, that a possible break should
	 become a Real break.
*/
class GRPossibleBreakState
{
public:
	class GRStaffAndState
	{
		public:
			// This is just a pointer for the staff.
			GRStaff * pstaff;

			// this is a REAL Copy of the lastrod of the staff, as found when the Possible Break is encountered.
			GRRod * lastrod;
			GRRod * firstrod;
			GuidoPos tpos; // tailposition of staff
			GRStaffState staffstate;

			GRStaffAndState()
			{ 
				pstaff = 0;
				lastrod = 0;
				firstrod = 0;
				tpos = 0;
			}

			virtual ~GRStaffAndState();
			void setLastRod(GRRod * plastrod);
			void setFirstRod(GRRod * pfirstrod);
	};

	class GRVoiceTagsAndStaff
	{
		public:
			class GRTagAndPosition
			{
				public:
					GRTag * grtag;
					GuidoPos assocpos;
			};

		public:
			GRVoiceManager * grvoicemgr;
			int staffnum;
			void BeginAfterBreak(GRStaff * newstaff, GuidoPos addpos);
			void EndAtBreak();
			// this saves the open tags for
			// a Voice
			KF_IPointerList<GRTagAndPosition> * grtags;
			// KF_IPointerList<GRTag> *grtags;

			// this is the curstaff for a voice at a pbreak. (needed to close the open tags if needed)
			GRStaff * pstaff;

			// the last rod of the voice.
			// This is needed to restretch the springs, if this break becomes the real newline.
			GRRod * lastrod;

			// the first rod of the voice in this section.
			// This is needed, when adding together spf's or doing "real" breaks.
			GRRod * firstrod;

			// the staffnum is saved so that tags being reopened after a break know, on which staff
			// they belong ...

			GRVoiceTagsAndStaff(GRVoiceManager * pvoicemgr = 0)
			{ 
				grvoicemgr = pvoicemgr;
				grtags = 0;
				lastrod = 0;
				firstrod = 0;
				staffnum = -1;
			}

			virtual ~GRVoiceTagsAndStaff();

			void SaveGRTags(KF_IPointerList<GRTag> * grtags);
			void setLastRod(GRRod * plastrod);
			void setFirstRod(GRRod * pfirstrod);
	};

public:
	float pbreakval;

	// this variable hold the SpaceForceFunction for the current breakstate.
#ifdef OLDSPFACTIVE
	GRSpaceForceFunction * spf;
	GRSpaceForceFunction * copyofcompletespf;
#endif

	GRSpaceForceFunction2 * sff;
	GRSpaceForceFunction2 * copyofcompletesff;

			 GRPossibleBreakState();
	virtual ~GRPossibleBreakState();

	KF_IVector<GRStaffAndState> * ssvect;		// the staff-state-vector
	KF_IVector<GRVoiceTagsAndStaff> * vtsvect;	// the voicetag-vector
	int springID;								// the springID (StaffManager) at this time
	GuidoPos simplerodspos;
	GuidoPos complexrodspos;	
	GuidoPos systemelementendpos;				// the position of the last element in the system ...
	TYPE_TIMEPOSITION tp;						// The timeposition of this PBreak

	float force;

	void SaveState	(KF_IVector<GRStaff> * vstaffs, KF_IVector<GRVoiceManager> * vvcemgrs,
						GRStaffManager * staffmgr, const TYPE_TIMEPOSITION & curtp,
						float parforce, float breakval);

	void ChangeStaffPointers (KF_IVector<GRStaff> * newstaves, GRPossibleBreakState * oldpbs);

public:
	// the lastrod and firstrod saves the rods that are created by the GRStaffManger
	// itself to adjust overlaps ....
	GRRod *lastrod;
	GRRod *firstrod;

};

#endif
