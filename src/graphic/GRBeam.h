#ifndef GRBeam_H
#define GRBeam_H

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

#include "ARBeam.h"
#include "GRPTagARNotationElement.h"
#include "GRSystemTagInterface.h"

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
		SimpleBeamList * simpleBeams;
};


/** \brief The Beam notation element.
*/
class GRBeam : public GRPTagARNotationElement, public GRSystemTagInterface
{
public:

				GRBeam(GRStaff * grstf, ARBeam * arbeam);
	virtual		~GRBeam();

	virtual void checkPosition(const GRSystem * grsys);
	virtual void ResumeTag(GRStaff * grstaff,GuidoPos assocpos);
	virtual void BreakTag(GRStaff * grstaff,GuidoPos &assocpos);
	virtual void setError(const GRStaff * grstaff,int p_error);
	virtual double getSlope(GRStaff * grstaff);
	virtual void tellPosition(GObject * gobj,const NVPoint &pos);
	virtual int getNumLines(GRStaff * grstaff);
	virtual void StaffBegin(GRStaff * grstaff = 0);
	virtual void StaffFinished(GRStaff * grstaff = 0);
	virtual void RangeEnd(GRStaff * grstaff = 0);
	virtual void addAssociation(GRNotationElement * grnot);
	virtual void print() const;
	virtual void GGSOutput() const;
	virtual void OnDraw( VGDevice & hdc ) const;
	virtual bool isAutoBeam() { return false; } // derived by GRAutoBeam

protected:
	ARBeam * getARBeam()										{ return static_cast<ARBeam *>(mAbstractRepresentation); }
	virtual GRPositionTag::GRSaveStruct * getNewGRSaveStruct()	{ return new GRBeamSaveStruct;  }

	virtual void checkNotes(GRStaff * grstaff);
	virtual int slope(GRBeamSaveStruct * st, int posx);

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
};

#endif
