#include <cstdlib>
#include "math.h"
#include "VGDevice.h"

#include "ARGlissando.h"
#include "ARNote.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "GRStaff.h"
#include "GRGlue.h"
#include "GRSingleNote.h"
#include "GRAccidental.h"
#include "GRGlobalStem.h"
#include "GRRest.h"
#include "GREmpty.h"
//#include "GRChord.h"
#include "GRStdNoteHead.h"
#include "GraphTools.h"

#include "GRGlissando.h"

extern GRSystem * gCurSystem;

using namespace std;


// -----------------------------------------------------------------------------
GRGlissando::GRGlissando(GRStaff * grstaff)
	: GRPTagARNotationElement( new ARGlissando, true ) // ownsAR
{
	initGRGlissando( grstaff );
	flaststartElement = NULL;
	nextGRGlissando = NULL;
	wavy = false;
	fill = false;
}

// -----------------------------------------------------------------------------
GRGlissando::GRGlissando(GRStaff * grstaff, ARGlissando * abstractRepresentationOfGlissando)
				   : GRPTagARNotationElement(abstractRepresentationOfGlissando)
{
	assert(abstractRepresentationOfGlissando);
	initGRGlissando( grstaff );
	flaststartElement = NULL;
		
	wavy = abstractRepresentationOfGlissando->isWavy();
	fill = abstractRepresentationOfGlissando->isFill();
}

GRGlissando::~GRGlissando()
{
	assert(mStartEndList.GetCount() == 0);
	FreeAssociatedList();
}

GRSystemStartEndStruct * GRGlissando::initGRGlissando( GRStaff * grstaff )
{
	assert(grstaff);

	setGRStaff( grstaff );// TEST

	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
	sse->grsystem = grstaff->getGRSystem();

	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	sse->endflag = GRSystemStartEndStruct::RIGHTMOST;

	mStartEndList.AddTail(sse);
		
	fglissInfos = new GRGlissandoSaveStruct;
	fglissInfos->numPoints = 4;

	sse->p = (void *)fglissInfos;
	
	return sse;
}

void GRGlissando::print() const
{
}

void GRGlissando::OnDraw( VGDevice & hdc ) const
{
	if (error) return;
	assert( gCurSystem );

	GRSystemStartEndStruct * sse = getSystemStartEndStruct( gCurSystem );
	if( sse == 0)
		return; // don't draw

	if (mColRef) 
		hdc.PushFillColor( VGColor( mColRef ) );
	
	//straight line
	if(!wavy)
	{
		float coorX[4] = {fglissInfos->points[0].x, fglissInfos->points[1].x, fglissInfos->points[2].x, fglissInfos->points[3].x};
		float coorY[4] = {fglissInfos->points[0].y, fglissInfos->points[1].y, fglissInfos->points[2].y, fglissInfos->points[3].y};
		if (sse->endflag == GRSystemStartEndStruct::OPENRIGHT)
		{
			NVRect r = getGRStaff()->getBoundingBox();
			r += getGRStaff()->getPosition();
			coorX[2] = coorX[3] = r.right - LSPACE;
		}

		if (sse->startflag == GRSystemStartEndStruct::OPENLEFT )
		{
			coorX[0] = coorX[1] = coorX[3] - LSPACE;
		}
		hdc.Polygon(coorX, coorY, 4);
	}
	//wavy
	else
	{
		//not implemented yet
		//we have to implement new function of the device, in order to be able to rotate a symbol
		/*
		float width = fglissInfos->points[3].x - fglissInfos->points[0].x;
		float height = fglissInfos->points[3].y - fglissInfos->points[0].y;
		float pasX = width/10;
		float pasY = height/10;

		float X = fglissInfos->points[0].x;
		float Y = fglissInfos->points[0].y;
		const int NSEGS = 25;

		for(int i=0; i<10; i++)
		{
			NVPoint thePoints[ 2*(NSEGS+3) ];
			int index = 0;
			float x1 = X+i*pasX;
			float y1 = Y+i*pasY;
			float y2a;
			float y2b;
			float x3 = X+pasX*(i+1);
			float y3 = Y+pasY*(i+1);
			if(i%2==0)
			{
				y2a = y1 - (pasY + pasX*pasX/pasY)/2;
				y2b = y3 - (pasY + pasX*pasX/pasY)/2;
			}
			else
			{
				y2a = y1 + (pasY + pasX*pasX/pasY)/2;
				y2b = y3 + (pasY + pasX*pasX/pasY)/2;
				
				x1-=1;
				x3+=1;
			}
			makeCurve(x1, y1, x1, y2a, x3, y2b, x3, y3, NSEGS, thePoints, &index);
			
			if(i%2==0)
			{
				y2a-=5;
				y2b-=5;
				x3-=1;
				x1+=1;
			}
			else
			{
				y2a+=5;
				y2b+=5;
				x1+=1;
				x3-=1;
			}
			
			makeCurve(x3, y3, x3, y2b, x1, y2a, x1, y1, NSEGS, thePoints, &index);
			float xPoints [ 2*(NSEGS+3) ];
			float yPoints [ 2*(NSEGS+3) ];
			for( int currPt = 0; currPt < index; ++ currPt )
			{
				xPoints [ currPt ] = thePoints[ currPt ].x;
				yPoints [ currPt ] = thePoints[ currPt ].y;
			}

			hdc.Polygon( xPoints, yPoints, index );
			
		}*/
	}
	if (mColRef) hdc.PopFillColor();
}


