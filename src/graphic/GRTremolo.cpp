/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARTremolo.h"
#include "GRStaff.h"
#include "GRTremolo.h"
#include "VGDevice.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "TagParameterInt.h"
#include "GuidoParser.h"
#include "GUIDOInternal.h"
#include "ARMusic.h"
#include "ARNote.h"
#include "GRSingleNote.h"
#include "GRStdNoteHead.h"
// #include "NEPointerList.h"


GRTremolo::GRTremolo( GRStaff * stf, ARTremolo * artrem )
					 	: GRPTagARNotationElement(artrem)				
{
	assert(stf);
	GRSystemStartEndStruct * sse= new GRSystemStartEndStruct;
	sse->grsystem = stf->getGRSystem(); 
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	
	sse->p = (void *) getNewGRSaveStruct();

	mStartEndList.AddTail(sse);
    
    isTwoNotesTremolo = false;
    fStep = LSPACE/2;
    fDeltaY = LSPACE/4;
    fWidth = LSPACE;
    
    std::string type = artrem->getStyle() ? artrem->getStyle()->getValue() : "";
    if( type == "/" )
        fNumberOfStrokes = 1;
    else if( type == "//" )
        fNumberOfStrokes = 2;
    else if( type == "////")
        fNumberOfStrokes = 4;
    else // default
        fNumberOfStrokes = 3;
    
    fPitch = artrem->getPitch() ? artrem->getPitch()->getValue() : "";
    
    fText = artrem->getText() ? artrem->getText()->getValue() : "";
    fThickness = artrem->getThickness() ? artrem->getThickness()->getValue() : LSPACE/2;
    dx = artrem->getDx() ? artrem->getDx()->getValue() : 0;
    dy = artrem->getDy() ? artrem->getDy()->getValue() : 0;
    
}

GRTremolo::~GRTremolo()
{
}

void GRTremolo::OnDraw( VGDevice & hdc ) const
{
	if(!mDraw)
		return;
    
    float coorX[4];
    float coorY[4];
    NVPoint pos = getPosition();
    
    for(int i = 0; i < fNumberOfStrokes ; i++)
    {
        computeStrokesCoordinates(pos, coorX, coorY);
        hdc.Polygon(coorX, coorY, 4);
        pos.y += fStep;
    }
    GRPTagARNotationElement::OnDrawText(hdc,fText.c_str());
}

void GRTremolo::tellPosition(GObject * caller, const NVPoint &np)
{
    GRNotationElement * grel = dynamic_cast<GRNotationElement *>(caller);
	if (grel == 0)
        return;

	GRStaff * staff = grel->getGRStaff();
	if (staff == 0)
        return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct( staff->getGRSystem());
	if (sse == 0)
        return;

    updateTremolo(staff);
    
    GRSingleNote * sng = dynamic_cast<GRSingleNote *>(grel);
    NVPoint pos;
    if (sng != 0)
    {
        pos = np;
        pos.x = sng->getStemStartPos().x;
        GDirection direction = sng->getStemDirection();
        if(direction == dirUP)
        {
            pos.y -= LSPACE/2 + sng->getStemLength()/2;
        }
        else
        {
            pos.y += LSPACE/2 + sng->getStemLength()/2;
        }
        pos.y -= (fNumberOfStrokes-1)*fStep/2;
        setPosition(pos);
    }
    else
    {
        GRGlobalStem * stem = findGlobalStem( sse, grel );
        if(stem)
        {
            pos = stem->getStemStartPos();
            GDirection direction = stem->getStemDir();
            if(direction == dirUP)
            {
                float upperNoteY = stem->getAssociations()->GetHead()->getPosition().y;
                float realStemLength = upperNoteY - stem->getStemStartPos().y;
                pos.y = upperNoteY - realStemLength/2 - LSPACE/2;
            }
            else
            {
                float lowerNoteY = stem->getAssociations()->GetTail()->getPosition().y;
                float realStemLength = stem->getStemStartPos().y - lowerNoteY;
                pos.y = lowerNoteY + realStemLength/2 + LSPACE/2;
            }
            pos.y -= (fNumberOfStrokes-1)*fStep/2;
            setPosition(pos);
        }
    }
}

void GRTremolo::updateTremolo( GRStaff * inStaff )
{
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(inStaff->getGRSystem());
	if (sse == 0)
        return;

	ARTremolo * artrem = static_cast<ARTremolo *>(getAbstractRepresentation());
    assert(artrem);

}

void GRTremolo::computeStrokesCoordinates( NVPoint pos, float coorX[4], float coorY[4] ) const
{
    float x = pos.x;
    float y = pos.y;
    
    coorX[0] = coorX[1] = x - fWidth/2;
    coorX[2] = coorX[3] = x + fWidth/2;
    
    coorY[0] = y+(fDeltaY+fThickness)/2;
    coorY[1] = y+(fDeltaY-fThickness)/2;
    coorY[2] = y-(fDeltaY+fThickness)/2;
    coorY[3] = y-(fDeltaY-fThickness)/2;
}

GRGlobalStem * GRTremolo::findGlobalStem( GRSystemStartEndStruct * sse, GRNotationElement * stemOwner ) const
{
	const NEPointerList * ptlist1 = stemOwner->getAssociations();
	if (ptlist1)
	{
		GuidoPos nepos = ptlist1->GetHeadPosition();
		while (nepos)
		{
			GRGlobalStem * stem = dynamic_cast<GRGlobalStem *>(ptlist1->GetNext(nepos));
			if (stem)
				return stem;
		}
	}
	return 0;
}

unsigned int GRTremolo::getTextAlign() const
{ 
	return (VGDevice::kAlignLeft | VGDevice::kAlignTop); 
}
