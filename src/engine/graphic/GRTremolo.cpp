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
#include "GRBeam.h"
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
    
    fIsTwoNotesTremolo = false;
    fStep = LSPACE/2;
    fDeltaY = LSPACE/4;
    fWidth = LSPACE;
    fBeamCount = 0;
    fNumberOfStrokes = artrem->getNumberOfStrokes();
    if(artrem->isSecondPitchCorrect())
        fIsTwoNotesTremolo = true;
    
    fText = artrem->getText() ? artrem->getText()->getValue() : "";
    fThickness = artrem->getThickness() ? artrem->getThickness()->getValue() : LSPACE/2;
    dx = artrem->getDx() ? artrem->getDx()->getValue() : 0;
    dy = artrem->getDy() ? (-1) * artrem->getDy()->getValue() : 0;
    
}

// -----------------------------------------------------------------------------
GRTremolo::~GRTremolo()
{
}

// -----------------------------------------------------------------------------
void GRTremolo::OnDraw( VGDevice & hdc ) const
{
	if(!mDraw)
		return;

    if (mColRef) {
        VGColor color(mColRef);
        hdc.PushFillColor(color);
    }
    
    float coorX[4];
    float coorY[4];
    NVPoint pos1 = fStartPos;
    NVPoint pos2 = fEndPos;
    for(int i = 0; i < (fNumberOfStrokes - fBeamCount) ; i++)
    {
        if(fIsTwoNotesTremolo)
        {
            computeTwoNotesStrokesCoordinates(pos1, pos2, coorX, coorY);
            pos2.y += fStep;
        }
        else
            computeSimpleStrokesCoordinates(pos1, coorX, coorY);
        hdc.Polygon(coorX, coorY, 4);
        pos1.y += fStep;
    }
    GRPTagARNotationElement::OnDrawText(hdc,fText.c_str());

    if (mColRef)
        hdc.PopFillColor();
}

// -----------------------------------------------------------------------------
void GRTremolo::tellPosition(GObject * caller, const NVPoint &np)
{
    GREvent * grel = dynamic_cast<GREvent *>(caller);
	if (grel == 0)
        return;

	GRStaff * staff = grel->getGRStaff();
	if (staff == 0)
        return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct( staff->getGRSystem());
	if (sse == 0)
        return;
    
    GRSingleNote * sng = dynamic_cast<GRSingleNote *>(grel);
    NVPoint pos;
    NVPoint textPos;
    
    GRNotationElement * startElement = sse->startElement;
	
    if(grel == startElement)
        fBeamCount = grel->getNumFaehnchen();
    
    if (sng != 0)
    {
        GDirection direction = sng->getStemDirection();
        pos = textPos = sng->getStemStartPos();

        if (direction == dirOFF)
        {
            textPos.y -= 2.5f * LSPACE;

            if (textPos.y > -2.5f * LSPACE)
                textPos.y = -2.5f * LSPACE;
            
            if (pos.y > 2 * LSPACE)
                pos.y -= 3 * LSPACE;
            else
                pos.y += 2 * LSPACE;
        }
        else if (direction == dirUP)
        {
            if (!fIsTwoNotesTremolo)
                pos.y += LSPACE / 2 + fBeamCount * fStep;
            else
                pos.y += fBeamCount * fStep * 1.5f;

            textPos.y -= 2*LSPACE;

            if (textPos.y > - 2 * LSPACE )
                textPos.y = - 2 * LSPACE;
        }
        else
        {
            pos.y -= fThickness + (fNumberOfStrokes-1) * fStep;

            if (!fIsTwoNotesTremolo)
                pos.y -= LSPACE / 2;
            else
                pos.y -= fBeamCount * fStep * 0.5f;

            if (textPos.y < 4*LSPACE)
                textPos.y = 4*LSPACE;
        }
    }
    else
    {
        GRGlobalStem * stem = findGlobalStem( sse, grel );
        NEPointerList * list = staff->getElements();
        GuidoPos gpos = list->GetElementPos(grel);
        bool foundStem = false;
        while(gpos && gpos != sse->startpos && !foundStem )
        {
            GRNotationElement * prevEl = list->GetPrev(gpos);
            stem = prevEl ? findGlobalStem( sse, prevEl ) : 0 ;
            if(stem) foundStem = true;
        }
        if(foundStem)
        {
            pos = textPos = stem->getStemStartPos();
            GDirection direction = stem->getStemDir();
            if(direction == dirOFF)
            {
                float upperNoteY = stem->getAssociations()->GetHead()->getPosition().y;
                float lowerNoteY = stem->getAssociations()->GetTail()->getPosition().y;
                textPos.y = upperNoteY -2.5f * LSPACE;
                if (textPos.y > -2.5 * LSPACE)
                    textPos.y = -2.5f * LSPACE;
                
                if (upperNoteY > 2*LSPACE)
                    pos.y = upperNoteY - 3 * LSPACE;
                else
                    pos.y = lowerNoteY + 2 * LSPACE;
            }
            else if (direction == dirUP)
            {
                if (!fIsTwoNotesTremolo)
                    pos.y += LSPACE / 2 + fBeamCount * fStep;
                else
                    pos.y += fBeamCount * fStep * 1.5f;

                textPos.y -= 2 * LSPACE;
                if(textPos.y > - 2 * LSPACE ) textPos.y = - 2 * LSPACE;
            }
            else
            {
                pos.y -= fThickness + (fNumberOfStrokes - 1) * fStep;
                
                if (!fIsTwoNotesTremolo)
                    pos.y -= LSPACE / 2;
                else
                    pos.y -= fBeamCount * fStep * 0.5f;

                if (textPos.y < 4 * LSPACE)
                    textPos.y = 4 * LSPACE;
            }
        }
    }
    
    if(grel == startElement)
    {
        fStartPos = pos;
        setPosition(textPos);
    }
    else
        fEndPos = pos;
}


// -----------------------------------------------------------------------------
void GRTremolo::computeSimpleStrokesCoordinates( NVPoint pos, float coorX[4], float coorY[4] ) const
{
    float x = pos.x + dx;
    float y = pos.y + dy;
    
    coorX[0] = coorX[1] = x - fWidth/2;
    coorX[2] = coorX[3] = x + fWidth/2;
    
    coorY[0] = y + fDeltaY/2 + fThickness;
    coorY[1] = y + fDeltaY/2;
    coorY[2] = y - fDeltaY/2;
    coorY[3] = y - fDeltaY/2 + fThickness;
}

// -----------------------------------------------------------------------------
void GRTremolo::computeTwoNotesStrokesCoordinates( NVPoint firstPos, NVPoint endPos, float coorX[4], float coorY[4] ) const
{
    
    float xFirst = firstPos.x + dx;
    float xEnd = endPos.x + dx;
    float yFirst = firstPos.y + dy;
    float yEnd = endPos.y + dy;

    float slopeY = (yEnd - yFirst)/(xEnd - xFirst) * fWidth/2;

    coorX[0] = coorX[1] = xFirst + fWidth/2;
    coorX[2] = coorX[3] = xEnd - fWidth/2;
    
    coorY[0] = yFirst + fThickness + slopeY;
    coorY[1] = yFirst + slopeY;
    coorY[2] = yEnd - slopeY;
    coorY[3] = yEnd + fThickness - slopeY;
}


// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
unsigned int GRTremolo::getTextAlign() const
{ 
	return (VGDevice::kAlignLeft | VGDevice::kAlignTop); 
}