void GRGlissando::tellPosition(GObject * caller, const NVPoint & newPosition)
{
	GRNotationElement * grel = dynamic_cast<GRNotationElement *>(caller);
	if (grel == 0 ) return;

	GRStaff * staff = grel->getGRStaff();
	if (staff == 0 ) return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct( staff->getGRSystem());
	if (sse == 0)	return;

	const GRNotationElement * const endElement = sse->endElement;

	if( grel == endElement )
	{
		updateGlissando( staff );
	}
}


void GRGlissando::updateGlissando( GRStaff * inStaff )
{
	GRSystemStartEndStruct * sse = getSystemStartEndStruct( inStaff->getGRSystem());
	if ( sse == 0 ) return;

	// --- Collects informations about the context ---

	fglissContext.staff = inStaff;
	getGlissandoBeginingContext( &fglissContext, sse );
	getGlissandoEndingContext( &fglissContext, sse );

	ARGlissando * arGliss = static_cast<ARGlissando *>(getAbstractRepresentation());
	const float staffLSpace = inStaff->getStaffLSPACE();
	assert(arGliss);
	

	//we gather the informations of parameters
	float dx1 = arGliss->getDx1()->getValue( staffLSpace );
	float dy1 = arGliss->getDy1()->getValue( staffLSpace );
	float dx2 = arGliss->getDx2()->getValue( staffLSpace );
	float dy2 = arGliss->getDy2()->getValue( staffLSpace );

	float XLeft = 0;
	float YLeft = 0;
	float YLeft2 = 0;
	float XRight = 0;
	float YRight = 0;
	float YRight2 = 0;

	float dFirstLeftx = 0;
	float dFirstLefty = 0;
	float dFirstRightx = 0;
	float dFirstRighty = 0;

	float acc = 0;
	
	
	if(fglissContext.firstRightHead)
	{
		XRight = fglissContext.firstRightHead->getPosition().x + fglissContext.rightNoteDX;
		dFirstRightx = fglissContext.firstRightHead->getBoundingBox().Width()*3/4*fglissContext.sizeRight;
		YRight = fglissContext.firstRightHead->getPosition().y + fglissContext.rightNoteDY;
		if(fglissContext.accidentalRight)
		{
			acc = fglissContext.accidentalRight->getBoundingBox().Width()*getSize();
		}
	}


	if(fglissContext.firstLeftHead)
	{
		XLeft = fglissContext.firstLeftHead->getPosition().x + fglissContext.leftNoteDX; 
		dFirstLeftx = fglissContext.firstLeftHead->getBoundingBox().Width()*3/4*fglissContext.sizeLeft;
		YLeft = fglissContext.firstLeftHead->getPosition().y + fglissContext.leftNoteDY;
	}


	if(fill && (fglissContext.secondLeftHead || fglissContext.secondRightHead))
	{
		if(fglissContext.secondLeftHead)
		{
			YLeft2 = fglissContext.secondLeftHead->getPosition().y + fglissContext.leftNoteDY;
		}
		else
			YLeft2 = YLeft;
		if(fglissContext.secondRightHead)
		{
			YRight2 = fglissContext.secondRightHead->getPosition().y + fglissContext.rightNoteDY;
		}
		else
			YRight2 = YRight;
		fglissInfos->points[0].y = YLeft - dy1;
		fglissInfos->points[1].y = YLeft2 - dy1;
		fglissInfos->points[3].y = YRight - dy2;
		fglissInfos->points[2].y = YRight2 - dy2;
		fglissInfos->points[0].x = fglissInfos->points[1].x = XLeft + dx1;
		fglissInfos->points[3].x = fglissInfos->points[2].x = XRight + dx2 - acc;
		fglissInfos->position = fglissInfos->points[0];
	}
	else
		{
		//now we manage the case of same Y but different pitches...
		if(YRight == YLeft)
		{
			bool forceUp = false;
			bool forceDown = false;
			compareAccidentals( sse, &forceUp, &forceDown);
			if(forceUp)
			{
				YLeft += LSPACE/4;
				YRight -= LSPACE/4;
			}
			else if (forceDown)
			{
				YLeft -= LSPACE/4;
				YRight += LSPACE/4;
			}
		}

		float deltaX = XRight - XLeft;
		float deltaY = YRight - YLeft;

		dFirstLefty = dFirstLeftx*deltaY/deltaX;
		dFirstRighty = dFirstRightx*deltaY/deltaX;

		float thickness = arGliss->getThickness()->getValue( staffLSpace )*sqrt(deltaX*deltaX + deltaY*deltaY)/deltaX;

		fglissInfos->points[0].y = YLeft - dy1 + dFirstLefty + thickness/2;
		fglissInfos->points[1].y = YLeft - dy1 + dFirstLefty - thickness/2;
		fglissInfos->points[3].y = YRight - dy2 - dFirstRighty + thickness/2;
		fglissInfos->points[2].y = YRight - dy2 - dFirstRighty - thickness/2;
		fglissInfos->points[0].x = fglissInfos->points[1].x = XLeft + dx1 + dFirstLeftx;
		fglissInfos->points[3].x = fglissInfos->points[2].x = XRight + dx2 - dFirstRightx - acc;
		fglissInfos->position = fglissInfos->points[0];
	}
}


