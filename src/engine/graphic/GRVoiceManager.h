#ifndef GRVoiceManager_H
#define GRVoiceManager_H

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

// #include "ARTStem.h"
// #include "GRTag.h"
#include "GRBeam.h"
#include "GRPossibleBreakState.h"

template <class T> class KF_IPointerList;

class ARTStem;
class ARTHead;
class ARDotFormat;
class ARNoteFormat;
class ARRestFormat;
class ARMusicalVoice;
class ARPositionTag;
class ARMusicalTag;
class ARMusicalEvent;

class GRBeam;
class GRChordTag;
class GRCluster;
class GREvent;
class GRGlissando;
class GRGlobalLocation;
class GRGlobalStem;
class GRGrace;
class GRIntens;
class GRMusic;
class GRNotationElement;
class GRRange;
class GRSingleNote;
class GRStaff;
class GRStaffManager;
class GRTag;
class GRTagARNotationElement;
class GRTrill;
class GRVoice;
class NEPointerList;

typedef KF_IPointerList<GRTag> GRTagPointerList;

/** \brief not yet documented
*/
class GRVoiceManager  
{
	friend class GRStaffManager;
	friend class GRPossibleBreakState;

public:
			 GRVoiceManager(GRMusic* music, GRStaffManager * p_staffmgr, const ARMusicalVoice * p_voice, int p_voicenum);
	virtual ~GRVoiceManager();

	float pbreakval;

	void		ReadBeginTags(const TYPE_TIMEPOSITION & tp);
	void		setGRStaff(GRStaff * newstaff);
	void		closeOpenTags();
	float		GetBreakScore(const TYPE_TIMEPOSITION & tp);
	GRVoice *	getGRVoice();

	enum {	CURTPBIGGER_ZEROFOLLOWS=-1,
			CURTPBIGGER_EVFOLLOWS = -2,
			NEWSYSTEM = -3,
			NEWPAGE = -4,
			PBREAK = -5,
			MODEERROR = -6,
			ENDOFVOICE=-10,
			DONE = 0,
			DONE_ZEROFOLLOWS = 1,
			DONE_EVFOLLOWS = 2 };


			void		checkCenterRest		(GRStaff * grstaff, float lastpos, float newpos);
			void		removeAssociations	(const NEPointerList & nl);
			GRStaff *	getCurStaff() const;
	virtual void		addGRTag			(GRTag * grtag, int head=1);
			void		rememberLastNLinePosition	(const TYPE_TIMEPOSITION & tp);
			void		setPossibleNLinePosition	(const TYPE_TIMEPOSITION & tp);

	// virtual void ManageVoice(int n);

//	virtual void	EndManageVoice( const TYPE_TIMEPOSITION & tp);
	virtual void	BeginManageVoice();

			int		Iterate		(TYPE_TIMEPOSITION & tp, int filltagmode);
			int		DoBreak		(const TYPE_TIMEPOSITION & tp, int system_or_page);
			int		getStaffNum () const	{ return staffnum; }
			const ARMusicalVoiceState * getVoiceState() const	{ return fVoiceState; }
			const ARMusicalVoice* getARVoice() const 			{ return arVoice; }
	static  void 	resetCurrentNotesTP() 						{ fCurrentNotesTP.clear(); }

protected:
	void beginOpenTags();

	GRVoice * grvoice;
	int voicenum;
	GREvent * lastev;

	GRChordTag *		curchordtag;
	GRGlobalStem *		curglobalstem;
	GRGlobalLocation *	curgloballocation;

//	virtual void addStartPTags();

	GRStaff *		mCurGrStaff;
	GRStaffManager* mStaffMgr;

	GRGrace *		mCurGrace;	
    GRCluster *     mCurCluster;
	GRGlissando *	mCurGlissando;

	const ARMusicalEvent*	curev;
	const ARMusicalVoice*	arVoice;
	const ARNoteFormat *	curnoteformat;
	const ARDotFormat *		curdotformat;
	const ARRestFormat *	currestformat;
	const ARTStem *			curstemstate;
	const ARTHead *			curheadstate;
	GRIntens*				curIntens = 0;

	static bool &	getCurStaffDraw(int index);

	TYPE_TIMEPOSITION curtp;
	int staffnum;

	GREvent *CreateNote			(const TYPE_TIMEPOSITION & tp, ARMusicalObject * arObject);
	GREvent *CreateTab			(const TYPE_TIMEPOSITION & tp, ARMusicalObject * arObject);
	GREvent *CreateEmpty		(const TYPE_TIMEPOSITION & tp, ARMusicalObject * arObject);
	GREvent *CreateRest			(const TYPE_TIMEPOSITION & tp, ARMusicalObject * arObject);
	GREvent *CreateGraceNote	(const TYPE_TIMEPOSITION & tp, ARMusicalObject * arObject, const TYPE_DURATION & dur);

	virtual void checkStartPTags(GuidoPos tstpos);
	virtual void checkEndPTags(GuidoPos tstpos);

	GRTagPointerList * getGRTags()	{ return fGRTags; }
	GRTagPointerList * nlinegrtags; // remembered for the nline-state ...

	GRNotationElement * parseTag(ARMusicalObject * arOfCompleteObject);
	void parsePositionTag		(ARPositionTag * apt);
	bool parseStateTag			(const ARMusicalTag * mtag);
	bool checkRepeatBeginNext();

	GRTagPointerList * toadd;
	
    // - for clusters
    int mCurrentClusterNoteNumber;

private:
	// this is needed for determining the elements that are centered in a bar like whole-note-rests
	GRMusic*				fMusic;
	GREvent *				fLastnonzeroevent;
	GRTagARNotationElement* fLastbar;
	GROctava*				fLastOctava;
	ARMusicalVoiceState *	fVoiceState;
	GRTagPointerList *		fGRTags;
	GRTrill*				fCurrentTrill = 0;
	

	GRSingleNote *	CreateSingleNote	(const TYPE_TIMEPOSITION & tp, ARMusicalObject * arObject, float size=0, bool isGrace=false);
	void			AddRegularEvent		(GREvent * ev);
	void			organizeGlissando(GRTag * g);
	void			organizeBeaming(GRBeam * grb);
	void			checkFillBar (GRTagARNotationElement* bar);
	void			addAssociations (GREvent* ev, bool setnext=true);
	int				endIteration ();
	void			checkCluster(GREvent *ev);
	void			checkHiddenNotes(const std::vector<GRSingleNote *>& notes);

	int 			IterateEvent		(ARMusicalEvent * arev, TYPE_TIMEPOSITION &timepos);
	int 			IterateNoDurEvent	(ARMusicalObject * obj, const TYPE_TIMEPOSITION& timepos);
	int 			IterateTag			(ARMusicalObject * obj);
	int 			IterateChord		(const TYPE_TIMEPOSITION& timepos);
	TYPE_DURATION	findDuration (const ARMusicalVoiceState * state, const ARMusicalEvent* ev) const;
	void 			doAssociate(GRSingleNote * grnote);
	
	std::vector<GRBeam *> fCurbeam;
	std::vector<GRBeam *> fBeams;
	typedef std::vector<std::pair<GRRange*, GRSingleNote*> >	TSharedArticulationsList;
	TSharedArticulationsList fSharedArticulations;
	void			handleSharedArticulations(const TSharedArticulationsList& list);
	void			setTrillNext (GRNotationElement* ev);

	static std::vector<GRSingleNote *> fCurrentNotesTP;   // current notes at a given time position (shared)
};

#endif
