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
    else if (type == "///")
        fNumberOfStrokes = 3;
    else if( type == "////")
        fNumberOfStrokes = 4;
    else // default
        fNumberOfStrokes = 3;
    
    fPitch = artrem->getPitch() ? artrem->getPitch()->getValue() : "";
    if(fPitch != "")
        isTwoNotesTremolo = true;
    
    fText = artrem->getText() ? artrem->getText()->getValue() : "";
    fThickness = artrem->getThickness() ? artrem->getThickness()->getValue() : LSPACE/2;
    dx = artrem->getDx() ? artrem->getDx()->getValue() : 0;
    dy = artrem->getDy() ? artrem->getDy()->getValue() : 0;
    
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
    
    float coorX[4];
    float coorY[4];
    
    if(isTwoNotesTremolo)
    {
        NVPoint pos1 = fStartPos;
        NVPoint pos2 = fEndPos;
        for(int i = 0; i < fNumberOfStrokes ; i++)
        {
            computeTwoNotesStrokesCoordinates(pos1, pos2, coorX, coorY);
            hdc.Polygon(coorX, coorY, 4);
            pos1.y += fStep;
            pos2.y += fStep;
        }
    }
    else
    {
        NVPoint pos = fStartPos;
        for(int i = 0; i < fNumberOfStrokes ; i++)
        {
            computeSimpleStrokesCoordinates(pos, coorX, coorY);
            hdc.Polygon(coorX, coorY, 4);
            pos.y += fStep;
        }
    }
    GRPTagARNotationElement::OnDrawText(hdc,fText.c_str());
}

// -----------------------------------------------------------------------------
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
    
    GRSingleNote * sng = dynamic_cast<GRSingleNote *>(grel);
    NVPoint pos;
    NVPoint textPos;
    if (sng != 0)
    {
        pos = np;
        pos.x = textPos.x = sng->getStemStartPos().x;
        GDirection direction = sng->getStemDirection();
        if(direction == dirUP)
        {
            textPos.y = np.y - (2*LSPACE + sng->getStemLength());
            if(textPos.y > - 2*LSPACE )
                textPos.y = - 2*LSPACE;
            pos.y -= LSPACE/2 + sng->getStemLength()/2;
        }
        else
        {
            textPos.y = np.y + sng->getStemLength();
            if(textPos.y < 4*LSPACE)
                textPos.y = 4*LSPACE;
            pos.y += LSPACE/2 + sng->getStemLength()/2;
        }
        pos.y -= (fNumberOfStrokes-1)*fStep/2;
    }
    else
    {
        GRGlobalStem * stem = findGlobalStem( sse, grel );
        NEPointerList * list = staff->getElements();
        GuidoPos gpos = list->GetElementPos(grel);
        bool foundStem = false;
        while(gpos != sse->startpos && !foundStem )
        {
            GRNotationElement * prevEl = list->GetPrev(gpos);
            stem = prevEl ? findGlobalStem( sse, prevEl ) : 0 ;
            if(stem)
                foundStem = true;
        }
        if(foundStem)
        {
            pos = textPos = stem->getStemStartPos();
            GDirection direction = stem->getStemDir();
            if(direction == dirUP)
            {
                float upperNoteY = stem->getAssociations()->GetHead()->getPosition().y;
                float realStemLength = upperNoteY - stem->getStemStartPos().y;
                pos.y = upperNoteY - realStemLength/2 - LSPACE/2;
                textPos.y = upperNoteY - realStemLength - 2*LSPACE;
                if(textPos.y > -2*LSPACE )
                    textPos.y = - 2*LSPACE;
            }
            else
            {
                float lowerNoteY = stem->getAssociations()->GetTail()->getPosition().y;
                float realStemLength = stem->getStemStartPos().y - lowerNoteY;
                pos.y = lowerNoteY + realStemLength/2 + LSPACE/2;
                textPos.y = lowerNoteY + realStemLength;
                if(textPos.y < 4*LSPACE)
                    textPos.y = 4*LSPACE;
            }
            pos.y -= (fNumberOfStrokes-1)*fStep/2;
        }
    }
    
    const GRNotationElement * startElement = sse->startElement;
	const GRNotationElement * endElement = sse->endElement;
	if(grel == startElement)
    {
        fStartPos = pos;
        setPosition(textPos);
    }
    else if (grel == endElement)
        fEndPos = pos;
}


// -----------------------------------------------------------------------------
void GRTremolo::computeSimpleStrokesCoordinates( NVPoint pos, float coorX[4], float coorY[4] ) const
{
    float x = pos.x + dx;
    float y = pos.y + dy;
    
    coorX[0] = coorX[1] = x - fWidth/2;
    coorX[2] = coorX[3] = x + fWidth/2;
    
    coorY[0] = y+(fDeltaY+fThickness)/2;
    coorY[1] = y+(fDeltaY-fThickness)/2;
    coorY[2] = y-(fDeltaY+fThickness)/2;
    coorY[3] = y-(fDeltaY-fThickness)/2;
}

// -----------------------------------------------------------------------------
void GRTremolo::computeTwoNotesStrokesCoordinates( NVPoint firstPos, NVPoint endPos, float coorX[4], float coorY[4] ) const
{

    float xFirst = firstPos.x + dx;
    float xEnd = endPos.x + dx;
    float yFirst = firstPos.y + dy;
    float yEnd = endPos.y + dy;
    
    coorX[0] = coorX[1] = xFirst + fWidth/2;
    coorX[2] = coorX[3] = xEnd - fWidth/2;
    
    coorY[0] = yFirst+fThickness/2;
    coorY[1] = yFirst-fThickness/2;
    coorY[2] = yEnd-fThickness/2;
    coorY[3] = yEnd+fThickness/2;
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
