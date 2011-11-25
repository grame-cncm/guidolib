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

#include "GUIDOEngine.h"	// for AddGGSOutput

#include "TagParameterFloat.h"

#include "GRBeam.h"
#include "GRSingleNote.h"
#include "GRSingleRest.h"
#include "GRStaff.h"
#include "GRGlue.h"
#include "GRPage.h"
#include "GRMusic.h"
#include "GRSystemTag.h"
#include "GRAutoBeam.h"
#include "GREmpty.h"
#include "GRSystemSlice.h"
#include "GRSimpleBeam.h"
#include "VGDevice.h"
#include "secureio.h"

// #include "NEPointerList.h"	// for template instanciation
#include <iostream>
using namespace std;

GRBeamSaveStruct::~GRBeamSaveStruct()
{
	delete simpleBeams;
}


GRBeam::GRBeam(GRStaff * grstaf,ARBeam * arbeam) : GRPTagARNotationElement(arbeam)
{
	mHasRestInMiddle = false;

	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct();
	
	GRBeamSaveStruct * st = new GRBeamSaveStruct();
	sse->grsystem = grstaf->getGRSystem();
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	sse->p = (void *) st;

	mStartEndList.AddTail(sse);
}

GRBeam::~GRBeam()
{
	// associations are removed within
	// the GRPositionTag-Removal ...
	FreeAssociatedList();
}

void GRBeam::GGSOutput() const
{
	if (error) return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct( gCurSystem );
	if (!sse) return;
 
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	assert(st);

	char buffer[500];
	snprintf(buffer, 500, "\\draw_beam<%ld,%d,%d,%d,%d>\n", 
			getID(),
			(int)(st->p[0].x + ggsoffsetx),
			(int)(st->p[0].y + ggsoffsety),
			(int)(st->p[3].x + ggsoffsetx),
			(int)(st->p[3].y + ggsoffsety ));

	AddGGSOutput(buffer);
}

void GRBeam::OnDraw( VGDevice & hdc) const
{
	if (error) return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct( gCurSystem );
	if (sse == 0) return;

	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	assert(st);

	if (mColRef) {
		VGColor color ( mColRef ); 	// custom or black
		hdc.PushFillColor( color );
		hdc.PushPen( color, 1);
	}
		
	float ax [4] = { st->p[0].x, st->p[1].x, st->p[3].x, st->p[2].x };
	float ay [4] = { st->p[0].y, st->p[1].y, st->p[3].y, st->p[2].y };
	
	// This does the drawing!
	hdc.Polygon( ax, ay, 4 );
	
	if (st->simpleBeams)
	{
		GuidoPos smplpos = st->simpleBeams->GetHeadPosition();
		while (smplpos)
		{
			GRSimpleBeam * smplbeam = st->simpleBeams->GetNext(smplpos);
			smplbeam->OnDraw(hdc);
		}
	}

	if (mColRef) {
		hdc.PopPen();
		hdc.PopFillColor();
	}
}

void GRBeam::print() const
{
}

void GRBeam::addAssociation(GRNotationElement * grnot)
{
	if (error || !grnot) return ;

	const GRStaff * grstf = grnot->getGRStaff();
	if (grstf == 0) return;

	if (grnot->getDuration() == DURATION_0)
	{
		GREvent * grn = GREvent::cast(grnot);
		if (!grn || grn->getGlobalStem() == NULL)
			return;
	}

	// if we are an autobeam we do not beam rests ....

	const bool isautobeam = isAutoBeam(); // (dynamic cast<GRAutoBeam *>(this) != 0);

	GREvent * grn = GREvent::cast(grnot);
	if (grn == 0) 
	{
		setError(grstf,1);
		return ;
	}

	const bool isRest = (dynamic_cast<GRRest *>(grnot) != 0);
	if (isautobeam && isRest)
	{
		if (mAssociated && mAssociated->GetCount() > 0)
			mHasRestInMiddle = true;
	}

	if (mHasRestInMiddle && !isRest)
	{
		// then we have a real Error
		setError(grstf,1);
		return;
	}

	// ignore all elements with duration 0
	// but only, if I already have the stem
	if (grnot->getDuration() == DURATION_0)
	{
		if (mAssociated)
		{
			GRNotationElement * el = mAssociated->GetTail();
			if (el)
			{
				GREvent * ev = GREvent::cast(el);
				
				if (ev->getGlobalStem() == NULL ||
					ev->getGlobalStem() == grn->getGlobalStem())
					return;
			}
		}
	}

	if (dynamic_cast<GRSingleRest *>(grn))
	{
		// ignoriere Pausen !
		// setError(grstf,1);
		return;
	}

	if (grn->getNumFaehnchen() == 0)
	{
		setError(grstf,1);
		return ;
	} 

	GRSystemStartEndStruct * sse = getSystemStartEndStruct(grstf->getGRSystem());
	if (!sse) return ;

	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	
	if (grn->getStemDirSet())
	{
		st->dirset = 1;
	}
	else
	{
		if (grn->getStemDirection() == dirUP)
			st->direction++;
		else if (grn->getStemDirection() == dirDOWN)
			st->direction--;
	}

	GRPTagARNotationElement::addAssociation(grnot);
		
	// this is a test ...
	// and cannot be done that way.
	// otherwise we do not get the flags on again ...
	grn->setFlagOnOff(false);
	grn->incBeamCount();
}

