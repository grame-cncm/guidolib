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
#include "ARMMRest.h"
#include "TagParameterFloat.h"
#include "TagParameterString.h"

#include "FontManager.h"
#include "GRSingleRest.h"
#include "GRStaff.h"
#include "GRSystem.h"
#include "GRSystemSlice.h"
#include "GRGlue.h"
#include "GRNoteDot.h"
#include "GRArticulation.h"
#include "VGDevice.h"
#include "VGFont.h"

//#define TRACE
#ifdef TRACE
#define traceMethod(method)		std::cout << (void*)this << " GRSingleRest::" << method << std::endl
#else
#define traceMethod(method)	
#endif

using namespace std;

NVPoint GRSingleRest::sRefpos;

GRSingleRest::GRSingleRest(GRStaff * grstf,const TYPE_DURATION & theDuration )
							: GRRest(grstf,new ARRest(theDuration), true) // ownsAR
{
	createRest(theDuration);
	mCurLSPACE = grstf->getStaffLSPACE();
}

GRSingleRest::GRSingleRest(GRStaff *grstf, const ARRest* arrest) : GRRest(grstf,arrest)
{
	assert(arrest);
	createRest(DURATION_1);
}

GRSingleRest::GRSingleRest(GRStaff * grstf, const ARRest * arrest, const TYPE_TIMEPOSITION & date,
			const TYPE_DURATION & duration, const TYPE_DURATION p_durtemplate)
			: GRRest(grstf, arrest, date, duration), mRestAppearance(arrest->getAppearance())
{
	assert(arrest);

	TYPE_DURATION d = p_durtemplate ? p_durtemplate : duration;
	if (mRestAppearance.size()) {
		TYPE_DURATION tmp = appearance2duration(mRestAppearance);
		if (tmp) d = tmp;
	}
	createRest(d);
	
	if (mDurationOfGR == DURATION_0)
		mNeedsSpring = 0;
}


GRSingleRest::~GRSingleRest()
{
}

TYPE_DURATION GRSingleRest::appearance2duration (const NVstring& str) const
{
	if (str == "/4")	return TYPE_DURATION(1, 4);
	if (str == "/8")	return TYPE_DURATION(1, 8);
	if (str == "/2")	return TYPE_DURATION(1, 2);
	if (str == "/1")	return TYPE_DURATION(1, 1);
	if (str == "/16")	return TYPE_DURATION(1, 16);
//	if (str == "/32")	return TYPE_DURATION(1, 32);
//	if (str == "/64")	return TYPE_DURATION(1, 64);
	return TYPE_DURATION(0, 1);
}

// -----------------------------------------------------------------------------
void GRSingleRest::accept (GRVisitor& visitor)
{
	visitor.visitStart (this);
	visitor.visitEnd (this);
}

// -----------------------------------------------------------------------------
GRSingleRest::TYPES GRSingleRest::duration2Type (const TYPE_DURATION& duration) const
{
	if (duration >= DURATION_1) 	return P1;
	if (duration >= DURATION_2)		return P2;
	if (duration >= DURATION_4)		return P4;
	if (duration >= DURATION_8)		return P8;
	if (duration >= DURATION_16)	return P16;
	if (duration >= DURATION_32)	return P32;
	if (duration >= DURATION_64)	return P64;
	if (duration >= DURATION_128)	return P128;
	return P0;
}

// -----------------------------------------------------------------------------
float GRSingleRest::type2YPosition (TYPES type) const
{
	switch (type) {
#ifdef SMUFL
		case P1:	return mCurLSPACE;
		case P128:	return mCurLSPACE * 3;
		default:	return mCurLSPACE * 2;
#else
		case P1:	return mCurLSPACE;
		case P2:
		case P4:	return mCurLSPACE * 2;
		case P8:	return mCurLSPACE * 1.25f;
		case P16:
		case P32:	return mCurLSPACE * 2.25f;
		case P64:	return mCurLSPACE * 3.25f;
		case P128:	return mCurLSPACE * 4.25f;
		default:	return mCurLSPACE;
#endif
	}
}

// -----------------------------------------------------------------------------
void GRSingleRest::setTypeAndPos(TYPE_DURATION duration)
{
	mType = duration2Type (duration);
	mPosition.y = type2YPosition (mType);
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
	setTypeAndPos (duration);

	const float extent = GetSymbolExtent(mType);
	mLeftSpace = (float)(extent * 0.5f * mSize);
	mRightSpace = (float)(extent * 0.5f * mSize);
}

void GRSingleRest::GetMap( GuidoElementSelector sel, MapCollector& f, MapInfos& infos ) const
{
	if (sel == kGuidoEvent || sel == kGuidoBarAndEvent) {
		SendMap (f, getRelativeTimePosition(), getDuration(), kRest, infos);
	}
}

