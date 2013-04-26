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

#include "GRCluster.h"
#include "GRStaff.h"

#include "ARCluster.h"

#include "VGDevice.h"
#include "GRSingleNote.h"

GRCluster::GRCluster(GRStaff * stf, ARCluster * arcls) :
						GRARCompositeNotationElement(arcls),
						GRPositionTag(arcls->getEndPosition(), arcls), mHaveBeenDrawn(false), mStaff(stf), mARCluster(arcls)
{
	assert(stf);
	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
	sse->grsystem = stf->getGRSystem();
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;

	sse->p = (void *) getNewGRSaveStruct();

	mStartEndList.AddTail(sse);

    
    int *firstNoteParameters = mARCluster->getFirstNoteParameters();

    mFirstNoteYPosition = stf->getNotePosition(firstNoteParameters[0], firstNoteParameters[1]);
    mSecondNoteYPosition = mFirstNoteYPosition;
}

GRCluster::~GRCluster() {}

void GRCluster::OnDraw(VGDevice &hdc)
{
    if (mHaveBeenDrawn == false)
    {
        if (mFirstNoteYPosition != mSecondNoteYPosition)
            mHaveBeenDrawn = true;

        NVRect r = getBoundingBox();
        r += getPosition();

        float x = r.right;

        float curLSpace = mStaff->getStaffLSPACE();

        const float xCoords [] = {x, x + 50, x + 50, x};
        const float yCoords [] = {mFirstNoteYPosition - curLSpace / 2, mFirstNoteYPosition - curLSpace / 2,
            mSecondNoteYPosition + curLSpace / 2, mSecondNoteYPosition + curLSpace / 2};

        hdc.Polygon(xCoords, yCoords, 4);
    }
    else
        mHaveBeenDrawn = false;
}

void GRCluster::addAssociation(GRNotationElement *p)
{
	GRARCompositeNotationElement::addAssociation(p);
}

GuidoPos GRCluster::AddTail(GRNotationElement * el)
{
	GuidoPos mypos = GRARCompositeNotationElement::AddTail(el);

	// now, we remove the associatons ...
	// of those elements, that are members of 
	// the composite-sructure ...

	GuidoPos elpos = NULL;
	if (mAssociated && (elpos = mAssociated->GetElementPos(el)) != NULL)
	{
		mAssociated->RemoveElementAt(elpos);
	}

	return mypos;
}

//-------------------------------------------------------------------
void GRCluster::tellPosition(GObject * caller, const NVPoint & np)
{
	GREvent *ev = dynamic_cast<GREvent *>(caller);
	
    if (ev)
	{
        NVPoint newPoint (np);
        newPoint.y = -LSPACE;

        setPosition(newPoint);
	}
}

void GRCluster::setSecondNoteYPosition()
{
    int *secondNoteParameters = mARCluster->getSecondNoteParameters();

    mSecondNoteYPosition = mStaff->getNotePosition(secondNoteParameters[0], secondNoteParameters[1]);

    if (mFirstNoteYPosition > mSecondNoteYPosition)
    {
        float tmpSwitch = mSecondNoteYPosition;
        mSecondNoteYPosition = mFirstNoteYPosition;
        mFirstNoteYPosition = tmpSwitch;
    }
}