void GRBeam::RangeEnd(GRStaff * grstaff)
{
	assert(grstaff);

	GRPositionTag::RangeEnd(grstaff);

	if (error) return;
	if (!mAssociated) return;
	
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(grstaff->getGRSystem());
	assert(sse);

	if (mAssociated && (mAssociated->GetCount() == 1) && isAutoBeam())
	{
		GREvent * ev = GREvent::cast(mAssociated->GetHead());
		if (ev)
		{
			ev->setFlagOnOff(true);
			ev->decBeamCount();
			if (sse->startElement)	// something did not work, if the starElement is a glue ... and not an event
				ev->removeAssociation(this);
			if (sse->endElement)
				ev->removeAssociation(this);
		}
		return;
	}

	GREvent * grn;
	GuidoPos syststpos = sse->startpos;
	if (syststpos)
	{
		// this is all done so that I really get a correct first staff to test my stuff ...
		while (syststpos && !(mAssociated->GetAt(syststpos)))
		{
			mAssociated->GetNext(syststpos);
		}
		int tststaffnum = mAssociated->GetNext(syststpos)->getStaffNumber(); 
		while (syststpos)
		{
			GRNotationElement * el = mAssociated->GetNext(syststpos);
			if (el)
			{
				if (el->getStaffNumber() != tststaffnum)
				{
					tagtype = GRTag::SYSTEMTAG;
					GRSystemTag * mysystag = new GRSystemTag(this);
					el->getGRSystemSlice()->addSystemTag(mysystag);
					break;
				}
			}		
		}
	}

	GuidoPos pos = sse->startpos;
	GRBeamSaveStruct * st = (GRBeamSaveStruct *) sse->p;
	bool first = true;

	GDirection mytmpdir = dirOFF;
	while (pos)
	{
		grn = GREvent::cast(mAssociated->GetNext(pos));
		if (grn)
		{
			if (!st->dirset)
			{
				if (first)
				{
					// get the stemdir of the first !? maybe it now has been set?
					mytmpdir = grn->getStemDirection();
				}
				if (mytmpdir != dirOFF)
					grn->setStemDirection(mytmpdir);
				else if (st->direction>=0)
					grn->setStemDirection(dirUP);
				else
					grn->setStemDirection(dirDOWN);
			}
			grn->setFlagOnOff(false);
			first = false;
		}
	}
}

void GRBeam::StaffFinished(GRStaff * grstaff)
{
	assert(false);
	assert(grstaff);

	// first, all the BASIC stuff is handled ...
	GRPositionTag::StaffFinished(grstaff);

	if (error) return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct(grstaff->getGRSystem());

	assert(sse);

	GuidoPos syststpos = sse->startpos;
	if (syststpos)
	{
		// this is all done so that I really get a correct first staff to
		// test my stuff ...
		while (syststpos && 
			!/*ynamic_cast<GRNotationElement *>*/(mAssociated->GetAt(syststpos)))
		{
			mAssociated->GetNext(syststpos);
		}
		const GRStaff *tststaff = mAssociated->GetNext(syststpos)->getGRStaff();
		while (syststpos)
		{
			GRNotationElement * el = mAssociated->GetNext(syststpos);
			if (el)
			{
				if (el->getGRStaff() != tststaff)
				{
					tagtype = GRTag::SYSTEMTAG;
					GRSystemTag * mysystag = new GRSystemTag(this);
					el->getGRSystemSlice()->addSystemTag(mysystag);
					break;
				}
			}		
		}
	}

	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	GREvent * grn;
	checkNotes(grstaff);

	if (error) return;

	GuidoPos pos = sse->startpos;
	while (pos)
	{
		grn = GREvent::cast(mAssociated->GetNext(pos));
		if (grn)
		{
			if (!st->dirset)
			{
				if (st->direction > 0)
					grn->setStemDirection(dirUP);
				else
					grn->setStemDirection(dirDOWN);
			}
			grn->setFlagOnOff(false);
		}
	}
}

void GRBeam::StaffBegin(GRStaff * grstaff)
{
	// now we need to build a new staff 
	assert(grstaff);
	GRPositionTag::StaffBegin(grstaff);
}


