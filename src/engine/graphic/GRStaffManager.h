#ifndef GRStaffManager_H
#define GRStaffManager_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "kf_ilist.h"	// required (should be replaced by STL containers)
#include "kf_vect.h"	// idem
#include "kr_hash.h"	// idem

template <class T> class KF_IVector;

#include "defines.h"	// For TYPE_TIMEPOSITION

#include "GRBeginSpaceForceFunction2.h"
#include "GRSpring.h"
#include "GRRod.h"

class ARMusicalTag;
class ARMusic;
class ARMusicalVoice;
class ARPageFormat;
class ARBarFormat;
class ARSystemFormat;
class ARAuto;
class ARAccolade;

class GRClef;
class GREvent;
class GRKey;
class GRPage;
class GRSystem;
class GRSystemSlice;
class GRMusic;
class GRPossibleBreakState;
class GRVoice;
class GRStaff;
class GRStaffState;
class GRVoiceManager;
class GRGlobalStem;

typedef KF_IVector<GRStaff> VStaff;
typedef KF_IVector<GRVoiceManager> VoiceManagerList;
typedef KF_Vector<int> VoiceSpringIDArray;


/** \brief This class combines the elements that are saved within a hash-entry for a 
	single graphical element.
*/

class SubHash
{
public:
	SubHash() : grel( 0 ), grstaff( 0 ), voice( 0 ), voiceID( -1 ) { }

	GRNotationElement * grel;
	GRStaff * grstaff;
	GRVoice * voice;
	int voiceID;
};

typedef KF_IPointerList<SubHash> SubHashList;

class HashEntry
{
public:
	HashEntry(int tmp)
	{
		assert(false);
		data = 0;
		mSpringID = tmp;
	}
	HashEntry()
	{
		data = 0;
		mSpringID = -1;
	}
	SubHashList * data;
	int mSpringID;
	// 
	int operator>(const HashEntry &h) const
	{
		if (mSpringID > h.mSpringID)
			return 1;
		return 0;
	}
};



typedef KR_HashTable<NVstring,HashEntry> SyncHash;
typedef KR_HashTable<NVstring,GRNotationElement *> SystemHash;
typedef SystemHash PageHash;



class VoiceEvent
{
public:
	GREvent *ev;
	GRVoice *vce;
};

typedef KF_IPointerList<GRRod> IRodList;
typedef KF_IVector<GRSpring> ISpringVector;
// typedef KF_IPointerList<GREvent> GREventList;
typedef KF_IPointerList<GRPossibleBreakState> GRPBList;
typedef KF_Vector<GuidoPos> PositionVector;
typedef KF_IVector<GRStaffState> IStaffStateVector;
typedef KF_IPointerList<VoiceEvent> vce_ev_list;
typedef KF_IPointerList<GRSystemSlice> sysslicelist;

typedef KF_IPointerList<GRBeginSpaceForceFunction2> bsfflist;
typedef KF_IPointerList<GRSpring> sprlist;
typedef KF_Vector<float> FloatVector;


	class GRForceRodEntry
	{
		// this class represents the entries in a list for determining, which springs
		// are stretched by InitialSpringStretch.
		// The Entries are later sorted by space-rods (and start-springIDs)
		// and further by force ...
		public:

			GRForceRodEntry() : force( 0 ), rod( 0 ) { }
	
			// the routine to compare the  entries against each other ..
			// returns:
			// -1: e1<e2
			//  0: e1 == e2
			//  1: e1 > e2
			static int comp( const GRForceRodEntry * e1, const GRForceRodEntry * e2 );
			float force;
			GRRod * rod;
	};


/** \brief  This class manages the staffs of a system. 

	The class fills a staff with
	objects/events (it converts the abstract representation into the graphical representation).
	The GRStaffManager will be called from the GRSystem.
	That means GRSystem gives the start and end time of the system and abstract representation and
	the GRStaffManager retrieves the objects and put them into the staffs. At the end the staffs 
	will be added to the system.
*/

class GRStaffManager
{
		friend class GRSystem;
		friend class GRPossibleBreakState;

	public:

				 GRStaffManager(GRMusic * p_grmusic, ARPageFormat * inPageFormat = 0, const GuidoLayoutSettings * settings = 0);
        virtual ~GRStaffManager();

