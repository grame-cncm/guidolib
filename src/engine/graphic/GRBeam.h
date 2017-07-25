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

	virtual void	accept (GRVisitor& visitor);
	virtual void	checkPosition(const GRSystem * grsys);
	virtual void	ResumeTag(GRStaff * grstaff,GuidoPos assocpos);
	virtual void	BreakTag(GRStaff * grstaff,GuidoPos &assocpos);
	virtual void	setError(const GRStaff * grstaff,int p_error);
//	virtual double	getSlope(GRStaff * grstaff);
	virtual void	tellPosition(GObject * gobj,const NVPoint &pos);
	virtual int		getNumLines(GRStaff * grstaff);
	virtual void	StaffBegin(GRStaff * grstaff = 0);
	virtual void	StaffFinished(GRStaff * grstaff = 0);
	virtual void	RangeEnd(GRStaff * grstaff = 0);
	virtual void	addAssociation(GRNotationElement * grnot);
	virtual void	GGSOutput() const;
	virtual void	OnDraw( VGDevice & hdc ) const;
	virtual bool	isAutoBeam() const		{ return false; } // derived by GRAutoBeam
	virtual GRNotationElement * getEndElement();
	virtual void	addSmallerBeam(GRBeam * beam);
	virtual void	setLevel(int l)			{ fLevel = l;}
	virtual void	decLevel()				{ fLevel--;}
	virtual bool	isGraceBeaming() const	{ return fIsGraceBeaming;}

protected:
	const ARBeam * getARBeam()										{ return static_cast<const ARBeam *>(mAbstractRepresentation); }
	virtual GRPositionTag::GRSaveStruct * getNewGRSaveStruct()	{ return new GRBeamSaveStruct;  }

	virtual bool checkNotes(GRStaff * grstaff);
//	virtual int slope(GRBeamSaveStruct * st, int posx);

	bool mHasRestInMiddle;

	int lastdirection;
	int lastnumf;

	// this is set when a System-Beam needs to check
	// its slope and all (that is being called by checkPos)
	// otherwise it is 0. The parameter is used by tellposition
	// to stop execution of procedure if it is a systemtag
	// now inside GRSystemTagInterface: 

	// int isSystemCall;

private:
	typedef struct {
		GDirection	stemdir;
		float		currentSize;
		float		currentLSPACE;
		bool		oneNote;
	} PosInfos;
	NVPoint initp0 (GRSystemStartEndStruct * sse, const GREvent * startEl, PosInfos& infos);
	void	initp1 (GRSystemStartEndStruct * sse, PosInfos& infos);
	NVPoint initp2 (GRSystemStartEndStruct * sse, const GREvent * endEl, PosInfos& infos);
	void	initp3 (GRSystemStartEndStruct * sse, PosInfos& infos);
	void	slopeAdjust (GRSystemStartEndStruct * sse, const GREvent * startEl, const GREvent * endEl,float slope, PosInfos& infos);
	void	adjustFeathered (float yFact1, float yFact2, PosInfos& info, GRSystemStartEndStruct * sse);
	float	setStemEndPos (GRSystemStartEndStruct * sse, PosInfos& info, bool needsadjust, float offsetbeam);
	void	setBeams (GRSystemStartEndStruct * sse, PosInfos& infos, float yFact1, float yFact2, int direction);

	bool	fIsFeathered;
	bool	fIsGraceBeaming;
	bool	fDrawDur;
	int		fLevel;

	static std::pair<float, float> fLastPositionOfBarDuration;
//	static std::pair<float, float> & getLastPositionOfBarDuration();
	std::vector<GRBeam *> fSmallerBeams;
};

#endif
