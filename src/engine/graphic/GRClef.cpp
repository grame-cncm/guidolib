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

#include <string.h>

#include "ARClef.h"
#include "GRClef.h"

#include "GRStaff.h"
#include "GRDefine.h"
#include "GRMusic.h"
#include "FontManager.h"

#include "VGDevice.h"

#include "GUIDOEngine.h"	// for AddGGSOutput 
#include "secureio.h"

#include <iostream>
using namespace std;

const char * const clef_cp8 = "8";
const char * const clef_cp15 = "15";

NVPoint GRClef::refposPerc;
NVPoint GRClef::refposTreble;
NVPoint GRClef::refposBass;
NVPoint GRClef::refposAlto;
unsigned int GRClef::sClefTextAlign;

GRClef::GRClef(const ARClef * arClef, GRStaff *curstaff, bool ownsAR)
 				 : GRTagARNotationElement(arClef, curstaff->getStaffLSPACE(), ownsAR),
    				mOctaveStr(0)
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
			mBoundingBox.top = -  (float(4.1) * curLSPACE);
			mBoundingBox.bottom =  (float(2.7) * curLSPACE);
			break;
		}

		case ARClef::BASS:
		{
			mSymbol = kClefBass;
			mClefBasePitch = NOTE_F;
			mPosition.y = (mClefStaffLine * curLSPACE);
			mBoundingBox.top = (-1 * curLSPACE);
			mBoundingBox.bottom = (3 * curLSPACE);
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

		default:
		{ // Here later gg and perc must be realised also
			mSymbol = kClefViolin;
			mClefBasePitch = NOTE_G;
			mClefBaseOctave += 1;
			// default for mClefStaffLine is 3
			mPosition.y = (mClefStaffLine * curLSPACE);

			mBoundingBox.top = - ( float(4.1) * curLSPACE ); // was 5.1 (?)
			mBoundingBox.bottom = ( float(2.7) * curLSPACE);
		}
	}

	ARClef::octavatype oct = arClef->getOctavaType();
	if (oct != ARClef::NONE) // d.h. -8,+8,-15,+15
	{
		mOctaveOffset.x  = 0; // (GCoord)(curLSPACE*1.25f) - mBoundingBox.left;
		const float textHeight = float(1.5) * LSPACE; 
		
		switch( oct )
		{
			case ARClef::LOW8:
			{
				mOctaveStr = clef_cp8;
				mBoundingBox.bottom += textHeight;
				mOctaveOffset.y = mBoundingBox.bottom;
				break;
			}
			case ARClef::HIGH8:
			{
				mOctaveStr = clef_cp8;
				mOctaveOffset.y = mBoundingBox.top;
				mBoundingBox.top -= textHeight;
				break;
			}
			case ARClef::LOW15:
			{
				mOctaveStr = clef_cp15;
				mBoundingBox.bottom += textHeight;
				mOctaveOffset.y = mBoundingBox.bottom;
				break;
			}
			case ARClef::HIGH15:
			{
				mOctaveStr = clef_cp15;
				mOctaveOffset.y = mBoundingBox.top;
				mBoundingBox.top -= textHeight;
				break;
			}			
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
	mLeftSpace = ((extent * 0.5f + LSPACE / 5 ) * mTagSize);
	mRightSpace = ((extent * 0.5f + LSPACE / 5 ) * mTagSize);

	if( mDoubleTreble )
		mRightSpace += extent;

	mBoundingBox.left = -mLeftSpace;
	mBoundingBox.right = mRightSpace;

	// this can only be done ONCE (during construction)
	mTagSize *= curstaff->getSizeRatio();

	// this information must be read from the font (or a table/map) ....
	// this is static information!
	// reference-positions are also always according to STANDARD-size characters (staffFormat 3pt per Halfspace)
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
	sClefTextAlign = (VGDevice::kAlignLeft | VGDevice::kAlignBase);
	mGrStaff = curstaff;

    mIsInHeader = arClef->isInHeader();
}

GRClef::~GRClef()
{
}

// -----------------------------------------------------------------------------
void GRClef::accept (GRVisitor& visitor)
{
	visitor.visitStart (this);
	visitor.visitEnd (this);
}

// -----------------------------------------------------------------------------
void GRClef::OnDraw(VGDevice & hdc) const
{
	if (error)  return;
	if (!mDraw || !mShow) return;

	GRTagARNotationElement::OnDraw(hdc);
	if (mDoubleTreble)
	{
		const float xOffset = ((mRightSpace + mLeftSpace) * float(0.5)) - LSPACE * float(0.2);
		OnDrawSymbol( hdc, mSymbol, xOffset, 0 );
	}
	else if (mOctaveStr) // Draws the octava
	{
		const int fontSize = (int)(float(1.5) * LSPACE);
		NVstring textFont ( FontManager::kDefaultTextFont );
		const VGFont* fontRef = FontManager::FindOrCreateFont( fontSize, &textFont );

		const float octX = mPosition.x + mOctaveOffset.x;
		const float octY = mPosition.y +  mOctaveOffset.y;

		hdc.SetTextFont( fontRef );
		unsigned int savedalign = hdc.GetFontAlign();
		hdc.SetFontAlign( VGDevice::kAlignBottom | VGDevice::kAlignCenter );
		hdc.DrawString( octX, octY, mOctaveStr, (int)strlen(mOctaveStr) );
		hdc.SetFontAlign( savedalign );
	}
}

void GRClef::GGSOutput() const
{
	char buffer[200];
	const char * cleftype;
	switch ( mSymbol )
	{
		case kClefViolin:	cleftype = "treble_clef";	break;
		case kClefBass:		cleftype = "bass_clef";		break;
		case kClefBratsche:	cleftype = "alto_clef";		break;
		default:	cleftype = NULL;
	}
	
	if (cleftype != NULL)
	{
		snprintf(buffer, 200, "\\draw_image<\"%s\",%ld,%d,%d>\n",
			cleftype,
			getID(),
			(int)(mPosition.x + mBoundingBox.left + ggsoffsetx),
			(int)(mPosition.y + 4 * LSPACE + ggsoffsety));
		
		AddGGSOutput(buffer);
	}
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







