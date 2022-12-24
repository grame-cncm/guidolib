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

#include <cmath>
#include <string>
#include <sstream>
#include <vector>

#include "ARBeam.h"
#include "ARFeatheredBeam.h"
#include "GRPTagARNotationElement.h"
#include "GRSystemTagInterface.h"
#include "GRVisitor.h"

class GRSimpleBeam;
class GREvent;

typedef std::vector<GRSimpleBeam *> SimpleBeamList;
typedef struct beamRect {
	NVPoint topLeft;
	NVPoint bottomLeft;
	NVPoint bottomRight;
	NVPoint topRight;

	void xList(float* l) const 	{ l[0] = topLeft.x; l[1] = bottomLeft.x, l[2] = bottomRight.x; l[3] = topRight.x; }
	void yList(float* l) const 	{ l[0] = topLeft.y; l[1] = bottomLeft.y, l[2] = bottomRight.y; l[3] = topRight.y; }
	float height() const		{ return topRight.y - topLeft.y; }
	float width() const			{ return std::abs(topRight.x - topLeft.x); }
	float slope() const			{ return height() / width(); }
	std::string toString() const { std::stringstream s; s << topLeft << " " << bottomLeft << " " << bottomRight << " " << topRight; return s.str(); }

	void operator -= (NVPoint p)	{ topLeft -= p; bottomLeft -= p; bottomRight -= p; topRight -= p; }
	void yOffset (float val) 		{ topLeft.y += val; bottomLeft.y += val; bottomRight.y += val; topRight.y += val;}
	void tilt (float val) 			{ topLeft.y -= val; bottomLeft.y -= val; bottomRight.y += val; topRight.y += val;}
	bool includes (float x) const	{ return (topLeft.x <= x) && (topRight.x >= x); }
	
	bool operator == (struct beamRect& r)  { return (topLeft==r.topLeft) && (topRight==r.topRight) && (bottomLeft==r.bottomLeft) && (bottomRight==r.bottomRight); }
	bool operator != (struct beamRect& r)  { return !(*this == r); }
} BeamRect;

class GRBeamSaveStruct : public GRPositionTag::GRSaveStruct
{
	public:		
				GRBeamSaveStruct()
					{
						dirset = 0;
						direction = 0;
					}

		virtual ~GRBeamSaveStruct();
	
		int dirset;
		int direction;
//		BeamRect fRect;
		NVPoint DurationLine[6];
		std::string duration;
		SimpleBeamList simpleBeams;	// the list of all graphics beams, including the main one appart in case of nested beams
};


/** \brief The Beam notation element.
 
	beams contains a set of rectangles
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
	virtual void	incLevel()				{ fLevel++;}
	virtual bool	isGraceBeaming() const	{ return fIsGraceBeaming;}

			void	refreshPosition();
			void	refreshBeams (const GRSystemStartEndStruct * sse, float currentLSPACE, int dir);
			std::string beamed() const;   // gives the list of beamed notes as a string
			void	setParent(GRBeam* parent)	{ fParent = parent; }
			GRBeam*	topParent(GRBeam* parent)	{ return fParent ? fParent->topParent(fParent) : parent; }
			const GREvent* startElt() const 	{ return fStartElt; };
			const GREvent* endElt() const 		{ return fEndElt; };
			GDirection getStemsDir () const;


protected:
	const ARBeam * getARBeam()									{ return static_cast<const ARBeam *>(mAbstractRepresentation); }
	virtual GRPositionTag::GRSaveStruct * getNewGRSaveStruct()	{ return new GRBeamSaveStruct;  }
	virtual bool checkNotes(GRStaff * grstaff);


private:
	typedef struct PosInfos {
		GDirection	stemdir;
		float		currentSize;
		float		currentLSPACE;
		bool		oneNote;
		bool		stemsReverse; 	// set to true when the beaming connect reversed stems (up and down)
		bool		stavesStartEnd; // set to true when start and end are on different staves
		const GREvent* highNote;	// the highest note (y position)
		const GREvent* lowNote;		// the lowest note
		NVPoint		startStaff;		// the start staff position
		float		highStaff;		// the higher staff y position
		float		lowStaff;		// the lower staff y position
		NVPoint		endStaff;		// the end staff position
	} PosInfos;

	void 	initTopLeft 	(GRSystemStartEndStruct * sse, const GREvent * startEl, PosInfos& infos);
	void	initBottomLeft 	(GRSystemStartEndStruct * sse, PosInfos& infos);
	void 	initTopRight 	(GRSystemStartEndStruct * sse, const GREvent * endEl, PosInfos& infos);
	void	initBottomRight (GRSystemStartEndStruct * sse, PosInfos& infos);
	void 	initRect 		(const GREvent * startEl, const GREvent * endElt, const GRBeam* parent, const PosInfos& infos);
	void	slopeAdjust 	(GRSystemStartEndStruct * sse, const GREvent * startEl, const GREvent * endEl,float slope, PosInfos& infos);
	float	slopeAdjust 	(BeamRect& rect);
	void	adjustFeathered (float yFact1, float yFact2, PosInfos& info, GRSystemStartEndStruct * sse);
	void	setStemEndPos 	(GRSystemStartEndStruct * sse, PosInfos& info, bool needsadjust);
	void 	adjustStemEndPos (GRSystemStartEndStruct * sse, PosInfos& infos);
	void	setBeams 		(GRSystemStartEndStruct * sse, PosInfos& infos, float yFact1, float yFact2, int direction);
	bool	reverseStems  			(const NEPointerList* assoc) const;
	void	yRange  				(const NEPointerList* assoc, const GREvent*& high, const GREvent*& low) const;
	void	scanStaves  			(const NEPointerList* assoc, float& highStaff, float& lowStaff) const;
	bool 	checkPartialBeaming (GuidoPos pos, GuidoPos endpos, GREvent *& next, int curFaehnchen);
	BeamRect getLeftPartialBeam  (GREvent* elt, float space, float size, float lspace, float slope, bool dirchange, int num) const;
	void 	 getRightPartialBeam (BeamRect& r1, float size, float lspace, float slope) const;

	void 	setLeft (BeamRect& r, const NVPoint& pos, const GRStaff* staff) const;
	void 	setRight (BeamRect& r, const NVPoint& pos, const GRStaff* staff, float xadjust) const;
	void 	adjustTremolos (GuidoPos pos);
	void	setUserLengths(GRNotationElement* start, GRNotationElement* end, const ARBeam * arBeam);
	const GREvent* previousEvent (GREvent* ev) const;

	float 	getBeamSpace(float lspace) const { return 0.75f * lspace; }
	float 	getBeamSize (float lspace) const { return 0.4f * lspace; }
	float 	getStemsOffset (GRSystemStartEndStruct * sse, PosInfos& infos, bool needsadjust) const;
	float 	getSlope (const GRSystem * system) const;
	BeamRect& mainRect ()				{ return fRect; }
	const BeamRect& mainRect ()	const 	{ return fRect; }
	
	bool	fIsFeathered;
	bool	fIsGraceBeaming;
	bool	fDrawDur;
	int		fLevel;
	bool 	fHasRestInMiddle;

	static std::pair<float, float> fLastPositionOfBarDuration;
	BeamRect 			  fRect;
	std::vector<GRBeam *> fSmallerBeams;	// smaller beams are beams nested in the current one
	GRBeam * fParent = nullptr;  // the parent beam in case of nested beams
	GREvent* fStartElt = nullptr;
	GREvent* fEndElt = nullptr;
	
};

#endif