int GRBeam::getNumLines(GRStaff *)
{
	// unused ?
/*
	GRSystemStartEndStruct * sse =
		getSystemStartEndStruct(grstaff->getGRSystem());
	
	assert(sse);
	
//	GRBeamSaveStruct *st = (GRBeamSaveStruct *) sse->p;
*/
	return 1;
	// return st->numf;
}

//--------------------------------------------------------------------
NVPoint GRBeam::initp0 (GRSystemStartEndStruct * sse, const GREvent * startEl, PosInfos& infos)
{
	// -- Init point 0 (top left)
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	const ARBeam * arBeam = getARBeam();

	GRStaff * refStaff;
	NVPoint offset;
	if (startEl)
	{
		st->p[0] = startEl->getStemStartPos();
		if (arBeam && arBeam->isGuidoSpecBeam())
			st->p[0].y = startEl->getPosition().y;
		refStaff = startEl->getGRStaff();
		infos.stemdir = startEl->getStemDirection();
		infos.currentSize = startEl->getSize();
	}
	else {
		st->p[0] = sse->startElement->getPosition();
		infos.currentSize = 1.0f;
		refStaff = sse->startElement->getGRStaff();
	}
	offset = refStaff->getPosition();
	if (tagtype == SYSTEMTAG)
		st->p[0] += offset;
	infos.currentLSPACE = refStaff->getStaffLSPACE();

	st->p[1] = st->p[0];
	
	// -- Adjust point 0
	if (arBeam->dx1 && arBeam->dx1->TagIsSet())
		st->p[0].x += (GCoord)(arBeam->dx1->getValue(infos.currentLSPACE));
	else
	{
		// This depends on the direction, we do not know this yet (do we?)
		if (infos.oneNote) {
			st->p[0].x -= TagParameterFloat::convertValue((float)2.0,"hs", infos.currentLSPACE) * infos.currentSize;
		}
	}

	if (arBeam->dy1 && arBeam->dy1->TagIsSet())
		st->p[0].y -= (arBeam->dy1->getValue(infos.currentLSPACE));
	return offset;
}

//--------------------------------------------------------------------
void GRBeam::initp1 (GRSystemStartEndStruct * sse, PosInfos& infos)
{
	// -- Adjust point 1
	const ARBeam * arBeam = getARBeam();
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	if (arBeam->dx2 && arBeam->dx2->TagIsSet())
		st->p[1].x += arBeam->dx2->getValue(infos.currentLSPACE);
	else
	{
		if (infos.oneNote)
			st->p[1].x -= TagParameterFloat::convertValue(2.0f,"hs", infos.currentLSPACE) * infos.currentSize;
	}
	if (arBeam->dy2 && arBeam->dy2->TagIsSet())
		st->p[1].y -= (GCoord)(arBeam->dy2->getValue(infos.currentLSPACE));
	else {
		if (infos.stemdir == dirUP)
			st->p[1].y = st->p[0].y + TagParameterFloat::convertValue(0.9f,"hs", infos.currentLSPACE) * infos.currentSize;
		else if (infos.stemdir == dirDOWN)
			st->p[1].y = st->p[0].y - TagParameterFloat::convertValue(0.9f,"hs", infos.currentLSPACE) * infos.currentSize;
	}
}

//--------------------------------------------------------------------
NVPoint GRBeam::initp2 (GRSystemStartEndStruct * sse, const GREvent * endEl, PosInfos& infos)
{
	// -- Init point 2 (top right)
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	const ARBeam * arBeam = getARBeam();

	GRStaff * refStaff;
	NVPoint offset;
	if (endEl) {
		st->p[2] = endEl->getStemEndPos();
		// beam length adjustment - DF sept 15 2009
		st->p[2].x += infos.currentLSPACE/10;
		if (arBeam && arBeam->isGuidoSpecBeam())
			st->p[2].y = endEl->getPosition().y;
		refStaff = endEl->getGRStaff();
	}
	else {
		st->p[2] = sse->endElement->getPosition();
		refStaff = sse->startElement->getGRStaff();
	}	
	offset = refStaff->getPosition();
	if (tagtype == SYSTEMTAG)
		st->p[2] += offset;
	infos.currentLSPACE = refStaff->getStaffLSPACE();

	st->p[3] = st->p[2];

	// -- Adjust point 2
	if (arBeam->dx3)
		st->p[2].x += arBeam->dx3->getValue(infos.currentLSPACE);
	if (arBeam->dy3 && arBeam->dy3->TagIsSet())
		st->p[2].y -= arBeam->dy3->getValue(infos.currentLSPACE);
	else {
		GCoord val = 0;
		if (arBeam->dy1 && arBeam->dy1->TagIsSet())
			val = arBeam->dy1->getValue(infos.currentLSPACE);
		st->p[2].y -= val;
	}
	return offset;
}

