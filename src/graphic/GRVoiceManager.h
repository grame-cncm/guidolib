#ifndef GRVoiceManager_H
#define GRVoiceManager_H

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

// #include "ARTStem.h"
// #include "GRTag.h"
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

class NEPointerList;
// class GRNoteFactory;
class GRStaff;
class GREvent;
class GRVoice;
class GRNotationElement;
class GRGrace;
class GRCluster;
class GRTrill;
class GRGlobalStem;
class GRGlobalLocation;
class GRChordTag;
class GRStaffManager;
class GRTag;
class GRSingleNote;

typedef KF_IPointerList<GRTag> GRTagPointerList;

/** \brief not yet documented
*/
class GRVoiceManager  
{
	friend class GRStaffManager;
	friend class GRPossibleBreakState;

public:
			 GRVoiceManager(GRStaffManager * p_staffmgr, ARMusicalVoice * p_voice, int p_voicenum);
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

protected:
	void beginOpenTags();

	GRVoice * grvoice;
	// this remembers the current stemstate in the current voice.
	// ARTStem::STEMSTATE stemstate;
	ARTStem * curstemstate;
	ARTHead * curheadstate;

	int voicenum;
	GREvent * lastev;
//	GColor mColor;

	GRChordTag *		curchordtag;
	GRGlobalStem *		curglobalstem;
	GRGlobalLocation *	curgloballocation;

	virtual void addStartPTags();
		
	
	ARMusicalVoiceState * curvst;
	
	ARMusicalEvent* curev;
	GRStaff *		mCurGrStaff;
	ARMusicalVoice* arVoice;
	GRStaffManager* mStaffMgr;

	GRGrace *		mCurGrace;	
    GRCluster *     mCurCluster;
	ARNoteFormat *	curnoteformat;
	ARDotFormat *	curdotformat;
	ARRestFormat *	currestformat;

	TYPE_TIMEPOSITION curtp;
	int staffnum;

	GREvent *CreateNote			(const TYPE_TIMEPOSITION & tp, ARMusicalObject * arObject);
	GREvent *CreateEmpty		(const TYPE_TIMEPOSITION & tp, ARMusicalObject * arObject);
	GREvent *CreateRest			(const TYPE_TIMEPOSITION & tp, ARMusicalObject * arObject);
	GREvent *CreateGraceNote	(const TYPE_TIMEPOSITION & tp, ARMusicalObject * arObject, const TYPE_DURATION & dur);

	virtual void checkStartPTags(GuidoPos tstpos);
	virtual void checkEndPTags(GuidoPos tstpos);

	GRTagPointerList * grtags;
	GRTagPointerList * nlinegrtags; // remembered for
	// the nline-state ...

	GRNotationElement * parseTag(ARMusicalObject * arOfCompleteObject);
	void parsePositionTag(ARPositionTag * apt);
	bool parseStateTag(ARMusicalTag * mtag);
	bool checkRepeatBeginNext();


	GRTagPointerList * toadd;

	// this is needed for determining the elements that are centered in a bar like whole-note-rests
	GREvent * lastnonzeroevent;
	GRBar * lastbar;


private:

	GRSingleNote *	CreateSingleNote	(const TYPE_TIMEPOSITION & tp, ARMusicalObject * arObject, float size=0);
	void			AddRegularEvent		(GREvent * ev);

};

#endif