		// this routine is used to get the current beginning_sff
		// This is used by the constructor of GRSystemSlice to save a pointer to the value, which is then used later
		// to calculate the actual optimum linebreaks ....
		GRBeginSpaceForceFunction2 * getCurrentBegSFF();

		int  IsAutoPageBreak() const;
		void setAutoTag		(const ARAuto *p_arauto);
		void setBarFormat	(const ARBarFormat * barfrmt,GRStaff * curstaff);
		void setSystemDistance(float distance, const GRVoiceManager & p_vcmgr);
		void setPageFormat	(const ARPageFormat * pform);
		void setSystemFormat(const ARSystemFormat * sysfrm);
		void NewPage		(GRPage *newpage);


		void addElementToSpring(GRNotationElement *grne,int springid);
		static float InitialSpringStretch(int start, int end, IRodList * simplerods, IRodList * complerods, ISpringVector *sprvect, std::ofstream *springlog = 0);
		int deleteRod(GRRod *rod);
		int addRod(GRRod *rod,bool spaceactive, bool atHead = false, int voiceID = -1);
		int FinishSyncSlice(const TYPE_TIMEPOSITION &tp);
		int AddGRSyncElement(GRNotationElement *grel, GRStaff *grstaff,int voiceid, GRVoice *vce, GRNotationElement * sameel = 0);
		int AddSystemTag(GRNotationElement * grel, GRStaff * grstaff,int voiceid);
		int AddPageTag(GRNotationElement * grel, GRStaff * grstaff,int voiceid);
		int getNewLinePage() const;

		virtual bool	setStaffStateTag(const ARMusicalTag * tag, int staffnum);
		virtual float	getSystemWidthCm();
//		virtual void	EndPage(ARMusicalVoice * voice, GuidoPos pos);
//		virtual void	EndSystem(ARMusicalVoice * arVoice, GuidoPos pos);
//		virtual void	setRelativeEndTimePosition(const TYPE_TIMEPOSITION & tp);
		virtual TYPE_TIMEPOSITION getRelativeTimePositionOfGR() const		{ return relativeTimePositionOfGR; }
		virtual GRStaff *	getStaff(int staff);
		virtual void		prepareStaff(int staff);

		TYPE_TIMEPOSITION	getRelativeEndTimePosition() const				{ return relativeTimePositionOfGR + mDurationOfGR; }
		GRGlobalStem *		getOtherGlobalStem(GRSystemSlice * psys, GRVoiceManager * curvcmgr, const NVstring & label);

		void createStaves();

		GRMusic * getGRMusic()												{ return mGrMusic; }
		int getNumVoices() const;

		void	notifyAccoladeTag( const ARAccolade * inAccoladeTag );

	protected:

#ifdef SPRINGLOG
		ofstream springlog;
#endif 
		// this variable remembers, wether we are currently breaking.
		// this is set before calling GRVoiceManager->Dobreak so that the prepare-staff-Function of GRStaffManager
		// nows that it should copy state-information that was previously available.
		bool	mIsBreak;
		int		mSystemSize;

		void UpdateBeginningSFF(int staffnum);
		void ResumeOpenTags (const GRSystemSlice * lastslice, GRSystemSlice * beginslice);

		// remember the system-distance (if it is explicitly set ....)
		float mSystemDistancePrev;
		float mSystemDistance;

		GRClef * mMaxClef;
		GRKey  * mMaxKey;
	
		NEPointerList * deletedElements;
		int				mNewLinePage;
		VStaff *		mMyStaffs;
		PositionVector	staffposvect;
		VoiceManagerList * mVoiceMgrList;
		GRSystemSlice * mGrSystemSlice;
		GRSystem *		mGrSystem;
		GRMusic *		mGrMusic;
		GRPage *		mGrPage;

		const ARSystemFormat * mCurSysFormat;
		sysslicelist *	 mSystemSlices;

		// this variable holds in a list the sffs for the beginning of a line given the current state information.
		// Whenever a clef or key changes, the sff is updated and added to the list. These values can then be used to evaluate
		// the potential lines. 
		bsfflist * beg_sff_list;
		sprlist * beg_spr_list;

