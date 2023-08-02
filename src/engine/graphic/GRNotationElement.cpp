/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>
#include <typeinfo>

// - Guido Misc
#include "VGDevice.h"
#include "FontManager.h"

// - Guido GR
#include "GRNotationElement.h"
#include "GRAccidental.h"
#include "GRStaff.h"
#include "GRGlue.h"
#include "GRSystemSlice.h"
#include "GRBar.h"
#include "GRClef.h"
#include "GRKey.h"
#include "GRMeter.h"
#include "GRGrace.h"

// - Guido AR
#include "ARMusicalObject.h"
#include "ARMusicalTag.h"
#include "ARNote.h"

#include "secureio.h"
#include "FontManager.h"
#include "VGFont.h"

using namespace std;

GRNotationElement::GRNotationElement()
{
	mAssociated = 0;
	mSpringID = -1;
	mNeedsSpring = 0;
	mGrStaff = 0;
	mLeftSpace = 0;
	mRightSpace = 0;
	mSymbol =  kNoneSymbol;
	mDraw = true;
	mShow = true;
    mIsInHeader = false;
}

GRNotationElement::~GRNotationElement()
{
	// here, we need to tell "a" spring, that the element no longer belongs ...
	// this is important for newlines that happen after the graphic-elements have been created ...
	if (mAssociated)
	{
		GuidoPos pos = mAssociated->GetHeadPosition();
		GRNotationElement * el;
		while (pos)
		{
			el = mAssociated->GetNext(pos);
			if (el)
				el->removeAssociation(this);
		}
		delete mAssociated;
		mAssociated = 0;
  } 
  // previously: no delete only set to NULL ... associated = NULL;
}

// --------------------------------------------------------------------------
void GRNotationElement::accept (GRVisitor& visitor)
{
	visitor.visitStart (this);
	visitor.visitEnd (this);
}

// --------------------------------------------------------------------------
void GRNotationElement::SendMap (const NVRect& map, MapCollector& f, TYPE_TIMEPOSITION date, TYPE_DURATION dur, GuidoElementType type, MapInfos& infos) const
{
	FloatRect r (map.left, map.top, map.right, map.bottom);
	r.Shift( infos.fPos.x, infos.fPos.y);
	r.Scale( infos.fScale.x, infos.fScale.y);

	GuidoDate from	= { date.getNumerator(), date.getDenominator() };
	TYPE_TIMEPOSITION end = date + dur;
	GuidoDate to	= { end.getNumerator(), end.getDenominator() };
	TimeSegment dates (from, to);			// current rolled segment
	GuidoElementInfos inf;
	inf.type = type;
	inf.staffNum = getStaffNumber();
	if (inf.staffNum < 0) inf.staffNum = 0;

	const ARMusicalObject * ar = getAbstractRepresentation();
	inf.voiceNum = ar ? ar->getVoiceNum() : 0;

    const ARNote *arNote = dynamic_cast<const ARNote *>(ar);
    inf.midiPitch = (arNote ? arNote->getMidiPitch() : -1);

	f.Graph2TimeMap (r, dates, inf);
}

// --------------------------------------------------------------------------
void GRNotationElement::SendMap (MapCollector& f, TYPE_TIMEPOSITION date, TYPE_DURATION dur, GuidoElementType type, MapInfos& infos) const
{
	SendMap (mMapping, f, date, dur, type, infos);
}

// -------------------------------------------------------------------------
void GRNotationElement::print(ostream& os) const
{
	const ARMusicalObject * ar = getAbstractRepresentation();
	os << "GRNotationElement at " << getRelativeTimePosition() << " - ";
	if (ar) {
        const ARMusicalTag * tag = static_cast<const ARMusicalTag *>(ar->isARMusicalTag());
        if (tag) {
			os << tag->getGMNName() << tag->getTagParameters();
        }
		else os << ar->getGMNName();
	}
	else
        os << "=> no ARMusicalObject";
}
std::ostream& operator<< (std::ostream& os, const GRNotationElement& e)		{ e.print(os); return os; }
std::ostream& operator<< (std::ostream& os, const GRNotationElement* e)		{ e->print(os); return os; }

// -------------------------------------------------------------------------
void 
GRNotationElement::addToBoundingBox( GRNotationElement * in )
{
	NVRect box (in->getBoundingBox());
	box += in->getPosition();
	box += in->getOffset();
	box -= getPosition();

	mBoundingBox.Merge( box );
}

