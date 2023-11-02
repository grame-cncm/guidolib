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
#include <sstream>
#include <string.h>

#include "ARMeter.h"
#include "GRMeter.h"
#include "GRDefine.h"
#include "GRMusic.h"
#include "GRStaff.h"
#include "VGDevice.h"
#include "VGFont.h"
#include "TagParameterFloat.h"

#include "GUIDOInternal.h"	// for gGlobalSettings.gDevice
#include "FontManager.h"
#include "secureio.h"

using namespace std;

#define GROUPSEVERAL	1		// control several time sign grouping
//#ifdef SMUFL
//#define TIMESIGSPACE	6
//#else
//#define TIMESIGSPACE	0
//#endif

//-------------------------------------------------------------------------------------
GRMeter::GRMeter( const ARMeter * ar, GRStaff * curstaff, bool p_ownsAR )
  : GRTagARNotationElement(ar, curstaff->getStaffLSPACE(), p_ownsAR),
	fMeters(ar->getMeters()), fGroupComplex(false)
{
	assert(ar);
	assert(curstaff);

	mNeedsSpring = 1;
	sconst = SCONST_METER;
	setGRStaff(curstaff);
	fCurLSPACE = curstaff->getStaffLSPACE();
	fNumericHeight	= fCurLSPACE * 0.85f;		// adjust numeric glyph height
	mTagSize	   *= curstaff->getSizeRatio();
    mIsInHeader		= ar->isInHeader();
	fGroupComplex	= ar->groupComplex();
	fType			= getARMeter()->getMeterType();
	mBoundingBox = computeBoundingBox (gGlobalSettings.gDevice);

	// set leftSpace, rightSpace
	float halfWidth = mBoundingBox.Width() / 2;
	mLeftSpace  = halfWidth;
	mRightSpace = halfWidth + fCurLSPACE;

	// y position correction according to staff lines count - DF sept. 23 2009
	if (curstaff) {
		int linesOffset = curstaff->getNumlines() - 5;
		if (linesOffset)
			mPosition.y += fCurLSPACE * linesOffset / 2;
	}
	if (ar->hidden()) mShow = false;
}

// -----------------------------------------------------------------------------
void GRMeter::setHPosition( float inX )
{
	GRTagARNotationElement::setHPosition(inX);
	mMapping = mBoundingBox;
	mMapping += mPosition + getOffset();
}

// -----------------------------------------------------------------------------
void GRMeter::accept (GRVisitor& visitor)
{
	visitor.visitStart (this);
	visitor.visitEnd (this);
}

//-------------------------------------------------------------------------------------
// converts a list of meters to a list of string pairs
// each pair represents the textual value of a meter in num denum order e.g.
//		3+2/4 =>  ["3+2","4"]
//		3+2/4 + 5/8 =>  ["3","4"], ["2","4"], ["5","8"]
// when fGroupComplex is set, meters that share a common denominator are grouped e.g.
//		3+2/4 + 5/8 =>  ["3+2","4"], ["5","8"]
vector<GRMeter::TSingleMeter> GRMeter::meters2metersStr(const std::vector<Fraction>& meters) const
{
	vector<GRMeter::TSingleMeter> out;
	stringstream strStream;
	size_t n = meters.size();
	if (n == 0) return out;

	if (fGroupComplex) {
		const char * sep = "";
		int previousdnum = 0;
		for (size_t i = 0; i < n; i++) {
			int dnum = meters[i].getDenominator();
			if (previousdnum) {
				if ((previousdnum != dnum)) {
					string numstr = strStream.str();
					strStream.str(string());
					strStream << previousdnum;
					out.push_back(make_pair(numstr, strStream.str()));
					strStream.str(string());
					sep = "";
				}
			}
			previousdnum = meters[i].getDenominator();
			strStream << sep << meters[i].getNumerator();
			sep = "+";
		}
		string numstr = strStream.str();
		strStream.str(string());
		strStream << previousdnum;
		out.push_back(make_pair(numstr, strStream.str()));
		strStream.str(string());
	}
	else {
		for (size_t i = 0; i < n; i++) {
			strStream << meters[i].getNumerator();
			string numstr = strStream.str();
			strStream.str(string());
			strStream << meters[i].getDenominator();
			out.push_back(make_pair(numstr, strStream.str()));
			strStream.str(string());
		}
	}
	return out;
}