void GRGlissando::getGlissandoBeginingContext( GRGlissandoContext * ioContext, GRSystemStartEndStruct * sse )
{
	GRNotationElement * startElement = sse->startElement;
	if(sse->startflag == GRSystemStartEndStruct::OPENLEFT)
		startElement = flaststartElement;
		
	GRSingleNote * note = dynamic_cast<GRSingleNote *>(startElement);
	if( note )
	{
		ioContext->firstLeftHead = note->getNoteHead();
		if(nextGRGlissando && fill)
		{
			GRSingleNote * nextnote = dynamic_cast<GRSingleNote *>(nextGRGlissando->getStartElement(ioContext->staff));
			if(nextnote)
				ioContext->secondLeftHead = nextnote->getNoteHead();
		}
		else
			ioContext->secondLeftHead = NULL;
		ioContext->sizeLeft = note->getSize();
		ioContext->leftNoteDX = note->getOffset().x;
		ioContext->leftNoteDY = note->getOffset().y;
	}
}

void GRGlissando::getGlissandoEndingContext( GRGlissandoContext * ioContext, GRSystemStartEndStruct * sse )
{	
	GRNotationElement * endElement = sse->endElement;
	if(sse->endflag == GRSystemStartEndStruct::OPENRIGHT)
		endElement = lastendElement;
	GRSingleNote * note = dynamic_cast<GRSingleNote *>(endElement);
	if( note )
	{
		//noteHead
		ioContext->firstRightHead = note->getNoteHead();
		//accidental
		GRAccidentalList noteacclist;
		note->extractAccidentals( &noteacclist );
		if(!noteacclist.empty())
			ioContext->accidentalRight = noteacclist.GetHead();

		//nextglissando?
		if(nextGRGlissando && fill)
		{
			GRSingleNote * nextnote = dynamic_cast<GRSingleNote *>(nextGRGlissando->getEndElement(ioContext->staff));
			if(nextnote)
			{
				//noteHead
				ioContext->secondRightHead = nextnote->getNoteHead();
				//accidental
				if(!ioContext->accidentalRight)
				{
					GRAccidentalList noteacclist2;
					nextnote->extractAccidentals( &noteacclist2 );
					if(!noteacclist2.empty())
						ioContext->accidentalRight = noteacclist2.GetHead();
				}
			}

		}
		else
			ioContext->secondRightHead = NULL;
		
		ioContext->sizeRight = note->getSize();
		ioContext->rightNoteDX = note->getOffset().x;
		ioContext->rightNoteDY = note->getOffset().y;
	}
}