// -------------------------------------------------------------------------
void GRNotationElement::OnDrawText( VGDevice & hdc, NVPoint pos, const char * text, int inCharCount ) const
{
	if(!mDraw || !mShow) return;
	
	const VGFont* hmyfont = FontManager::gFontText;
	const int size = getFontSize();
	const unsigned char * colref = getColRef();
	
	if (getFont())
		hmyfont = FontManager::FindOrCreateFont( size, getFont(), getFontAttrib());
	
	hdc.SetTextFont( hmyfont );
	const VGColor prevTextColor = hdc.GetFontColor();
	if (colref) hdc.SetFontColor( VGColor( colref ));

	const NVPoint & refpos = getReferencePosition();
	const NVPoint & offset = getOffset();

	hdc.SetFontAlign(getTextAlign());
	hdc.DrawString(	(float)(pos.x + offset.x + (refpos.x * size)),
					(float)(pos.y + offset.y + (refpos.y * size)),
					text, inCharCount );
	if (colref) hdc.SetFontColor( prevTextColor );
}

// -------------------------------------------------------------------------
void GRNotationElement::OnDrawText( VGDevice & hdc,  const char * text, int inCharCount ) const
{
	OnDrawText (hdc, mPosition, text, inCharCount);
}

// -------------------------------------------------------------------------
/* (JB) Choosing font width was both inaccurate and non-portable. Non-proportionnal
	character drawing can be done using the inScaleX parameter.

	Also, using different fonts heights may offers the best rendering quality, 
	but it would probably be acceptable (and faster) to just scale the graphic device 
	context to draw characters with different heights.
	
	Todo: replace inFontSize by an y-scale parameter.
*/
/** \brief Draws a musical symbol glyph at given position into input graphic device context.
	It possible to scale the symbol, by specifying a font height and a x-scale.
*/
void GRNotationElement::OnDrawSymbol( VGDevice & hdc, unsigned int inSymbol, float x, float y, float fSize ) const
{
	// - Setup colors
	if(!mDraw || !mShow)
		return;

	const unsigned char * colref = getColRef();
	const VGColor prevFontColor = hdc.GetFontColor();
  	if (colref)
		hdc.SetFontColor( VGColor( colref ));

	// - Setup text align 
	hdc.SetFontAlign(getTextAlign());
	// - Draw
	DrawSymbol( hdc, inSymbol, x, y, fSize );
//	DrawBoundingBox( hdc, VGColor(0,0,200)); // debug 

	// - Restore context
	if (colref)
		hdc.SetFontColor( prevFontColor );  //(TODO: in a parent method)
}

// -------------------------------------------------------------------------
void GRNotationElement::DrawNumericSymbols( VGDevice & hdc, const char* str, float x, float y, float size, float spacing) const
{
	while (*str) {
#ifdef SMUFL
		int symbol = (*str == '+') ? kMeterPlusSymbol : *str + kMeter0Symbol - '0';
#else
		int symbol = *str;
#endif
		DrawSymbol(hdc, symbol, x, y, size);
		float w, h;
		FontManager::gFontScriab->GetExtent(symbol, &w, &h, &hdc);
		x += w * size + spacing;
		str++;
	}
}

// -------------------------------------------------------------------------
/** \brief Low-level symbol drawing.
*/
void GRNotationElement::DrawSymbol( VGDevice & hdc, unsigned int inSymbol, float inOffsetX, float inOffsetY, float inFontSize ) const
{
	if(!mDraw || !mShow) return;

	// - Setup font
	const VGFont* myfont = FontManager::gFontScriab;
	const float theSize = (inFontSize != 0) ? inFontSize : getSize();
	if (theSize < kMinNoteSize) return;		// element is too small, don't draw it

	if (theSize != float(1.0)) {
		const int newFontSize = (int)(theSize * 4 * LSPACE + 0.5f ); // +0.5 to round from float to int.
		myfont = FontManager::FindOrCreateFont( newFontSize );
	}
//	hdc.SelectFont( myfont );
	hdc.SetMusicFont( myfont );

	// - Setup position
	const NVPoint & offset = getOffset();
  	const NVPoint & refpos = getReferencePosition();
	const float xPos = mPosition.x + offset.x + (refpos.x * theSize) + inOffsetX;
	const float yPos = mPosition.y + offset.y + (refpos.y * theSize) + inOffsetY;

	// - Draw
	hdc.DrawMusicSymbol( xPos, yPos, inSymbol );

#ifdef SHOWRODS
	// draw the rods ....
	const float leftspc = getLeftSpace();
	const float rightspc = getRightSpace();

   	if (leftspc)
	{
		const float myposy = 350;		
		hdc.PushPen(VGColor( 0, 0, 200 ), 5 );	// Always blue		
		hdc.Line( mPosition.x-leftspc,myposy-30, mPosition.x-leftspc,myposy+30 );
		hdc.Line( mPosition.x-leftspc, myposy, mPosition.x, myposy );
		hdc.PopPen();
	}
  	if (rightspc)
	{
		const float myposy = 350;
		hdc.PushPen(VGColor( 0, 0, 200 ), 5 );	// Always blue		
		hdc.Line( mPosition.x+rightspc, myposy-30, mPosition.x+rightspc, myposy+30 );
		hdc.LineTo( mPosition.x+rightspc, myposy, mPosition.x, myposy );
		hdc.PopPen();
	}
#endif // #ifdef SHOWRODS
}

