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

// #include "Bitmap.h"
#include "ARDynamics.h"

#include "GRDynamics.h"
#include "GRNote.h"
#include "GRStaff.h"
#include "GRRest.h"
#include "GRGlue.h"
#include "GRDefine.h"
// #include "NEPointerList.h"

#define MAX(A,B)(A>B?A:B)
#define MIN(A,B)(A>B?B:A)

GRDynamics::GRDynamics(GRStaff * pstaff)
	: GRPTagARNotationElement(new ARDynamics,1) // owns AR
{
	assert(pstaff);
	
	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
	GRDynamicsSaveStruct * st = new GRDynamicsSaveStruct;

	sse->p = (void *) st;
	sse->grsystem = pstaff->getGRSystem();
	sse->startElement = NULL;
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	sse->endElement = NULL;

	mStartEndList.AddTail(sse);
}

GRDynamics::GRDynamics(GRStaff * pstaff,ARDynamics * abstractRepresentationOfDynamic)
	: GRPTagARNotationElement(abstractRepresentationOfDynamic)
{
	assert(abstractRepresentationOfDynamic);

	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
	GRDynamicsSaveStruct * st = new GRDynamicsSaveStruct;
	st->d_pos = abstractRepresentationOfDynamic->location == 1 ? 
		GRDynamicsSaveStruct::D_BOTTOM : 
		GRDynamicsSaveStruct::D_TOP;
	sse->p = (void *) st;
	sse->grsystem = pstaff->getGRSystem();
	sse->startElement = NULL;
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	sse->endElement = NULL;

	mStartEndList.AddTail(sse);
}


GRDynamics::~GRDynamics()
{
	delete mAssociated;
	mAssociated = 0;
}

/** \brief returns -1 on error.
*/
int GRDynamics::updateDynamic(const GRStaff * pstaff) 
{
	if (!pstaff) return -1;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct(pstaff->getGRSystem());

	assert(sse);

	GRDynamicsSaveStruct * st = (GRDynamicsSaveStruct *)sse->p;
	assert(st);

//	GRNotationElement *startElement = sse->startElement;

	NVRect startRect;
	NVPoint startPoint;
	NVRect endRect;
	NVPoint endPoint;

	GRNotationElement * e = mAssociated->GetAt(sse->startpos);

	startRect = e->getBoundingBox();
	GREvent * ev = GREvent::cast(e);
	if (ev)
		startPoint = ev->getCrescStart(); 
	else
		startPoint = e->getPosition();

	st->position.x = startPoint.x;

	// position.y bestimmen anhand der tiefsten/höchsten Note
	if (st->d_pos == GRDynamicsSaveStruct::D_BOTTOM) 
		st->position.y = startPoint.y + startRect.bottom;
	else if (st->d_pos == GRDynamicsSaveStruct::D_TOP) 
		st->position.y = startPoint.y - startRect.top;

	GuidoPos pos = sse->startpos;

	GCoord dummy;
	bool last = false;
	while (pos)  
	{
		e = mAssociated->GetNext(pos);
		if (st->d_pos == GRDynamicsSaveStruct::D_BOTTOM) 
		{
			dummy = e->getPosition().y + e->getBoundingBox().bottom;
			st->position.y = MAX(st->position.y, dummy);
		} 
		else if (st->d_pos == GRDynamicsSaveStruct::D_TOP) 
		{
			dummy = e->getPosition().y - e->getBoundingBox().top;
			st->position.y = MIN(st->position.y, dummy);
		}

		if (last)
			break;
	
		if (pos == sse->endpos)
			last = true;
	}

	endRect = e->getBoundingBox();
	ev = GREvent::cast(e);
	if (ev)
		endPoint = ev->getCrescEnd();
	else
		endPoint = e->getPosition();


	const float staffwidth = LSPACE * 4; // width of a staff
	if (st->d_pos == GRDynamicsSaveStruct::D_BOTTOM) 
	{
		st->position.y = (GCoord)MAX( (float)st->position.y, staffwidth);
		st->position.y += (GCoord)LSPACE; // insert some space
	} 
	else if (st->d_pos == GRDynamicsSaveStruct::D_TOP) 
	{
		st->position.y = (GCoord)MIN(st->position.y, 0);
		st->position.y-= (GCoord)(2 * LSPACE); // some space
	}

	st->boundingBox.left = 0;
	st->boundingBox.top = LSPACE / 4;
	st->boundingBox.bottom = LSPACE / 4;
	st->boundingBox.right = endPoint.x + endRect.right - st->position.x;

	// Preparation fo  Advanced Guido
	// if (d_pos == D_FREE) position.y=freepos

	return 1;
}


void GRDynamics::OnDraw( VGDevice & ) const 
{
}

void GRDynamics::print() const
{
}


void GRDynamics::tellPosition(GObject * caller, const NVPoint & newPosition)
{
	GRNotationElement * grel = dynamic_cast<GRNotationElement *>(caller);
	if (!grel || grel->getGRStaff() == NULL)
	{
		return;
	}

	GRSystemStartEndStruct * sse = 
		getSystemStartEndStruct(grel->getGRStaff()->getGRSystem());

	if (!sse)
		return;
	
	
	GRNotationElement * startElement = sse->startElement;
	GRNotationElement * endElement = sse->endElement;
	
	if ( grel == endElement ||
		( endElement == NULL && grel == startElement) )
	{
		updateDynamic(grel->getGRStaff());
	}
}

/** \brief Called by the notes inbetween the range.
*/
void GRDynamics::addAssociation(GRNotationElement * grnot)
{
	if (error) return;

// Not only events can be associated with dynamics!
//	if (GREvent::cast(grnot))
   GRARNotationElement::addAssociation(grnot);

   // this deals with setting the startElement
   // if needed ....
   GRPositionTag::addAssociation(grnot);

	// maybe we do somethings else here? ->
	// like setting flags ...

}

void GRDynamics::removeAssociation(GRNotationElement * el)
{
	GRPositionTag::removeAssociation(el);
	GRARNotationElement::removeAssociation(el);
	
}

