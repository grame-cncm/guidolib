/*
  GUIDO Library
  Copyright (C) 2006  Grame

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#ifdef WIN32
#pragma warning (disable : 4786)
#endif

#include <stdlib.h>
#include <sstream>
#include "ARNote.h"
#include "visitor.h"

using namespace std;

namespace guido 
{

std::map<std::string, std::pair<char, int> > ARNote::fNormalizeMap;

//______________________________________________________________________________
ARNote::ARNote() 
	:	fOctave(kUndefinedOctave), fAccidental(0), 
		fDots(0), fDuration(kUndefinedDuration,4)
		
{
	if (!fNormalizeMap.size()) {
		fNormalizeMap["c"]   = make_pair('c', 0);
		fNormalizeMap["d"]   = make_pair('d', 0);
		fNormalizeMap["e"]   = make_pair('e', 0);
		fNormalizeMap["f"]   = make_pair('f', 0);
		fNormalizeMap["g"]   = make_pair('g', 0);
		fNormalizeMap["a"]   = make_pair('a', 0);
		fNormalizeMap["b"]   = make_pair('b', 0);
		fNormalizeMap["h"]   = make_pair('b', 0);

		fNormalizeMap["do"]  = make_pair('c', 0);
		fNormalizeMap["re"]  = make_pair('d', 0);
		fNormalizeMap["mi"]  = make_pair('e', 0);
		fNormalizeMap["fa"]  = make_pair('f', 0);
		fNormalizeMap["sol"] = make_pair('g', 0);
		fNormalizeMap["la"]  = make_pair('a', 0);
		fNormalizeMap["si"]  = make_pair('b', 0);
		fNormalizeMap["ti"]  = make_pair('b', 0);

		fNormalizeMap["cis"] = make_pair('c', 1);
		fNormalizeMap["dis"] = make_pair('d', 1);
		fNormalizeMap["fis"] = make_pair('f', 1);
		fNormalizeMap["gis"] = make_pair('g', 1);
		fNormalizeMap["ais"] = make_pair('a', 1);
	}
}

//______________________________________________________________________________
char ARNote::NormalizedPitchName (const string& name, int* alter)
{
	pair<map<string, pair<char, int> >::const_iterator, map<string, pair<char, int> >::const_iterator>
	erp = fNormalizeMap.equal_range( name );
	char outname = 0;
	if (erp.first != erp.second) {
		outname = (*erp.first).second.first;
		if (alter) *alter = (*erp.first).second.second;
	}
	return outname;
}

//______________________________________________________________________________
char ARNote::NormalizedPitchName (int* alter) const
{
	return NormalizedPitchName( getName(), alter );
}

//______________________________________________________________________________
char ARNote::NormalizedPitch2Name (pitch p)
{
	if (p >= A) return 'a' + p - A;
	else if (p >= C) return 'c' + p - C;
	return 0;
}

//______________________________________________________________________________
ARNote::pitch ARNote::OffsetPitch (pitch p, int offset, int& octave, int& alter, int targetInterval)
{
	octave += offset / 12;
	offset %= 12;
	int n = p;
	while (offset > 0) {
		if (n == E) { n++; offset--; }
		else if (n == B) { n = C; octave++; offset--; }
		else if (offset > 1) { n++; offset -= 2; }
		else if (offset == 1) { alter++; offset--; }
	}
	while (offset < 0) {
		if (n == C) { n = B; offset--; octave++; }
		else if (n == F) { n--; octave--; offset++; }
		else if (offset < -1) { n--; offset += 2; }
		else if (offset == -1) { alter--; offset++; }
	}
	offset = n - p - targetInterval;
	if (offset > 0) {
		if (n == B) { n=C; octave++; alter--; }
		else if (n == E) { n++; alter--; }
		else { n++; alter-=2; }
	}
	else if (offset < 0) {
		if (n == C) { n=B; octave--; alter++; }
		else if (n == F) { n--; alter++; }
		else { n--; alter+=2; }
	}
	return pitch(n);
}

//______________________________________________________________________________
ARNote::pitch ARNote::NormalizedName2Pitch	(char note)
{
	switch (note) {
		case 'a':	return A;
		case 'b':	return B;
		case 'c':	return C;
		case 'd':	return D;
		case 'e':	return E;
		case 'f':	return F;
		case 'g':	return G;
	}
	return kNoPitch;
}

//______________________________________________________________________________
ARNote::pitch ARNote::enharmonic (pitch p, int& octave, int& alter)
{
	if (alter > 0) {	
		p = incPitch (p, octave, alter);
		alter--;
	}
	else if (alter < 0) {
		p = decPitch (p, octave, alter);
		alter++;
	}
	return p;
}

//______________________________________________________________________________
ARNote::pitch ARNote::incPitch	(pitch p, int& octave, int& alter)
{
	alter++;
	if (alter >= 2) {
		alter -= 2;
		switch (p) {
			case C:	p = D; break;
			case D:	p = E; break;
			case F: p = G; break;
			case G: p = A; break;
			case A: p = B; break;
			case E:
				p = F;
				alter++;
				break;
			case B:
				p = C;
				alter++;
				octave++;
				break;
			default:
				;
		}
	}
	return p;
}

//______________________________________________________________________________
ARNote::pitch ARNote::decPitch	(pitch p, int& octave, int& alter)
{
	alter--;
	if (alter <= -2) {
		alter += 2;
		switch (p) {
			case D:	p = C; break;
			case E:	p = D; break;
			case G: p = F; break;
			case A: p = G; break;
			case B: p = A; break;
			case F:
				p = E;
				alter--;
				break;
			case C:
				p = B;
				alter--;
				octave--;
				break;
			default:
				;
		}
	}
	return p;
}

//______________________________________________________________________________
ARNote::pitch ARNote::chromaticOffsetPitch	(pitch p, int interval, int& octave, int& alter, bool preferSharp)
{
	if (interval > 0) {
		while (interval) {
			p = incPitch (p, octave, alter);
			interval--;
		}
	}
	else if (interval < 0) {
		while (interval) {
			p = decPitch (p, octave, alter);
			interval++;
		}
	}
	if (preferSharp) {
		if (alter < 0) p = enharmonic (p, octave, alter);
	}
	else if (alter > 0)
		p = enharmonic (p, octave, alter);
	return p;
}

//______________________________________________________________________________
int ARNote::midiPitch (int& currentOctave) const
{
	int alter=0, midi = -1;
	pitch notepitch = GetPitch (alter);

	int octave = fOctave == kUndefinedOctave ? currentOctave : fOctave;
	currentOctave = octave;
	if (notepitch != ARNote::kNoPitch) {
		// offset in octave numeration between guido and midi is 4
		int midioctave = (octave + 4) * 12;
		midi = midioctave + (notepitch*2) + alter;
		if (notepitch > ARNote::E) midi--;
	}
	return midi;
}

//______________________________________________________________________________
ARNote::pitch ARNote::GetPitch (int& alter) const
{
	char npitch = NormalizedPitchName (&alter);
	alter += GetAccidental();
	return NormalizedName2Pitch(npitch);
}

//______________________________________________________________________________
rational ARNote::totalduration(rational& current, int& currentdots) const
{
	rational dur = duration();	// determine the note duration
	if (dur == getImplicitDuration())
		dur = current;			// when implicit, uses the current duration
	else {
		current = dur;			// else update the current duration
		currentdots = 0;		// and reset the current dots count
	}

	int dots = GetDots();		// determine the dots count
	if (dots)
		currentdots = dots;	
	else
		dots = currentdots;		// when no dots, uses the current dots information
		
	int den = 1;
	rational mult = rational(0,1);
	while (dots--)
		mult += rational(1, den *= 2);
	dur += dur * mult;
	dur.rationalise();
	return dur;
}

//______________________________________________________________________________
ARNote& ARNote::operator =  (const rational& d)	{ fDuration  = d; fDuration.rationalise(); return *this; }
ARNote& ARNote::operator += (const rational& d)	{ fDuration += d; fDuration.rationalise(); return *this; }
ARNote& ARNote::operator *= (const rational& d)	{ fDuration *= d; fDuration.rationalise(); return *this; }
ARNote& ARNote::operator /= (const rational& d)	{ fDuration /= d; fDuration.rationalise(); return *this; }
ARNote& ARNote::operator -= (const rational& d)	{ fDuration -= d; fDuration.rationalise(); return *this; }

//______________________________________________________________________________
void ARNote::acceptIn(basevisitor& v) {
	if (visitor<SMARTP<ARNote> >* p = dynamic_cast<visitor<SMARTP<ARNote> >*>(&v)) {
		SMARTP<ARNote> sptr = this;
		p->visitStart(sptr);
	}
	else guidoelement::acceptIn(v);
}

//______________________________________________________________________________
void ARNote::acceptOut(basevisitor& v) {
	if (visitor<SMARTP<ARNote> >* p = dynamic_cast<visitor<SMARTP<ARNote> >*>(&v)) {
		SMARTP<ARNote> sptr = this;
		p->visitEnd(sptr);
	}
	else guidoelement::acceptOut(v);
}

//______________________________________________________________________________
ARNote::operator string() const {
    ostringstream str;

	bool octOut = false;
	str << getName();
	if ((getName() != "_") && (getName() != "empty")) {
		int n = GetAccidental();
		if (n) {
			char c = (n > 0) ? '#' : '&';
			n = abs(n);
			while (n--) str << c;
		}

		n = GetOctave();
		if ( n != ARNote::kUndefinedOctave) {
			str << n;
			octOut = true;
		}
	}

	long n = fDuration.getNumerator();
	if (n != kUndefinedDuration) {
		if ((n != 1) || octOut)  str << '*' << n;
		n = fDuration.getDenominator();
		if (n > 0) str << '/' << n;
	}

	n = GetDots();
	while (n--) str << '.';

	return  str.str();
}

//______________________________________________________________________________
SMARTP<ARNote> ARNote::create()
    { ARNote* o = new ARNote(); assert(o!=0); return o; }

}