		FloatVector staffTopVector;
		FloatVector staffBottomVector;

		// these rods are build during BuildSPF and then saved by GRPossibleBreakPosition
		// to be merged later through MergeSPFs 
		GRRod * lastrod;
		GRRod * firstrod;

		TYPE_TIMEPOSITION	relativeTimePositionOfGR;
		TYPE_DURATION		mDurationOfGR;
		const ARMusic *		mArMusic;

	
		SystemHash	systemHash;		// a hash for system-tags
		PageHash	pageHash;		// a hash-table for page-tags
		SyncHash	syncHash;		// this hash has all the synchronization work ...
		VoiceSpringIDArray voiceSpringArr;
		int mTempSpringID;			// The temporary SpringID for each Slice (especially for Tags)

		// The real SpringID ...
		// This is incremented for the whole System that this stafmanager handles ...
		int mSpringID;
		// this remembers the mLastSpringID for the BuildSPF function.
		int mLastSpringID;

		// The mSimpleRods are the ones, that cover just a single spring
		IRodList * mSimpleRods;

		// mComplexRods cover more than one spring (they need to be handled differently during stretching).
		IRodList * mComplexRods;

		// This vector holds the springs that are handled by this staffManager
		// They are later passed to the system(slice) ...
		ISpringVector * mSpringVector;

		// The staff-State as a vector analogous to the StaffVector
		IStaffStateVector * mStaffStateVect;

		// The SpaceForceFunction
		// This is build a new, when a call to BuildSPF is done.
		// It is then added to the break-state if needed ...
		// At the end, the new spf is added to the curspf.

		// this is the SPF for the FIRST-FIT-Algorithm.
		// Later on, the optimum-fit will be realized by having SPFs for each part between pbreaks.
		GRSpaceForceFunction2 * cursff;

		// The EventList for events that are added for synchronisation
		vce_ev_list evlist;

		GRPBList *	pblist;
		const ARAuto *	mArAuto;
		std::vector<const ARAccolade*> mCurAccoladeTag;
		GuidoLayoutSettings settings;
	
	private:
		// the methods below are used by createStaves
		// they have been added to structure the structure the method
		enum	{ kNormalState, kNewSystem, kNewPage, kPBreak };
		typedef struct {
			TYPE_TIMEPOSITION timePos;		// the current time position
			TYPE_TIMEPOSITION mintp;		// the smallest timeposition for the next event in any of the voices
			// This position is maintained for forward switching ...
			// It remembers the smallest timeposition where the is a need for a filltagmode=1
			TYPE_TIMEPOSITION minswitchtp;
			bool	conttagmode;
			int		newline;				// this is set if at least one voice has a newSystem/newPage
			float	pbreakval;
		} TCreateStavesState;

		void 	checkAccidentalsCollisions();
		void 	finishStaves(const TCreateStavesState& state, float beginheight);
		bool	nextTimePosition (int nvoices, bool filltagMode, TCreateStavesState& state);
		float	systemBreak (int newlineMode, float beginheight);
		int		initVoices(int cnt);
        void    applyStaffSize(GRStaff *staff, int staffNum);

		// this routine takes the current system-slice list and finds the optimum break for it.
		// it then also creates the systems with the respective systemslices.
		float			FindOptimumBreaks(int pageorsystembreak, float beginheight = 0 );
		GRSystemSlice * CreateBeginSlice (const GRSystemSlice * lastslice);
		void			createNewSystemRods(int startid,int endid);
		int				CheckForBarSpring(int sprid);
//		void			EndStaves(const TYPE_TIMEPOSITION & tp, int lastline = 0);
		void			handleDeletedElements();

		void			MergeSPFs(GRPossibleBreakState * pbs1, GRPossibleBreakState * pbs2 = 0);
		void			BreakAtPBS(GuidoPos pbpos);
		GRSpaceForceFunction2 * BuildSFF();

		// this routine takes care of breaking the positiontags of a new system.
		// The routine first retrieves the lastslice of the (new, to be created) system and then manages the still open tags of the
		// system. It saves the open tags in a list, which will then be used to resume the tags in the next line.
		void TakeCareOfBreakAt(GRSystem * newsys);
};

#endif