//--------------------------------------------------------------------
void GRBeam::initp3 (GRSystemStartEndStruct * sse, PosInfos& infos)
{
	// -- Adjust point 3 (bottom right)
	const ARBeam * arBeam = getARBeam();
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;

	if (arBeam->dx4)
		st->p[3].x += (arBeam->dx4->getValue(infos.currentLSPACE));
	if (arBeam->dy4 && arBeam->dy4->TagIsSet())
		st->p[3].y -= (arBeam->dy4->getValue(infos.currentLSPACE));
	else {
		if (infos.stemdir == dirUP)
			st->p[3].y =  st->p[2].y + (GCoord) (infos.currentSize * TagParameterFloat::convertValue( 0.9f,"hs", infos.currentLSPACE));
		else if (infos.stemdir == dirDOWN)
			st->p[3].y = st->p[2].y - (GCoord) (infos.currentSize * TagParameterFloat::convertValue( 0.9f,"hs", infos.currentLSPACE));
	}
}

//--------------------------------------------------------------------
void GRBeam::slopeAdjust (GRSystemStartEndStruct * sse, const GREvent * startEl, const GREvent * endEl, float slope, PosInfos& infos)
{
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	if (startEl && endEl && infos.stemdir != endEl->getStemDirection())
	{
		// then we should try an optimizing strategy which is not implemented now ....
	}
	else if (startEl && endEl && slope > 0.1f )
	{
		// adjust the length of the stem for second st->p ...			
		if( infos.stemdir == dirDOWN) {
			// change the first one ...
			const float newy = st->p[2].y - 0.1f * (float)(st->p[2].x - st->p[0].x);
			const float diff = newy - (float)st->p[0].y;
			st->p[0].y = (GCoord) newy;
			st->p[1].y += (GCoord) diff;
		}
		else {
			// change the second one ...
			const float newy = st->p[0].y + 0.1f * (float)(st->p[2].x - st->p[0].x);	
			const float diff = newy - (float)st->p[2].y;
			st->p[2].y = (GCoord) newy;
			st->p[3].y += (GCoord) diff;				
		}
	}
	else if (startEl && endEl && slope < -0.1f)
	{
		if (infos.stemdir == dirDOWN)
		{
			// change the last one 
			const float newy = -0.1f * (float)(st->p[2].x - st->p[0].x) + st->p[0].y;
			const float diff =  newy - (float)st->p[2].y;
			st->p[2].y = (GCoord) newy;
			st->p[3].y += (GCoord) diff;
		}
		else
		{
			// change the first
			const float newy = st->p[2].y + 0.1f * (float) (st->p[2].x - st->p[0].x);
			const float diff =  newy - (float)st->p[0].y;
			st->p[0].y = (GCoord) newy;
			st->p[1].y += (GCoord) diff;
		}
	}
	else if (endEl == 0)
	{
		// then we have an empty-event make the slope even .... 
		// or make it dependant on generell direction of beam (how ...)
		st->p[2].y = st->p[0].y;
		st->p[3].y = st->p[1].y;
	}		
	else if (startEl == 0)
	{
		st->p[0].y = st->p[2].y;
		st->p[1].y = st->p[3].y;
	}
}

