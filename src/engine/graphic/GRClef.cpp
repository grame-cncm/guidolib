/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>
#include <string.h>

#include "ARClef.h"
#include "GRClef.h"

#include "GRStaff.h"
#include "GRDefine.h"
#include "GRMusic.h"
#include "FontManager.h"
#include "TagParameterFloat.h"

#include "VGDevice.h"

#include "secureio.h"

using namespace std;

const char * const clef_cp8 = "8";
const char * const clef_cp15 = "15";

#ifdef SMUFL
NVPoint GRClef::fRefpos;
#else
NVPoint GRClef::refposPerc;
NVPoint GRClef::refposTreble;
NVPoint GRClef::refposBass;
NVPoint GRClef::refposAlto;
#endif

unsigned int GRClef::sClefTextAlign;

GRClef::GRClef(const ARClef * arClef, GRStaff *curstaff, bool ownsAR)
 				 : GRTagARNotationElement(arClef, curstaff->getStaffLSPACE(), ownsAR)
{
	assert(arClef);
	assert(curstaff);

	ARClef::cleftype clefType = arClef->getClefType();

	mClefStaffLine = curstaff->getNumlines() - arClef->getLine(); // lines are painted reverse
	if (mClefStaffLine < 0)
		mClefStaffLine = 0;

	mClefBaseOctave = arClef->getOctavaType();

	const float curLSPACE = curstaff->getStaffLSPACE();

	mNeedsSpring = 1;
	mDoubleTreble = false;
	sconst = SCONST_CLEF;	// for springs ?

	switch( clefType )
	{
		case ARClef::OFF:
		{
			mSymbol = 0;
			mClefBasePitch = NOTE_G;
			mClefBaseOctave += 1;
			mBoundingBox.top = 0;
			mBoundingBox.bottom = 0;
			break;
		}

		case ARClef::DOUBLEG:	// same as the octave treble clef (-8)
			mDoubleTreble = true;	// no break;		
		case ARClef::VIOLIN:
		{
			mSymbol = kClefViolin;
			mClefBasePitch = NOTE_G;
			mClefBaseOctave += 1;
			// default for mClefStaffLine is 3
			mPosition.y = mClefStaffLine * curLSPACE;
			// dependant on Font .. needed for octava-keys
#ifdef SMUFL
			mBoundingBox.top    = -(float(4.3) * curLSPACE);
#else
			mBoundingBox.top    = -(float(4.1) * curLSPACE);
#endif
			mBoundingBox.bottom =  (float(2.7) * curLSPACE);
			break;
		}

		case ARClef::BASS:
		{
			mSymbol = kClefBass;
			mClefBasePitch = NOTE_F;
			mPosition.y = (mClefStaffLine * curLSPACE);
			mBoundingBox.top = (-1 * curLSPACE);
#ifdef SMUFL
			mBoundingBox.bottom = (2.7 * curLSPACE);
#else
			mBoundingBox.bottom = (3 * curLSPACE);
#endif
			break;
		}
		
		case ARClef::BRATSCHE:
		{
			mSymbol = kClefBratsche;
			mClefBasePitch = NOTE_C;
			mClefBaseOctave += 1;
			mPosition.y = (mClefStaffLine * curLSPACE);
			mBoundingBox.top = (-2 * curLSPACE);
			mBoundingBox.bottom = (2 *curLSPACE);
			break;
		}

		case ARClef::PERC:
		{
			mSymbol = kClefPerc;
			mClefBasePitch = NOTE_H;
			mClefBaseOctave += 1;
			mPosition.y = (mClefStaffLine * curLSPACE);
			mBoundingBox.top = -curLSPACE;
			mBoundingBox.bottom = curLSPACE;
			break;
		}

		case ARClef::TAB:
		{
			mSymbol = 0;
			mClefBasePitch = NOTE_G;
			mClefBaseOctave = 0;
			mBoundingBox.top = (-2.5 * curLSPACE);
			mBoundingBox.bottom = (2.5 *curLSPACE);
		}
		
		default:
		{ // Here later gg and perc must be realised also
			mSymbol = kClefViolin;
			mClefBasePitch = NOTE_G;
			mClefBaseOctave += 1;
			// default for mClefStaffLine is 3
			mPosition.y = (mClefStaffLine * curLSPACE);
#ifdef SMUFL
			mBoundingBox.top    = -(float(4.3) * curLSPACE);
#else
			mBoundingBox.top    = -(float(4.1) * curLSPACE);
#endif
			mBoundingBox.bottom =  (float(2.7) * curLSPACE);
		}
	}

	ARClef::octavatype oct = arClef->getOctavaType();
	if (oct != ARClef::NONE) // d.h. -8,+8,-15,+15
	{
		mOctaveOffset.x  = 0;
		const float textHeight = float(1.5) * LSPACE;
		switch( oct ) {
			case ARClef::LOW8:
				mOctaveStr = clef_cp8;
				mBoundingBox.bottom += textHeight;
				mOctaveOffset.y = mBoundingBox.bottom;
				break;

			case ARClef::HIGH8:
				mOctaveStr = clef_cp8;
				mOctaveOffset.y = mBoundingBox.top;
				mBoundingBox.top -= textHeight;
				break;

			case ARClef::LOW15:
				mOctaveStr = clef_cp15;
				mBoundingBox.bottom += textHeight;
				mOctaveOffset.y = mBoundingBox.bottom;
				break;
			case ARClef::HIGH15:
				mOctaveStr = clef_cp15;
				mOctaveOffset.y = mBoundingBox.top;
				mBoundingBox.top -= textHeight;
				break;

			case ARClef::NONE:  // just to avoid a warning
				break;
		}
	}

	// calling the function from GObject ....	
	// this must be made platform-independantly ...
    // extent is always character extent with respect to STANDARD size (staffFormat 3pt per Halfspace)
    // all other extents must be calculated according to different sizes ....

	// this is OK, the size determines the extent of the rods ...
	float extent = GetSymbolExtent(mSymbol);
#ifdef SMUFL
	mLeftSpace = ((extent * 0.5f) * mTagSize);
	mRightSpace = ((extent * 0.5f) * mTagSize);
#else
	mLeftSpace  = ((extent * 0.5f + LSPACE / 5) * mTagSize);
	mRightSpace = ((extent * 0.5f + LSPACE / 5) * mTagSize);
#endif
	if( mDoubleTreble )
		mRightSpace += extent;

	mBoundingBox.left = -mLeftSpace;
	mBoundingBox.right = mRightSpace;

	// this can only be done ONCE (during construction)
	mTagSize *= curstaff->getSizeRatio();

	// this information must be read from the font (or a table/map) ....
	// this is static information!
	// reference-positions are also always according to STANDARD-size characters (staffFormat 3pt per Halfspace)
#ifdef SMUFL
	fRefpos.x  = - extent * 0.5f - LSPACE / 5.0f;
	fRefpos.y  = 0; //LSPACE;
#else
	switch (mSymbol)
	{
		case kClefViolin:
			refposTreble.x  = - extent * 0.5f - LSPACE / 5.0f;
			refposTreble.y  = LSPACE;
			break;

		case kClefBratsche:
			refposAlto.x  =  - extent * 0.5f - LSPACE / 5.0f;
			refposAlto.y  = 2 * LSPACE;
			break;

		case kClefBass:
			refposBass.x  = - extent * 0.5f - LSPACE / 5.0f;
			refposBass.y  = 3 * LSPACE;
			break;

		case kClefPerc:
			refposPerc.x  = - extent * 0.5f - LSPACE / 5.0f;
			refposPerc.y  = LSPACE;
			break;
	}
#endif

	sClefTextAlign = (VGDevice::kAlignLeft | VGDevice::kAlignBase);
	mGrStaff = curstaff;

    mIsInHeader = arClef->isInHeader();
}

