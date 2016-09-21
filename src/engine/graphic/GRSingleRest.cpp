/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003, 2004 	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>

#include "GUIDOEngine.h"
#include "GUIDOInternal.h"

#include "ARRest.h"
#include "ARRestFormat.h"
#include "TagParameterFloat.h"
#include "TagParameterString.h"

#include "GRSingleRest.h"
#include "GRStaff.h"
#include "GRSystem.h"
#include "GRSystemSlice.h"
#include "GRGlue.h"
#include "GRNoteDot.h"
#include "GRArticulation.h"
#include "VGDevice.h"

//#define TRACE
#ifdef TRACE
#define traceMethod(method)		std::cout << (void*)this << " GRSingleRest::" << method << std::endl
#else
#define traceMethod(method)	
#endif


NVPoint GRSingleRest::sRefpos;

GRSingleRest::GRSingleRest(GRStaff * grstf,const TYPE_DURATION & theDuration )
							: GRRest(grstf,new ARRest(theDuration), true) // ownsAR
{
	//mStemLen = 0;
	//mStemDir = dirUP;

	firstbar = secondbar = 0;
	createRest(theDuration);
	// spacing = getSpacing();
	
	mCurLSPACE = grstf->getStaffLSPACE();
}

GRSingleRest::GRSingleRest(GRStaff *grstf,
						   ARRest* abstractRepresentationOfRest)
						   : GRRest(grstf,abstractRepresentationOfRest)
{
	assert(abstractRepresentationOfRest);
	firstbar = secondbar = 0;
	createRest(DURATION_1);
}

GRSingleRest::GRSingleRest(GRStaff * grstf, ARRest * abstractRepresentationOfRest,
						   const TYPE_TIMEPOSITION & relativeTimePositionOfGRRest,
						   const TYPE_DURATION & durationOfGRRest,
						   const TYPE_DURATION p_durtemplate)
						   : GRRest(grstf,abstractRepresentationOfRest,relativeTimePositionOfGRRest,
						   durationOfGRRest)
{
	assert(abstractRepresentationOfRest);
	firstbar = secondbar = NULL;

	if (p_durtemplate>DURATION_0)
		createRest(p_durtemplate);
	else
		createRest(durationOfGRRest);

	if (mDurationOfGR == DURATION_0)
		mNeedsSpring = 0;
}


GRSingleRest::~GRSingleRest()
{
}

void GRSingleRest::createRest(const TYPE_DURATION & duration)
{
	traceMethod("createRest");

	durtemplate = duration;
	mType = P0;
	mBoundingBox.Set( 0, 0, 0, 0 );
	mLeftSpace = 0;
	mRightSpace = 0;
	sRefpos.x = 0;

	// (JB) changed equal comparaisons (duration == DURATION_xxx) to
	// greater or equal: (duration >= DURATION_xxx), to catch dotted durations.
	if (duration >= DURATION_1)
	{
		mType = P1;
		mPosition.y = mCurLSPACE;
	}
	else if (duration >= DURATION_2)
	{
		mType = P2;
		mPosition.y = mCurLSPACE * 2;
	}
	else if (duration >= DURATION_4)
	{
		mType = P4;
		mPosition.y = (float)(2 * mCurLSPACE);
	}
	else if (duration >= DURATION_8)
	{
		mType = P8;
		mPosition.y = (float)(1.25f * mCurLSPACE);

	}
	else if (duration >= DURATION_16)
	{
		mType = P16;
		mPosition.y = (float)(2.25f * mCurLSPACE);
	}
	else if (duration >= DURATION_32)
	{
		mType = P32;
		mPosition.y = (float)(2.25f * mCurLSPACE);
	}
	else if (duration >= DURATION_64)
	{
		mType = P64;
		mPosition.y = (float)(3.25f * mCurLSPACE);
	}
	else  if (duration >= DURATION_128)
	{
		mType = P128;
		mPosition.y = (float)(4.25f * mCurLSPACE);
	}
	else
	{ // Unknown Duration ... what do we do know
		mType = P0;
		return;
	}
	
	const float extent = GetSymbolExtent(mType);
	mLeftSpace = (float)(extent * 0.5f * mSize);
	mRightSpace = (float)(extent * 0.5f * mSize);
}