//-------------------------------------------------------------------------------------
string GRMeter::makeNumeratorString (const std::vector<Fraction>& meters) const
{
	stringstream strStream;
	const char * sep = "";
	for (size_t i = 0; i < meters.size(); i++) {
		strStream << sep << meters[i].getNumerator();
		sep = "+";
	}
	return strStream.str();
}

//-------------------------------------------------------------------------------------
string GRMeter::makeDenominatorString (const std::vector<Fraction>& meters) const
{
	size_t n = meters.size();
	if (!n) return "";

	stringstream strStream;
	const char * sep = "";
	if (getARMeter()->isSingleUnit())
		strStream << meters[0].getDenominator();
	else if (fGroupComplex) {
		size_t previous = 0;
		for (size_t i = 0; i < n; i++) {
			int d = meters[i].getDenominator();

			if (previous && (d != previous)) {
				strStream << sep << d;
				sep = "+";
			}
			previous = n;
		}
	}
	else {
		for (size_t i = 0; i < n; i++) {
			strStream << sep << meters[i].getDenominator();
			sep = "+";
		}
	}
	return strStream.str();
}

//-------------------------------------------------------------------------------------
// computes the bounding box of a meter strings
NVRect GRMeter::computeBoundingBox (VGDevice* hdc, const string& str) const
{
	NVRect bb;
	if (!hdc)	return bb;

	float w = FontManager::ComputeSymbolsStrWidth(hdc, str);
	bb.left = - w * 0.5f * mTagSize;
	bb.right = -bb.left;
	bb.top = -(mTagSize-1) * fNumericHeight * 2;
	bb.bottom = 4 * fCurLSPACE + (mTagSize-1) * fNumericHeight * 2;
	return bb;
}

//-------------------------------------------------------------------------------------
// computes the bounding box of two meter strings and returns the largest one
NVRect GRMeter::computeBoundingBox (VGDevice* hdc, const std::string& numStr, const std::string& dnumStr) const
{
	NVRect nbb  = computeBoundingBox (hdc, numStr);
	NVRect dbb  = computeBoundingBox (hdc, dnumStr);
	return (nbb.Width() > dbb.Width()) ? nbb : dbb;
}

//-------------------------------------------------------------------------------------
// computes the overall bounding box of a meter
NVRect GRMeter::computeBoundingBox (VGDevice* hdc) const
{
	NVRect bb;
	if (!hdc)	return bb;
	
	if (fType == ARMeter::NUMERIC) {
		bb  = computeBoundingBox (hdc, makeNumeratorString (fMeters), makeDenominatorString (fMeters));
	}
	else if (fType == ARMeter::C || fType == ARMeter::C2) {
		float w = GetSymbolExtent(kCSymbol) * mTagSize;
		bb.left  = (GCoord)(- w * 0.5f);
		bb.right = -bb.left;
		bb.top = -(mTagSize-1) * fNumericHeight;
		bb.bottom = 4 * fCurLSPACE + (mTagSize-1) * fNumericHeight;
	}
	return bb;
}

//-------------------------------------------------------------------------------------
// depending on the relative numerator and denominator size, an offset
// must be applied to the smallest part
// this offset is computed as half of difference between the bounding doxes
std::pair<float,float> GRMeter::GetXOffsets(VGDevice & hdc, const std::string& num, const std::string& dnum) const
{
	NVRect nbb = computeBoundingBox (&hdc, num);
	NVRect dbb = computeBoundingBox (&hdc, dnum);
	pair<float, float> offsets = make_pair(0.f,0.f);
	float diff = (nbb.Width() - dbb.Width()) / 2;
	if (diff > 0)	offsets.second = diff;
	else			offsets.first = -diff;
	return offsets;
}

