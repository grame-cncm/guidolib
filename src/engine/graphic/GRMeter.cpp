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

//-------------------------------------------------------------------------------------
GRMeter::GRMeter( const ARMeter * ar, GRStaff * curstaff, bool p_ownsAR )
  : GRTagARNotationElement(ar, curstaff->getStaffLSPACE(), p_ownsAR),
	fMeters(ar->getMeters()), fGroupComplex(false)
{
	assert(ar);
	assert(curstaff);

	mNeedsSpring = 1;
	sconst = SCONST_METER;
	fCurLSPACE = curstaff->getStaffLSPACE();
	fNumericHeight	= fCurLSPACE * 0.85;		// adjust numeric glyph height
//	const TagParameterFloat* p = ar->getSize();
//	mTagSize	   *= curstaff->getSizeRatio() * ar->getSize();
//	mTagSize	   *= curstaff->getSizeRatio() * (p ? p->getValue() : 1.f);
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
		int previous = 0;
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
	
	float w, h;
	FontManager::gFontScriab->GetExtent (str.c_str(),  int(str.size()), &w, &h, hdc);
	bb.left  = (GCoord)(- w * 0.5f * mTagSize);
	bb.right = -bb.left;
	// default top position (staff first line) is adjusted to the glyph size
	bb.top = -(mTagSize-1) * fNumericHeight * 2;
	// default bottom position (staff last line) is adjusted to the glyph size
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
void GRMeter::GGSOutput() const
{
//this code is obsolete, it makes use of numerator and denominator values that are not any more
//maintained due to complex meter support
//
//	char buffer[20]; char buf[100];
//	if (error) return;
//	if (fType == ARMeter::NUMERIC) {
//		
//	  snprintf(buf,100,"\\draw_image<\"%d\",%ld,%d,%d>\n",
//			fNumerator, getID(), (int)(mPosition.x + ggsoffsetx), (int)(LSPACE + ggsoffsety));
//	  AddGGSOutput(buf);
//		  
//	  snprintf(buf,100,"\\draw_image<\"%d\",%ld,%d,%d>\n",
//			fDenominator, getID(), (int)(mPosition.x + ggsoffsetx), (int)(3*LSPACE + ggsoffsety));
//	  AddGGSOutput(buf);
//	}
//	else if (fType == ARMeter::C) {
//	  snprintf(buffer,20,"c" /*SCR_ALLABREVE*/ );
//	  snprintf(buf,100,"\\draw_image<\"%s\",%ld,%d,%d>\n",
//			buffer, getID(), (int)(mPosition.x + ggsoffsetx), (int)(mPosition.y + 2 * LSPACE + ggsoffsety));
//	  AddGGSOutput(buf);
//	}
//	else if (fType == ARMeter::C2) {
//	  snprintf(buffer,20,"C" /* SCR_ALLABREVE2 */ );
//	  snprintf(buf,100,"\\draw_image<\"%s\",%ld,%d,%d>\n",
//			buffer, getID(), (int)(mPosition.x + ggsoffsetx), (int)(mPosition.y + 2 * LSPACE + ggsoffsety));
//	  AddGGSOutput(buf);
//	}
}

//-------------------------------------------------------------------------------------
// depending on the relative numerator and denominator size, an offset
// must be applied to the smallest part
// this offset is computed as half of difference between the bounding doxes
std::pair<float,float> GRMeter::GetXOffsets(VGDevice & hdc, const std::string& num, const std::string& dnum) const
{
	NVRect nbb = computeBoundingBox (&hdc, num);
	NVRect dbb = computeBoundingBox (&hdc, dnum);
	pair<float, float> offsets = make_pair(0,0);
	float diff = (nbb.Width() - dbb.Width()) / 2;
	if (diff > 0)	offsets.second = diff;
	else			offsets.first = -diff;
	return offsets;
}

//-------------------------------------------------------------------------------------
// draw a complex meter part (numerator or denominator
void GRMeter::DrawSymbolStr(const char* str, float x, float y, VGDevice & hdc ) const
{
	while (*str) {
		DrawSymbol(hdc, *str, x, y, mTagSize);
		float w, h;
		FontManager::gFontScriab->GetExtent(*str, &w, &h, &hdc);
		x += w * mTagSize;
		str++;
	}
}

//-------------------------------------------------------------------------------------
// draw a meter with a single denominator
float GRMeter::DrawNumericSingle(VGDevice & hdc, const string& num, const string& dnum, float x ) const
{
	NVRect bb  = computeBoundingBox (&hdc, num, dnum);
	pair<float, float> offsets = GetXOffsets(hdc, num, dnum);
	
	float xpos = x + offsets.first;
	// meter ref pos is on staff first line, numeric glyph refpos is on vertical center
	float y = fCurLSPACE - (mTagSize - 1) * fNumericHeight;
	DrawSymbolStr (num.c_str(), xpos, y, hdc);

	xpos = x + offsets.second;
    y += (2 * fCurLSPACE + 1.7 * fCurLSPACE * (mTagSize - 1) );
	DrawSymbolStr (dnum.c_str(), xpos, y, hdc);
	return bb.Width();
}

//-------------------------------------------------------------------------------------
void GRMeter::DrawNumericSeveral(VGDevice & hdc ) const
{
	float wsep, h;
	FontManager::gFontScriab->GetExtent('+', &wsep, &h, &hdc);
	wsep *= mTagSize;
	vector<GRMeter::TSingleMeter> m = meters2metersStr(fMeters);
	float x = -mBoundingBox.Width()/2;
	for (size_t i=0; i < m.size(); i++) {
		if (i) {
			// '+' y position is on the middle line
			DrawSymbol(hdc, '+', x, 2*fCurLSPACE, mTagSize);
			x += wsep;
		}
		x += DrawNumericSingle (hdc, m[i].first, m[i].second, x);
	}
}

//-------------------------------------------------------------------------------------
void GRMeter::OnDraw(VGDevice & hdc) const
{
	if (error || !mDraw || !mShow)	return;

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