void GRSingleRest::GGSOutput() const
{
}

void GRSingleRest::GetMap( GuidoElementSelector sel, MapCollector& f, MapInfos& infos ) const
{
	if (sel == kGuidoEvent) {
		SendMap (f, getRelativeTimePosition(), getDuration(), kRest, infos);
	}
}

void GRSingleRest::OnDraw( VGDevice & hdc ) const
{
	if(!mDraw)
		return;
	traceMethod("OnDraw");
	if (mType == P0) return;		// don't know how to draw it !

	GRRest::OnDrawSymbol( hdc, mType );
	DrawSubElements( hdc );		// - Draw elements (dots...)

	// Draw articulations (fermata...)
	const GRNEList& articulations = getArticulations();
	for( GRNEList::const_iterator ptr = articulations.begin(); ptr != articulations.end(); ++ptr )
	{
		GRArticulation * el = *ptr;
		el->OnDraw(hdc);
	}

	if (gBoundingBoxesMap & kEventsBB)
		DrawBoundingBox( hdc, kEventBBColor);
}

void GRSingleRest::updateBoundingBox()
{
	traceMethod("updateBoundingBox");
	const float extent = GetSymbolExtent(mType);
	const int theSize = getFontSize();
	const NVPoint & offset = getOffset();
	float scale = getSize();

	mBoundingBox.left = -extent/2 * scale;
	mBoundingBox.right = mBoundingBox.left + (extent * scale);

	switch (mType) {
		case P1:
			mBoundingBox.top = (LSPACE - mPosition.y) * scale;
			mBoundingBox.bottom = (theSize - LSPACE) * scale;
			break;
		case P2:
			mBoundingBox.top = (LSPACE * 1.5f - mPosition.y) * scale;
			mBoundingBox.bottom = (theSize - LSPACE * 1.5f) * scale;
			break;
		case P8:
		case P16:
			mBoundingBox.top = (LSPACE - mPosition.y) * scale;
			mBoundingBox.bottom = theSize * scale;
			break;
		default:
			mBoundingBox.top = -mPosition.y * scale;
			mBoundingBox.bottom = theSize * scale;
			break;
	}
	mMapping = mBoundingBox;
	mMapping += mPosition + offset;
}

void GRSingleRest::setPosition( const NVPoint & inPos )
{
	GREvent::setPosition( inPos );
	
	if (mAssociated) {
		mAssociated->tellPosition( this, getPosition());
	}
}


/** \brief Called when a newline has been introduced, after the graphical 
	element was drawn
	
	returns:
	1, if all is ok
	-1, if not implemented
	 0, if only a part of ndur could be done
*/
int GRSingleRest::adjustLength( const TYPE_DURATION & ndur )
{
	// ATTENTION,what happens when
	// the note was within a tuplet!!!!
	// tuplet is not handled yet ....
	
//	GRTuplet * mytuplet = 0;
//	if (mAssociated)
//	{
	//	GuidoPos pos = mAssociated->GetHeadPosition();
	//	GRNotationElement * el;
	//	while (pos)
	//	{
	//		el = mAssociated->GetNext(pos);
	//		mytuplet = dynamic_cast<GRTuplet *>(el);
	//		if (mytuplet)
	//		{
	//			mytuplet->removeEvent(this); // use removeAssociation ?
	//			break;
	//		}		
	//	}
//	}
	
	mDurationOfGR = ndur;
		
	// does it own the stuff? YES
	// rest should not need this?
	RemoveAllSubElements();
	
	// create a new Rest (with all its elements)
	createRest( mDurationOfGR );
	
	// tell all the elements there positions.
	setPosition( mPosition );
	
	if (mType == P0) // could not handle the duration!
		return 0;	
	return 1;
}