/** \brief Called from the LAST-Note of the Beam ...
*/
// (JB) \bug There is probably a bug somewhere: with map mode
// different than "ISOTROPIC" (proportionnal), beamings polygons between "UP" stems
// draw at an incorrect x position, while "DOWN" beams are always ok.
//
// Too long, needs some code-factorization, if possible.
void GRBeam::tellPosition( GObject * gobj, const NVPoint & p_pos)
{
	/* Beams are polygons made of four points:
		
		0                  2		
		 ------------------
		 |                |
		 ------------------
		1                  3
	*/

	// a new test is performed: if it is a systemTag and
	// it is not a systemcall (checkpos), than we can just do nothing.
	if (error || !mAssociated || ( mAssociated->GetCount() == 0 )
		|| ( tagtype == GRTag::SYSTEMTAG && !mIsSystemCall )) 
		return;
	
	GRNotationElement * el = dynamic_cast<GRNotationElement *>(gobj);
	if (!el || !el->getGRStaff())
		return;
	
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(el->getGRStaff()->getGRSystem());
	assert(sse);
	if (el != sse->endElement)
		return;
	
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	GuidoPos pos;
	
	GREvent * startEl = GREvent::cast(sse->startElement);
	GREvent * endEl   = GREvent::cast(sse->endElement);
	
	// this is the staff to which the beam belongs and who draws it.
	const GRStaff * beamstaff = sse->startElement->getGRStaff();	
	PosInfos infos = { dirUP, LSPACE, 1.0f, (endEl == startEl)};
	
	ARBeam * arBeam = getARBeam();
	const bool isSpecBeam = arBeam->isGuidoSpecBeam();

	NVPoint offset = initp0 (sse, startEl, infos);
	initp1 (sse, infos);
	offset = initp2 (sse, endEl, infos);
	initp3 (sse, infos);

	// -----------------------------------------------------------
	// Now, we adjust the stemlengths, according to beamslope ...
	// we have the start and end-position in st->p
	// We have to determine the slope and adjust the slope to minimum and maximum ...
    // -----------------------------------------------------------
	float stemWidth = st->p[2].x - st->p[0].x;
	float slope = (st->p[2].y - st->p[0].y) / stemWidth;
	// another hack to control the slope when events are on different staves - DF sept 15 2009
	if (startEl && endEl && (startEl->getGRStaff() != endEl->getGRStaff())) {
		while ((slope < -0.20) || (slope > 0.20)) {
			float shift = (slope < 0) ? -LSPACE/4 : LSPACE/4;
			st->p[0].y += shift;
			st->p[1].y += shift;
			st->p[2].y -= shift;
			st->p[3].y -= shift;
			slope = (st->p[2].y - st->p[0].y) / stemWidth;
		}
	}
	
	bool needsadjust = true;
	// we have to adjust the slope ONLY, if the stemlength
	// of the first and last element has not been set automatically!
	if ( (startEl && startEl->getStemLengthSet() && endEl && endEl->getStemLengthSet())
		|| tagtype == SYSTEMTAG || (arBeam && isSpecBeam))
	{
		needsadjust = false;
	}
	else slopeAdjust (sse, startEl, endEl, slope, infos);

	if (arBeam && isSpecBeam)
	{
		// then we have to set the length ... if it is not set otherwise ...
		GRSingleNote * nt = dynamic_cast<GRSingleNote *>(sse->startElement);
		if(nt && !nt->getStemLengthSet()) {
			const float myval = arBeam->dy1->getValue();
			 nt->setStemLength( myval);
		}

		nt = dynamic_cast<GRSingleNote *>(sse->endElement);
		if(nt && !nt->getStemLengthSet()) {
			float myval;
			if (arBeam->dy3 && arBeam->dy3->TagIsSet())
				myval = arBeam->dy3->getValue();
			else
				myval = arBeam->dy1->getValue();
			nt->setStemLength( myval);
		}		
	}
	
	float offsetbeam = 0;
	// nobeamoffset describes, if no beamoffset is valid: if notes ask for different beam-offsets
	// then, there is just no offset .... things must be changed manually then ....
	bool nobeamoffset = false;
	
	if(( startEl && startEl->getStemLengthSet()) || ( endEl && endEl->getStemLengthSet()))
		  nobeamoffset = true;

	for (int counter = 0; counter < 2; ++counter )
	{
		if (counter == 1) {
			if ((offsetbeam == 0) || nobeamoffset) break;
			else if (!nobeamoffset) {
				st->p[0].y -= offsetbeam;
				st->p[1].y -= offsetbeam;
				st->p[2].y -= offsetbeam;
				st->p[3].y -= offsetbeam;
				offsetbeam = 0;
			}
		}
		
		pos = sse->startpos;
		while (pos)
		{
			// now we calculate the stem-end-positions ...
			GuidoPos oldpos = pos;
			GREvent * sn = GREvent::cast(mAssociated->GetNext(pos));
			if (sn)
			{
				float rx = sn->getStemStartPos().x - st->p[0].x;
				if (tagtype == SYSTEMTAG)
					rx += sn->getGRStaff()->getPosition().x;
				
				float disty = st->p[2].y - st->p[0].y;
				float distx = st->p[2].x - st->p[0].x;
				float ly = disty / distx * rx;
				
				ly += st->p[0].y;
				
				float diffy = (float)sn->getStemStartPos().y;
				GDirection adjustdir=dirAUTO;
				if (sn->getStemDirection() == dirUP)
				{
					diffy += (float)sn->getStemLength();
					adjustdir = dirUP;
				}
				else if (sn->getStemDirection() == dirDOWN)
				{ 
					adjustdir = dirDOWN;
					diffy -= (float)sn->getStemLength();
				}
					
				ly -= diffy;
				if (tagtype == SYSTEMTAG)
					ly -= (float)sn->getGRStaff()->getPosition().y;
				
				float offbase = 3.5f * infos.currentLSPACE;
				if (ly < 0)
				{
					if (needsadjust) {
						if (adjustdir == dirDOWN)
						{
							const float newoffs = ly - offbase;
							if (newoffs < offsetbeam)
							{
								if (offsetbeam > 0)
								{
									GuidoTrace("WARNING: different beam adjustments!");
									nobeamoffset = true;
								}
								else offsetbeam = newoffs;
							}
							ly = -offbase;
						}
						else if (ly > -offbase)
						{
							const float newoffs = ly + offbase;
							if (newoffs > offsetbeam)
							{
								if (offsetbeam < 0)
								{
									GuidoTrace("WARNING: different beam adjustments!");
									nobeamoffset = true;
								}
								else offsetbeam = newoffs;
							}
						}
					}
					ly = -ly;
				}
				else if (needsadjust) {
					if (adjustdir == dirUP)
					{
						const float newoffs = ly + offbase;
						if (newoffs > offsetbeam)
						{
							if (offsetbeam < 0)
							{
								GuidoTrace("WARNING: different beam adjustments!");
								nobeamoffset = true;
							}
							else offsetbeam = newoffs;
						}
						ly = offbase;
					}
					else if (ly < offbase)
					{
						const float newoffs = ly - offbase;
						if (newoffs < offsetbeam)
						{
							if (offsetbeam > 0)
							{
								GuidoTrace("WARNING: different beam adjustments!");
								nobeamoffset = true;
							}
							else offsetbeam = newoffs;
						}
					}
				}
//				sn->changeStemLength( ly );
				// adjusted - DF sept 15 2009
				sn->changeStemLength( ly - infos.currentLSPACE/20 );				
			}
			if (oldpos == sse->endpos)
				break;
		}
	}	
	
	// -- Now we need to add the simplebeams as simplebeamgroups ...
	NVPoint myp[4];
	int dir = st->direction;
	if (st->dirset)
	{
		// then the direction was set explicitly by the user ...
		GREvent * sn = GREvent::cast(mAssociated->GetHead());
		if (sn)
			dir = sn->getStemDirection();
	}
	else dir = infos.stemdir;

	bool first = true;
	pos = sse->startpos;

	// - These constants defines the space and the thickness of additionnal beams.
	const float yFact1 = 0.75f * infos.currentLSPACE;	// was 0.7f
	const float yFact2 = 0.4f * infos.currentLSPACE;
	// for beam length adjustment - DF sept 15 2009
	const float xadjust = infos.currentLSPACE/10;
	GDirection lastLocalDir = dirOFF;
	int previousBeamsCount = 0;
	while (pos)
	{
		GuidoPos oldpos = pos;
		GREvent * stemNote = GREvent::cast(mAssociated->GetNext(pos));
		if (stemNote)
		{
			GDirection localDir = stemNote->getStemDirection();
			float yLocalFact1 = yFact1 * localDir;
			float yLocalFact2 = yFact2 * localDir;

			// now we check the number of beams ...
			if (stemNote->getBeamCount() < stemNote->getNumFaehnchen())
			{
				float beamCount = (float)(stemNote->getBeamCount());
				stemNote->incBeamCount();
			
				if (first && (sse->startflag == GRSystemStartEndStruct::OPENLEFT)) {
					// the additional beam starts at the startElement (glue), we have more beams to draw
					myp[0] = sse->startElement->getPosition();					
					if (tagtype == SYSTEMTAG)
						myp[0] += stemNote->getGRStaff()->getPosition();
					myp[1] = myp[0];
//					first = false; // never read (according to clang :-)
				}
				else {
					// the additional beam starts at sn. We have more beams to draw
					myp[0] = stemNote->getStemStartPos();
					if (tagtype == SYSTEMTAG)
						myp[0] += stemNote->getGRStaff()->getPosition();
					myp[0].y += beamCount * yLocalFact1;
					if (localDir != dir)
						myp[0].y -= yLocalFact2;
					myp[1].x = myp[0].x;
					myp[1].y = myp[0].y + yLocalFact2;
				}

				// now we look for the endposition
				GREvent * sn2 = NULL;
				GuidoPos tmppos = pos;
				// partialbeam is set, if the new SimpleBeam only covers part of the masterBeam.
				int partialbeam = 0;
				while (tmppos)
				{
					GuidoPos oldpos2 = tmppos;
					GREvent * tmpsn = GREvent::cast(mAssociated->GetNext(tmppos));
					if (tmpsn) {
						if (tmpsn->getBeamCount() < tmpsn->getNumFaehnchen())
						{
							sn2 = tmpsn;
							sn2->incBeamCount();
							continue;
						}
						else
						{
							partialbeam = 1;
							break;
						}
					}
					if (oldpos2 == sse->endpos)
						break;
				}
				
				if (sn2)
				{
					if (!partialbeam && sse->endflag == GRSystemStartEndStruct::OPENRIGHT)
					{
						// then the position is different ...
						myp[2] = sse->endElement->getPosition();
						myp[2].x += xadjust;
						if (tagtype == SYSTEMTAG)
							myp[2] += sn2->getGRStaff()->getPosition();
						myp[2].y = myp[0].y;						
					}
					else
					{
						// we have an End-Position ...
						myp[2] = sn2->getStemEndPos();
						myp[2].x += xadjust;
						if (tagtype == SYSTEMTAG)
							myp[2] += sn2->getGRStaff()->getPosition();
						myp[2].y += beamCount * yLocalFact1;
					}
					if (localDir != dir)
						myp[2].y -= yLocalFact2;
					myp[3].x = myp[2].x;
					myp[3].y = myp[2].y + yLocalFact2;
				}
				else
				{
					// we do not have an End-Positon single beam ... (meaning a single straight flag)
					// but only, if it is not open on the left or the right hand side.
					const float slope = (float)(st->p[2].y - st->p[0].y ) / (float)(st->p[2].x - st->p[0].x);

					if (sse->startflag == GRSystemStartEndStruct::OPENLEFT)
					{
						// then we have to deal with the startposition of the glue-element ....
						// BUT, you can only set this, if the previous beam had this beamcount at the end ....
						// how do I know that? not now.

						// sn is the only element .... and we are open on the left ...
						if (tagtype == SYSTEMTAG)
							myp[0] += stemNote->getGRStaff()->getPosition();
						
						myp[1] = myp[0];
						myp[2] = stemNote->getStemEndPos();
						myp[2].x += xadjust;
						if (tagtype == SYSTEMTAG)
							myp[2] += stemNote->getGRStaff()->getPosition();
						myp[2].y += beamCount * yLocalFact1;
						if (localDir != dir)
							myp[2].y -= yLocalFact2;
						myp[3] = myp[2];
						myp[3].y += yLocalFact2;
					}
					else if (sse->endflag == GRSystemStartEndStruct::OPENRIGHT)
					{						
						myp[0] = stemNote->getStemEndPos();
						if (tagtype == SYSTEMTAG)
							myp[0] += stemNote->getGRStaff()->getPosition();
						myp[0].y += beamCount * yLocalFact1;
						if (localDir != dir)
							myp[0].y -= yLocalFact2;
						myp[1] = myp[0];
						myp[1].y += yLocalFact2;

						myp[2] = sse->endElement->getPosition();
						myp[2].x += xadjust;
						if (tagtype == SYSTEMTAG)
							myp[2] += sn2->getGRStaff()->getPosition();
						myp[2].y = myp[0].y;
						
						myp[3].x = myp[2].x;
						myp[3].y = myp[1].y;
					}

					/* 26/11/03 
					 Beaming bug: wrong direction for partial beam (beam-bug.gmn)
					 can be tested but changing this test. 				 

						startpos check added to correct problem with partial beam
						going outside a group like in [ _/16 c d/8 ]
					*/
					else if( oldpos == sse->endpos || pos == NULL || ((!stemNote->isSyncopated()) && (oldpos != sse->startpos)))
					{
						// Partial beams leftward ( using slope)
						myp[2] = stemNote->getStemEndPos();
						myp[2].x += xadjust;
						if (tagtype == SYSTEMTAG)
							myp[2] += stemNote->getGRStaff()->getPosition();
						
						if (localDir != dir)
							myp[2].y -= yLocalFact2;

						myp[2].y += beamCount * yLocalFact1;
						myp[3] = myp[2];
						myp[3].y += yLocalFact2;

						myp[0] = myp[2];
						myp[0].x -= infos.currentLSPACE;
						myp[0].y -= slope * infos.currentLSPACE;
						myp[1] = myp[0];
						myp[1].y += yLocalFact2;

						// ? We are at the end, there is no valid event at the end ... so what do we do?
						// useless tests: the code was the same and is now outside the test (above) - DF sept 15 2009
/*
						if (sse->endflag == GRSystemStartEndStruct::OPENRIGHT)
						{
						}
						else
						{
						}
*/
					}
					else
					{
						// Partial beams rightward ( using slope)
						myp[2] = myp[0];
						myp[2].x += infos.currentLSPACE;
						myp[2].y += slope * infos.currentLSPACE;
						myp[3] = myp[2];
						myp[3].y += yLocalFact2;
					}
				}
				
				// now we construct a SimpleBeam, we now have to "undo" the systemTag-stuff
				if (tagtype == SYSTEMTAG)
				{
					const NVPoint & offset = beamstaff->getPosition();
					myp[0] -= offset;
					myp[1] -= offset;
					myp[2] -= offset;
					myp[3] -= offset;
				}

				if (sse->startflag == GRSystemStartEndStruct::OPENLEFT)
				{
					myp[0].y = myp[2].y;
					myp[1].y = myp[3].y;
				}

				GRSimpleBeam * tmpbeam = new GRSimpleBeam(this,myp);
				if( st->simpleBeams == 0 )
					st->simpleBeams = new SimpleBeamList(1);

				st->simpleBeams->AddTail(tmpbeam);
				
				pos  = sse->startpos;
				oldpos = pos;
				first = true;
				lastLocalDir = localDir;
				previousBeamsCount = stemNote->getBeamCount() - 1;
			}
			// a new hack, again to catch stems directions change - DF sept 15 2009
			else if (localDir != dir) {
				// check for stems length
				NVPoint stemloc = stemNote->getStemStartPos();
				if (tagtype == SYSTEMTAG)
					stemloc += stemNote->getGRStaff()->getPosition();
				int beamscount = stemNote->getBeamCount() - 1;
				if ((beamscount > 0) && (previousBeamsCount > beamscount) && (lastLocalDir != localDir)) {
					if (localDir == dirUP)
						stemNote->changeStemLength(stemNote->getStemLength() + (yLocalFact1 * beamscount));
					else if (localDir == dirDOWN)
						stemNote->changeStemLength(stemNote->getStemLength() - (yLocalFact1 * beamscount));
				}			
			}
		}
		if (oldpos == sse->endpos)
			break;
	}
	
	// now we have to make sure, that the original positions
	// for the beam are set for the right staff
	if (tagtype == SYSTEMTAG)
	{
		const NVPoint &offset = beamstaff->getPosition();
		st->p[0] -= offset;
		st->p[1] -= offset;
		st->p[2] -= offset;
		st->p[3] -= offset;
	}
}