// -------------------------------------------------------------------------
/** \brief For debugging purpose only
*/
void		
GRNotationElement::DrawExtents( VGDevice & hdc, const VGColor & inColor ) const
{
	if(!mDraw)
		return;
	
#if (0)
	hdc.PushPen( inColor, LSPACE * 0.15f );
	const float x1 = mPosition.x - getLeftSpace();
	const float x2 = mPosition.x + getRightSpace();
	const float y = mPosition.y;
	hdc.Line( x1, y, x2, y );
	hdc.Line( x1, y - LSPACE, x1, y + LSPACE );
	hdc.Line( x2, y - LSPACE, x2, y + LSPACE );
	hdc.PopPen();
#endif
}

// -------------------------------------------------------------------------
void GRNotationElement::OnDraw(VGDevice & hdc) const
{
	if(!mDraw || !mShow)
		return;
	
	if (mSymbol != 0)
		OnDrawSymbol( hdc, mSymbol );
//	NVRect r = mBoundingBox + mPosition;
//	hdc.Frame(r.left, r.top, r.right, r.bottom);
}

// -------------------------------------------------------------------------
unsigned int GRNotationElement::getTextAlign() const
{
	return (VGDevice::kAlignLeft | VGDevice::kAlignBase);
}

// -------------------------------------------------------------------------
TYPE_TIMEPOSITION  GRNotationElement::getRelativeEndTimePosition() const
{
	return (mRelativeTimePositionOfGR + mDurationOfGR);
}

// -------------------------------------------------------------------------
void GRNotationElement::recalcVerticalPosition()
{
}

// -------------------------------------------------------------------------
void GRNotationElement::setHPosition( float inX )
{
	// - Call inherited
	GObject::setHPosition( inX );
	
	// - Notify associations using 'tell'
	if (mAssociated)
		mAssociated->tellPosition( this, getPosition());
}

// -------------------------------------------------------------------------
void GRNotationElement::setPosition( const NVPoint & inPos )
{
	// - Call inherited
	GObject::setPosition(inPos );

	// - Notify associations using 'tell' (ties, slurs, tuplets...)
	if (mAssociated) 
		mAssociated->tellPosition( this, getPosition());
}

// -------------------------------------------------------------------------
NVRect GRNotationElement::getAssociatedBoundingBox() const
{
	NVRect rect;
	GuidoPos pos = mAssociated->GetHeadPosition();
	bool first = true;
	while( pos ) {
		GRNotationElement * el = mAssociated->GetNext(pos);
		NVRect bb = el->getBoundingBox() + el->getPosition();
		if (first) {
			rect = bb;
			first = false;
		}
		else rect.Merge(bb);
	}
	return rect;
}

// -------------------------------------------------------------------------
void GRNotationElement::addAssociation( GRNotationElement * p )
{
	// DF 02-08-2023 don't associate a GRGrace to any element
	// it's useless since notes are already notified of their 'grace' status
	// solves the crash of issue #173
	if (dynamic_cast<const GRGrace*>(p)) return;

	if (mAssociated == 0)
		mAssociated = new NEPointerList; // these elements don't belong to the list

	mAssociated->AddTail(p);
}

// -------------------------------------------------------------------------
void GRNotationElement::removeAssociation( GRNotationElement * el )
{
	if (mAssociated)
		mAssociated->RemoveElement(el);
}

// -------------------------------------------------------------------------
void GRNotationElement::FreeAssociatedList()
{
	delete mAssociated;
	mAssociated = 0;
}

// -------------------------------------------------------------------------
/** \brief This removes a whole list of associations */
void GRNotationElement::removeAssociation( const NEPointerList & nl )
{
	GuidoPos pos = nl.GetHeadPosition();
	while (pos)
		removeAssociation( nl.GetNext(pos));
}