int GRSingleRest::getNumFaehnchen() const
{
	switch (mType)
	{
		case P0: 	return 0;
		case P1: 	return 0;
		case P2:	return 0;
		case P4:	return 0;
		case P8:	return 1;
		case P16:	return 2;
		case P32:	return 3;
		case P64:	return 4;
		case P128:	return 5;		
		default:	return 0;
	}
}

const NVPoint & GRSingleRest::getReferencePosition() const
{
	return sRefpos;
}

void GRSingleRest::setRestFormat( ARRestFormat * restfrmt )
{
	traceMethod("setRestFormat");
	if (restfrmt != 0) {
		// - Get the color
		if (restfrmt->getColor())
		{
			if (mColRef == 0 )
				mColRef = new unsigned char [4];

			if (restfrmt->getColor()->getRGB( mColRef ) == false )
			{
				delete [] mColRef;
				mColRef = 0;
			}
		}

		// - Get the offsets and size
		const TagParameterFloat * tpf1 = restfrmt->getDX();
		if (tpf1)
			mOffset.x = tpf1->getValue();

		const TagParameterFloat * tpf2 = restfrmt->getDY();
		if (tpf2)
			mOffset.y = tpf2->getValue();

		const TagParameterFloat * tpf = restfrmt->getSize();
		mSize = tpf ? tpf->getValue() : float(1.0);
	}
		
	NVPoint dotPos;
	switch (mType) {
		case P1:
			dotPos.Set( 0, 1.8f * mCurLSPACE );
			break;
		case P32:
		case P64:
		case P128:
			dotPos.Set( 0, 1 * mCurLSPACE);
			break;
		default:
			dotPos.Set( 0, 2 * mCurLSPACE);
	}
	createDots( getDuration(), 0, dotPos );
}

void GRSingleRest::setFillsBar(bool value, GRNotationElement * bar1,
							   GRNotationElement * bar2)
{
	mFillsBar = value;
	if (mFillsBar)
	{
		firstbar = bar1;
		secondbar = bar2;
	}
}

void GRSingleRest::tellPosition( GObject * caller, const NVPoint & newPosition )
{
	if (mFillsBar && caller == secondbar)
	{
		// then we have a bar filler ....

		// now we need to check, wether the firstbar exists ...
		// if not, we need the horizontal position of the firstglue
		// of the first systemslice .... 
		float posx = -1;
		if (firstbar)
		{	
			// we need to check, if the systems of the firstbar
			// and secondbar are identical ....
			GRSystem * sys1 = firstbar->getGRStaff()->getGRSystem();
			GRSystem * sys2 = secondbar->getGRStaff()->getGRSystem();
			if (sys1 == sys2)
				posx = firstbar->getPosition().x;
		}
		if ((posx < 0 || !firstbar) && mGrStaff)
		{
			// this will only work, if we are at
			// the very beginning, and there was no
			// "artificial" begin-slice.
			GRGlue * myglue = mGrStaff->getSecondGlue();
			if (myglue)
				posx = myglue->getPosition().x;
			else
			{
				GRSystem * grsystem = mGrStaff->getGRSystem();
				// take the first systemslice ....
				if (grsystem)
				{
					GRSystemSlice * grsysslc = grsystem->getFirstGRSystemSlice();
					if (grsysslc)
					{
						myglue = grsysslc->getEndGlue();
						if (myglue)
							posx = myglue->getPosition().x;
					}
				}
			}
		}
		setHPosition((float)((secondbar->getPosition().x - posx) * 0.5f + posx));
		return;		
	}

	if ( mNeedsSpring == 0 && mSpringID == -1)
		setHPosition(newPosition.x);
}

unsigned int GRSingleRest::getTextAlign() const
{ 
	return (VGDevice::kAlignCenter | VGDevice::kAlignBase); 
}