/** \brief Gets an X-Value and gives back the needed y-Value.
	Right now, I need to restrict slopes to certain values. (min and max)
*/
int GRBeam::slope(GRBeamSaveStruct * st, int posx)
{
	return 0;
}

double GRBeam::getSlope(GRStaff * grstaff)						
{
	return 0.0;
}

void GRBeam::setError(const GRStaff * grstaff, int p_error)
{
	GRTag::setError(p_error);
	if (error && mAssociated)
	{
		GuidoPos pos = mAssociated->GetHeadPosition();
		while (pos)
		{
			GREvent * grnot = GREvent::cast (mAssociated->GetNext(pos));
			if (grnot)
			{
				grnot->setFlagOnOff(1);
			}
		}
	}

	// this just removes the associations from
	// all active SSE's. The SSE's themselves
	// stay, where they are, so that they are
	// delete properly later ....
	DeleteAllSSEs();

	delete mAssociated;
	mAssociated = NULL;
}

void GRBeam::checkNotes( GRStaff * grstaff )
{
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(grstaff->getGRSystem());
	if (!sse) return;

//	GRBeamSaveStruct *st = (GRBeamSaveStruct *) sse->p;
	if (mAssociated)
	{
		GuidoPos pos = sse->startpos;
		bool last = false;
		while (pos)
		{
			GREvent * grnot = GREvent::cast(mAssociated->GetNext(pos));
			if (!grnot) 
			{
				setError( grstaff, 1 );
				break;
			}

			if (last)
				break;

			if (pos == sse->endpos)
				last = true;
		}
	}
}