// Static method.
// -1: g1 < g2
//  0 : g1 == g2
//  1 : g1 > g2
int GRNotationElement::ComparePos( const GRNotationElement * g1, const GRNotationElement * g2 )
{
	if (g2 == 0)		return -1;
	if (g1 == 0)		return 1;

	if ( g1->mRelativeTimePositionOfGR < g2->mRelativeTimePositionOfGR )
		return -1;

	if ( g1->mRelativeTimePositionOfGR > g2->mRelativeTimePositionOfGR )
		return 1;

	// now, the timepositions are equal.
	// We return the Graphicalelements in there correct order ...
	
	if (typeid(*g1) == typeid(GRGlue))	// we should consider using a dynamic cast only
	{
		// was const GRGlue * myglue = dynamic cast<const GRGlue *>(g1);
		const GRGlue * myglue = static_cast<const GRGlue *>(g1);
		
		if (myglue->mIsStart)	return -1;
		else return 1;
	}
	else if (typeid(*g2) == typeid(GRGlue))
	{
		// was	const GRGlue * myglue = dynamic cast<const GRGlue *>(g2);
		const GRGlue * myglue = static_cast<const GRGlue *>(g2);

		if (myglue->mIsStart) return 1;
		else return -1;
	}
	else if (typeid(*g1) == typeid(GRBar))		return -1;
	else if (typeid(*g2) == typeid(GRBar))		return 1;
	else if (typeid(*g1) == typeid(GRClef))		return -1;
	else if (typeid(*g2) == typeid(GRClef))		return 1;
	else if (typeid(*g1) == typeid(GRKey))		return -1;
	else if (typeid(*g2) == typeid(GRKey))		return 1;
	else if (typeid(*g1) == typeid(GRMeter))		return -1;
	else if (typeid(*g2) == typeid(GRMeter))		return 1;


	// just don't know ...
	return 0;
}

GRSystemSlice * GRNotationElement::getGRSystemSlice() const
{
	return mGrStaff ? mGrStaff->getGRSystemSlice() : 0;
}

/** \brief Called by elements that are associated with this object
*/
void GRNotationElement::tellPosition( GObject * caller, const NVPoint & point )
{
	if (mNeedsSpring == 0)
	{
		// this ensures, that setHPosition is never called
		// by a spring, otherwise we would have
		// recursive loops ...
		setPosition(point);
	}
}

/** \brief Use the system slices to find the number of the staff.
		returns -1 on failure.
*/
int GRNotationElement::getStaffNumber() const
{
	if ( mGrStaff )
	{
		GRSystemSlice * slc = mGrStaff->getGRSystemSlice();
		if (slc)
			return slc->getStaffNumber( mGrStaff );
	}
	return -1;
}

// -------------------------------------------------------------------------
//template <class T> int compnotposy( const T * el1, const T * el2 )
int compnotposy( const GRNotationElement * el1, const GRNotationElement * el2 )
{
	assert( el1 );
	assert( el2 );

	int staffnum1 = -1;
	int staffnum2 = -1;
	
	NVPoint pt1 (el1->getPosition());
	NVPoint pt2 (el2->getPosition());

	GRStaff * staff = el1->getGRStaff();	
	if( staff ) staffnum1 = staff->getStaffNumber();
	staff = el2->getGRStaff();
	if( staff ) staffnum2 = staff->getStaffNumber();

	pt1.y += (float)(staffnum1 * 20 * LSPACE);
	pt2.y += (float)(staffnum2 * 20 * LSPACE);

	if (pt1.y > pt2.y ) return 1;
	if (pt1.y < pt2.y ) return -1;
    return 0;
}
int compaccposy( const GRAccidental * el1, const GRAccidental * el2 )
{
	assert( el1 );
	assert( el2 );

	int staffnum1 = -1;
	int staffnum2 = -1;
	
	NVPoint pt1 (el1->getPosition());
	NVPoint pt2 (el2->getPosition());

	GRStaff * staff = el1->getGRStaff();	
	if( staff ) staffnum1 = staff->getStaffNumber();
	staff = el2->getGRStaff();
	if( staff ) staffnum2 = staff->getStaffNumber();

	pt1.y += (float)(staffnum1 * 20 * LSPACE);
	pt2.y += (float)(staffnum2 * 20 * LSPACE);

	if (pt1.y > pt2.y ) return 1;
	if (pt1.y < pt2.y ) return -1;
    return 0;
}