void GRGlissando::compareAccidentals(GRSystemStartEndStruct * sse, bool * isUp, bool * isDown)
{
	GRNotationElement * startElement = sse->startElement;
	if(sse->startflag == GRSystemStartEndStruct::OPENLEFT)
		startElement = flaststartElement;
	GRSingleNote * startnote = dynamic_cast<GRSingleNote *>(startElement);

	GRNotationElement * endElement = sse->endElement;
	if(sse->endflag == GRSystemStartEndStruct::OPENRIGHT)
		endElement = lastendElement;
	GRSingleNote * endnote = dynamic_cast<GRSingleNote *>(endElement);

	if(startnote && endnote )
	{
		ARNote * arstartnote = startnote->getARNote();
		ARNote * arendnote = endnote->getARNote();
		if(arstartnote && arendnote )
		{
			int startnoteAcc = arstartnote->getAccidentals() + arstartnote->getDetune();
			int endnoteAcc = arendnote->getAccidentals() + arendnote->getDetune();
			if(startnoteAcc<endnoteAcc)
				*isUp = true;
			else if(endnoteAcc<startnoteAcc)
				*isDown = true;
		}
	}
}

void GRGlissando::removeAssociation(GRNotationElement * el )
{
	GRPositionTag::removeAssociation(el);
	GRARNotationElement::removeAssociation(el);
}
void GRGlissando::addAssociation(GRNotationElement * grnot)
{

	if (error) return;

		if ( GREvent::cast( grnot )  && 	// stop immediately if it's not an event.
		(dynamic_cast<GRNote *>(grnot) ||
		 dynamic_cast<GRRest *>(grnot) ||
		 dynamic_cast<GREmpty *>(grnot)))
	{
	  	GRARNotationElement::addAssociation(grnot);
	}
	else
	{
		setError(1);
	}

	if (!error)
	{
		GRPositionTag::addAssociation(grnot);
	}
}


// -----------------------------------------------------------------------------
GRNotationElement * GRGlissando::getStartElement(GRStaff * grstaff) const
{
	GuidoPos pos = mStartEndList.GetHeadPosition();
	while (pos)
	{
		GRSystemStartEndStruct * sse = mStartEndList.GetNext(pos);
		if (sse->grsystem == grstaff->getGRSystem())
			return sse->startElement;
	}
	return 0;
}

// -----------------------------------------------------------------------------
GRNotationElement * GRGlissando::getEndElement(GRStaff * grstaff) const
{
	GuidoPos pos = mStartEndList.GetHeadPosition();
	while (pos)
	{
		GRSystemStartEndStruct * sse = mStartEndList.GetNext(pos);
		if (sse->grsystem == grstaff->getGRSystem())
			return sse->endElement;
	}
	return 0;
}

//reimplemented to keep the last startelement in memory..
void GRGlissando::BreakTag(GRStaff * grstaff, GuidoPos & assocpos)
{
	if (grstaff == 0) return;
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(grstaff->getGRSystem());
	sse->grsystem = grstaff->getGRSystem();

	assert(sse);
	
	NEPointerList * associated = 0;
	GRNotationElement * el = dynamic_cast<GRNotationElement *>(this);
	if (el)
		associated = el->getAssociations();

	if (!associated) return;
	

	lastendElement = sse->endElement;
	lastendflag = sse->endflag;
	lastendpos = sse->endpos;
	
	sse->endflag = GRSystemStartEndStruct::OPENRIGHT;

	flaststartElement = sse->startElement;
	sse->endElement = NULL;

	setEndElement(grstaff,grstaff->getEndGlue());
	sse->endpos = assocpos;

	if (associated && assocpos)
		associated->GetNext(assocpos);
}