/** \brief Called after the new Staff has been created by the
	Staffmanager after a pbreak-Break.
	(It is ALWAYS called after a call to BreakTag)
	It is somewhat analogous to StaffBegin
	but uses the assocpos ...
*/
void GRBeam::ResumeTag(GRStaff *grstaff,GuidoPos assocpos)
{
	if (!grstaff) return;
	GRPositionTag::ResumeTag(grstaff,assocpos);
}

/** \brief Break a tag at a previously saved PBreak-Location.

	It sets the sse-Positions ...
	(It is somewhat equivalent to StaffFinished
	with the difference, that assocpos is
	used). assocpos is the tail-position of the
	mAssociated list at the time of the pbreak-
	situation.
*/
void GRBeam::BreakTag( GRStaff * grstaff, GuidoPos & assocpos)
{
	if (!grstaff) return;

	// do the "basic" stuff.
	GRPositionTag::BreakTag(grstaff,assocpos);

	if (error) return;

	GRSystemStartEndStruct *sse = getSystemStartEndStruct(grstaff->getGRSystem());
	assert(sse);
//	GRBeamSaveStruct *st = (GRBeamSaveStruct *) sse->p;
	GREvent * grn;

	checkNotes(grstaff);
	if (error) return;

	GuidoPos pos = sse->startpos;
	while (pos)
	{
		GuidoPos prevpos = pos;
		grn = GREvent::cast(mAssociated->GetNext(pos));
		if (grn)
			grn->setFlagOnOff(0);

		if (prevpos == sse->endpos)
			break;
	}
}

void GRBeam::checkPosition( const GRSystem * grsys)
{
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(grsys);
	assert(sse);

	mIsSystemCall = true;
	tellPosition( sse->endElement, sse->endElement->getPosition());
	mIsSystemCall = false;
}