// -----------------------------------------------------------------------------
void GRClef::accept (GRVisitor& visitor)
{
	visitor.visitStart (this);
	visitor.visitEnd (this);
}

// -----------------------------------------------------------------------------
void GRClef::setHPosition( float inX )
{
//cerr << (void*)this << " GRClef::setHPosition " << inX << " spring id: " << getSpringID() << endl;
	GRTagARNotationElement::setHPosition(inX);
	mMapping = mBoundingBox;
	float h = mBoundingBox.Height();
	float hr = h * mTagSize;
	float diff = (hr - h)/2;
	mMapping.top -= diff;
	mMapping.bottom += diff;
	mMapping += mPosition + getOffset();
}

// ----------------------------------------------------------------------------
/** \brief Retrieves the mapping
*/
void GRClef::GetMap( GuidoElementSelector sel, MapCollector& f, MapInfos& infos ) const
{
	if (sel == kClefSel)
		SendMap (f, getRelativeTimePosition(), getDuration(), kClef, infos);
}

// -----------------------------------------------------------------------------
void GRClef::DrawTAB(VGDevice & hdc, float size, float lspace) const
{
	const int fontSize = (int)(1.5 * lspace * size);
	const VGFont* font = FontManager::FindOrCreateFont( fontSize, "Helvetica");
	hdc.SetTextFont( font );
	unsigned int savedalign = hdc.GetFontAlign();
	hdc.SetFontAlign( VGDevice::kAlignBase | VGDevice::kAlignCenter );
	float y = mPosition.y - (2.5 * lspace);
	lspace *= 1.5 * size;
	hdc.DrawString( mPosition.x, y, "T", 1 );
	hdc.DrawString( mPosition.x, y + lspace, "A", 1 );
	hdc.DrawString( mPosition.x, y + lspace * 2, "B", 1 );
	hdc.SetFontAlign( savedalign );
}

