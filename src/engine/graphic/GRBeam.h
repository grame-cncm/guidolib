#ifndef GRBeam_H
#define GRBeam_H

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

#include "ARBeam.h"
#include "ARFeatheredBeam.h"
#include "GRPTagARNotationElement.h"
#include "GRSystemTagInterface.h"
#include "GRVisitor.h"

class GRSimpleBeam;
class GREvent;

typedef KF_IPointerList<GRSimpleBeam> SimpleBeamList;

class GRBeamSaveStruct : public GRPositionTag::GRSaveStruct
{
	public:		
				GRBeamSaveStruct()
					{
						dirset = 0;
						direction = 0;
						simpleBeams = 0; 
					}

		virtual ~GRBeamSaveStruct();
	
		int dirset;
		int direction;
		NVPoint p[4];
		NVPoint DurationLine[6];
		std::string duration;
		SimpleBeamList * simpleBeams;
};


/** \brief The Beam notation element.
*/
class GRBeam : public GRPTagARNotationElement, public GRSystemTagInterface
{
public:
				 GRBeam(GRStaff * grstf, const ARBeam * arbeam);
	virtual		~GRBeam();

	virtual void	accept 		(GRVisitor& visitor);

	virtual void	checkPosition(const GRSystem * grsys);
	virtual void	ResumeTag	(GRStaff * grstaff,GuidoPos assocpos);
	virtual void	BreakTag	(GRStaff * grstaff,GuidoPos &assocpos);
	virtual void	setError	(const GRStaff * grstaff,int p_error);
	virtual void	tellPosition(GObject * gobj,const NVPoint &pos);
	virtual void	StaffBegin	(GRStaff * grstaff = 0);
	virtual void	StaffFinished(GRStaff * grstaff = 0);
	virtual void	RangeEnd	(GRStaff * grstaff = 0);
	virtual void	addAssociation(GRNotationElement * grnot);
	virtual void	OnDraw		( VGDevice & hdc ) const;
	virtual void	addSmallerBeam(GRBeam * beam);

	virtual GRNotationElement * getEndElement();
	virtual bool	isAutoBeam() const		{ return false; } // derived by GRAutoBeam
	virtual void	setLevel(int l)			{ fLevel = l;}
	virtual void	decLevel()				{ fLevel--;}
	virtual bool	isGraceBeaming() const	{ return fIsGraceBeaming;}

protected:
	const ARBeam * getARBeam()									{ return static_cast<const ARBeam *>(mAbstractRepresentation); }
	virtual GRPositionTag::GRSaveStruct * getNewGRSaveStruct()	{ return new GRBeamSaveStruct;  }
	virtual bool checkNotes(GRStaff * grstaff);


private:
	typedef struct {
		GDirection	stemdir;
		float		currentSize;
		float		currentLSPACE;
		bool		oneNote;
		bool		stemsReverse; 	// set to true when the beaming connect reversed stems (up and down)
		bool		stavesStartEnd; // set to true when start and end are on different staves
		NVPoint		startStaff;		// the start staff position
		NVPoint		endStaff;		// the end staff position
	} PosInfos;

	void 	initp0 (GRSystemStartEndStruct * sse, const GREvent * startEl, PosInfos& infos);
	void	initp1 (GRSystemStartEndStruct * sse, PosInfos& infos);
	void 	initp2 (GRSystemStartEndStruct * sse, const GREvent * endEl, PosInfos& infos);
	void	initp3 (GRSystemStartEndStruct * sse, PosInfos& infos);
	void	slopeAdjust (GRSystemStartEndStruct * sse, const GREvent * startEl, const GREvent * endEl,float slope, PosInfos& infos);
	void	adjustFeathered (float yFact1, float yFact2, PosInfos& info, GRSystemStartEndStruct * sse);
	float	setStemEndPos (GRSystemStartEndStruct * sse, PosInfos& info, bool needsadjust, float offsetbeam);
	void	setBeams (GRSystemStartEndStruct * sse, PosInfos& infos, float yFact1, float yFact2, int direction);
	bool	reverseStems  (const NEPointerList* assoc) const;

	bool	fIsFeathered;
	bool	fIsGraceBeaming;
	bool	fDrawDur;
	int		fLevel;
	bool 	fHasRestInMiddle;

	static std::pair<float, float> fLastPositionOfBarDuration;
	std::vector<GRBeam *> fSmallerBeams;
};

#endif