//-------------------------------------------------------------------------------------
// draw a meter with a single denominator
float GRMeter::DrawNumericSingle(VGDevice & hdc, const string& num, const string& dnum, float x ) const
{
	NVRect bb  = computeBoundingBox (&hdc, num, dnum);
	pair<float, float> offsets = GetXOffsets(hdc, num, dnum);
	
	float xpos = x + offsets.first;
	// meter ref pos is on staff first line, numeric glyph refpos is on vertical center
	float lspace = LSPACE;
	float my = 2 * fCurLSPACE;			// the staff middle line
	float hr = LSPACE / fCurLSPACE;		// a ratio to compensate the staff size and mTagSize
	float space = 0.15f * fCurLSPACE;	// the space above and below the middle line
	float y = my - space - fNumericHeight * mTagSize * hr;
	DrawNumericSymbols (hdc, num.c_str(), xpos, y, mTagSize);
	xpos = x + offsets.second;
    y = my + space + fNumericHeight * mTagSize * hr;
	DrawNumericSymbols (hdc, dnum.c_str(), xpos, y, mTagSize);
	return bb.Width();
}

//-------------------------------------------------------------------------------------
void GRMeter::DrawNumericSeveral(VGDevice & hdc ) const
{
	float wsep, h;
#ifdef SMUFL
	ConstMusicalSymbolID plus = kMeterPlusSymbol;
#else
	ConstMusicalSymbolID plus = '+';
#endif
	FontManager::gFontScriab->GetExtent(plus, &wsep, &h, &hdc);
	wsep *= mTagSize;
	vector<GRMeter::TSingleMeter> m = meters2metersStr(fMeters);
	float x = -mBoundingBox.Width()/2;
	for (size_t i=0; i < m.size(); i++) {
		if (i) {
			// '+' y position is on the middle line
			DrawSymbol(hdc, plus, x, 2*fCurLSPACE, mTagSize);
			x += wsep;
		}
		x += DrawNumericSingle (hdc, m[i].first, m[i].second, x);
	}
}

//-------------------------------------------------------------------------------------
void GRMeter::OnDraw(VGDevice & hdc) const
{
	if (getError() || !mDraw || !mShow)	return;

	const unsigned char * colref = getColRef();
	const VGColor prevFontColor = hdc.GetFontColor();
  	if (colref)
		hdc.SetFontColor( VGColor( colref ));

	if (fType == ARMeter::NUMERIC) {
		if (getARMeter()->isSingleUnit())	DrawNumericSingle  (hdc, makeNumeratorString (fMeters), makeDenominatorString(fMeters), mBoundingBox.left);
		else								DrawNumericSeveral (hdc);
	}
	else {
		float y = 2 * fCurLSPACE;		// ref pos is on the staff first line, glyph starts on second line
		if (fType == ARMeter::C)
			DrawSymbol(hdc, kCSymbol, mBoundingBox.left, y, mTagSize);
		else if (fType == ARMeter::C2)
			DrawSymbol(hdc, kC2Symbol, mBoundingBox.left, y, mTagSize);
	}

//	NVRect r = mBoundingBox + mPosition;
//	hdc.Frame(r.left, r.top, r.right, r.bottom);

	if (colref)
		hdc.SetFontColor( prevFontColor );  //(TODO: in a parent method)
}

//-------------------------------------------------------------------------------------
const ARMeter*	GRMeter::getARMeter() const		{ return static_cast<const ARMeter*>(getAbstractRepresentation()); }


// ----------------------------------------------------------------------------
/** \brief Retrieves the mapping
*/
void GRMeter::GetMap( GuidoElementSelector sel, MapCollector& f, MapInfos& infos ) const
{
	if (sel == kMeterSel)
		SendMap (f, getRelativeTimePosition(), getDuration(), kMeter, infos);
}