// -----------------------------------------------------------------------------
void GRClef::OnDraw(VGDevice & hdc) const
{
	if (getError() || !mDraw || !mShow) return;

	const unsigned char * colref = getColRef();
	const VGColor prevFontColor = hdc.GetFontColor();
  	if (colref)
		hdc.SetFontColor( VGColor( colref ));

	const ARClef* arclef = getARClef();
	const TagParameterFloat * p = arclef->getSize();
	float size = p ? p->getValue() : 1;
	const GRStaff* staff = getGRStaff();
	float lspace = staff ? staff->getStaffLSPACE() : LSPACE;
	
	if (arclef->getClefType() == ARClef::TAB)
		DrawTAB (hdc, size, lspace);

	else {
		GRTagARNotationElement::OnDraw(hdc);
		if (mDoubleTreble)
		{
			const float xOffset = ((mRightSpace + mLeftSpace) * float(0.5)) - lspace * float(0.2);
			OnDrawSymbol( hdc, mSymbol, xOffset, 0 );
		}
		else if (mOctaveStr) // Draws the octava
		{
			const int fontSize = (int)(float(1.5) * lspace * size);
			const VGFont* fontRef = FontManager::FindOrCreateFont( fontSize, FontManager::kDefaultTextFont.c_str());

			const float octX = mPosition.x + mOctaveOffset.x;
			const float octY = mPosition.y +  (mOctaveOffset.y * size);

			hdc.SetTextFont( fontRef );
			unsigned int savedalign = hdc.GetFontAlign();
			hdc.SetFontAlign( VGDevice::kAlignBottom | VGDevice::kAlignCenter );
			hdc.DrawString( octX, octY, mOctaveStr, (int)strlen(mOctaveStr) );
			hdc.SetFontAlign( savedalign );
		}
	}

  	if (colref)
		hdc.SetFontColor( prevFontColor);
}

const ARClef * GRClef::getARClef() const
{
	return /*dynamic*/static_cast<const ARClef*>(getAbstractRepresentation());
}

bool GRClef::operator==(const GRClef & clef) const
{
	return (   mClefBasePitch == clef.mClefBasePitch
			&& mClefBaseOctave == clef.mClefBaseOctave 
			&& mClefStaffLine == clef.mClefStaffLine 
			&& mOctaveStr == clef.mOctaveStr
			&& mOctaveOffset == clef.mOctaveOffset
			&& mDoubleTreble == clef.mDoubleTreble
			&& mSymbol == clef.mSymbol );
}

bool GRClef::operator==(const GRTag & tag) const
{
	const GRClef * clef = dynamic_cast<const GRClef *>(&tag);
	if (clef)
		return this->operator==(*clef);
	return false;
}

#ifdef SMUFL
const NVPoint & GRClef::getReferencePosition() const	{ return fRefpos; }
#else
const NVPoint & GRClef::getReferencePosition() const
{
	switch (mSymbol)
	{
		case kClefPerc:		return refposPerc;
		case kClefViolin:	return refposTreble;
		case kClefBratsche:	return refposAlto;
		case kClefBass:		return refposBass;
		default: 		return refposTreble;
	}
}
#endif