void GRSingleRest::DrawMultiMeasuresCount( VGDevice & hdc, int count, float x, float y, float ratio, const ARMMRest* mrest) const
{
	if (count < 0) return;

	float dx = mrest->getDX()->getValue();
	float dy = mrest->getDY()->getValue();
	const TagParameterFloat* psize = mrest->getSize();
	float size = psize ? psize->getValue() : 1.0;
	const TagParameterString* pcolor = mrest->getColor();
	VGColor savedColor = hdc.GetFontColor();
	if (pcolor) {
		unsigned char colorstr[4];
		pcolor->getRGB(colorstr);
		hdc.SetFontColor(VGColor(colorstr));
	}

	string str = std::to_string(count);
	float w = FontManager::ComputeSymbolsStrWidth(&hdc, str);
	DrawNumericSymbols (hdc, str.c_str(), x-w/4 + dx, y-2*LSPACE * ratio - dy, ratio * size, 5);
	if (pcolor)  hdc.SetFontColor(savedColor);
}

void GRSingleRest::DrawMultiMeasuresRest( VGDevice & hdc, const ARMMRest* mrest ) const
{
	GRStaff* staff = getGRStaff();
	float lspace = staff ? staff->getStaffLSPACE() : LSPACE;
	if (mType == P1) {
		if (fSecondbar) {

			const unsigned char * colref = getColRef();
			VGColor color = (colref) ? VGColor( colref ) : VGColor(0,0,0);
			if (colref)
				hdc.PushFillColor( color);

			float bx = fSecondbar->getPosition().x;
			float x = getPosition().x;
			float offset = bx - x - lspace*1.5;
			float x1 = x-offset + mOffset.x;
			float x2 = x + offset + mOffset.x;
			float h = mSize * LSPACE/2.3;
			float y = getPosition().y + lspace + mOffset.y;
			hdc.Rectangle(x1, y-h, x2, y+h);

			const float lineThickness = staff->currentLineThikness();
			hdc.PushPen(color, lineThickness);
			hdc.Line(x1, y - lspace, x1, y + lspace);
			hdc.Line(x2, y - lspace, x2, y + lspace);
			hdc.PopPen();
			if (colref)
				hdc.PopFillColor();
		}
		else cerr << "Warning: no bar for multi-measure rest" << endl;
	}
	else {
		GRRest::OnDrawSymbol( hdc, mType );
		DrawSubElements( hdc );		// - Draw elements (dots...)
	}
	DrawMultiMeasuresCount (hdc, mrest->getMeasuresCount(), 0, staff->getPosition().y - LSPACE/2, lspace/LSPACE, mrest);
}

void GRSingleRest::OnDraw( VGDevice & hdc ) const
{
	if(!mDraw || !mShow)
		return;
	traceMethod("OnDraw");
	if (mType == P0) return;		// don't know how to draw it !
	
	const ARMMRest* mrest = getARMMRest();
	if (mrest && getWholeMeasure()) {
		DrawMultiMeasuresRest (hdc, mrest);
		return;
	}

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

void GRSingleRest::setRestFormat( const ARRestFormat * restfrmt )
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
	if (mRestAppearance.empty())
		createDots( getDuration(), 0, dotPos );
}

void GRSingleRest::setFillsBar(bool value, GRNotationElement * bar1, GRNotationElement * bar2, bool filled)
{
	mFillsBar = value;
	if (mFillsBar)
	{
		fFirstbar = bar1;
		fSecondbar = bar2;
		if (filled) {
			setTypeAndPos(DURATION_1);
			RemoveAllSubElements();
		}
	}
}

void GRSingleRest::tellPosition( GObject * caller, const NVPoint & newPosition )
{
	const GRNotationElement* elt = dynamic_cast<const GRNotationElement*>(caller);
	if (mFillsBar && caller == fSecondbar)
	{
		// then we have a bar filler ....

		// now we need to check, wether the firstbar exists ...
		// if not, we need the horizontal position of the firstglue
		// of the first systemslice .... 
		float posx = -1;
		if (fFirstbar)
		{	
			// we need to check, if the systems of the firstbar
			// and secondbar are identical ....
			GRSystem * sys1 = fFirstbar->getGRStaff()->getGRSystem();
			GRSystem * sys2 = fSecondbar->getGRStaff()->getGRSystem();
			if (sys1 == sys2)
				posx = fFirstbar->getPosition().x;
		}
		if ((posx < 0 || !fFirstbar) && mGrStaff)
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
		setHPosition((float)((fSecondbar->getPosition().x - posx) * 0.5f + posx));
		setWholeMeasure(1);
		return;		
	}

	if ( mNeedsSpring == 0 && mSpringID == -1)
		setHPosition(newPosition.x);
}

unsigned int GRSingleRest::getTextAlign() const
{ 
	return (VGDevice::kAlignCenter | VGDevice::kAlignBase); 
}

