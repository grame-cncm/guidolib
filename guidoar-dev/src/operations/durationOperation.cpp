/*

  Copyright (C) 2007  Grame


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

    Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
    research@grame.fr

*/

#ifdef WIN32
# pragma warning (disable : 4786)
#endif

#include <iostream>

#include "ARFactory.h"
#include "AROthers.h"
#include "ARTag.h"
#include "ARNote.h"
#include "durationvisitor.h"
#include "durationOperation.h"
#include "tree_browser.h"

using namespace std;

namespace guido 
{

//_______________________________________________________________________________
durationOperation::durationOperation() : fFactor(1)
{
	for (int i=0; i<kMaxDivider; i++)
		fPrimeNumbers[i] = false;
	fPrimeNumbers[23] = true;
	fPrimeNumbers[29] = true;
	fPrimeNumbers[31] = true;
	fPrimeNumbers[37] = true;
	fPrimeNumbers[41] = true;
	fPrimeNumbers[43] = true;
	fPrimeNumbers[47] = true;
	fPrimeNumbers[53] = true;
	fPrimeNumbers[59] = true;
	fPrimeNumbers[61] = true;
	fPrimeNumbers[67] = true;
	fPrimeNumbers[71] = true;
	fPrimeNumbers[73] = true;
	fPrimeNumbers[79] = true;
	fPrimeNumbers[83] = true;
	fPrimeNumbers[89] = true;
	fPrimeNumbers[97] = true;
	fPrimeNumbers[101] = true;
	fPrimeNumbers[103] = true;
	fPrimeNumbers[107] = true;
	fPrimeNumbers[109] = true;
	fPrimeNumbers[113] = true;
	fPrimeNumbers[127] = true;
	fPrimeNumbers[131] = true;
	fPrimeNumbers[137] = true;
	fPrimeNumbers[139] = true;
	fPrimeNumbers[149] = true;
	fPrimeNumbers[151] = true;
	fPrimeNumbers[157] = true;
	fPrimeNumbers[163] = true;
	fPrimeNumbers[167] = true;
	fPrimeNumbers[173] = true;
	fPrimeNumbers[179] = true;
	fPrimeNumbers[181] = true;
	fPrimeNumbers[191] = true;
	fPrimeNumbers[193] = true;
	fPrimeNumbers[197] = true;
	fPrimeNumbers[199] = true;
}

//_______________________________________________________________________________
Sguidoelement durationOperation::operator() ( const Sguidoelement& score, float factor )
{
	if (!score) return 0;
	return stretch (score, float2rational(factor));
}

//_______________________________________________________________________________
Sguidoelement durationOperation::operator() ( const Sguidoelement& score, const rational& targetDuration )
{
	if (!score) return 0;
	durationvisitor dv;
	rational dur = dv.duration (score);
	rational r = targetDuration / dur;
	return stretch (score, r.rationalise());
}

//_______________________________________________________________________________
SARMusic durationOperation::operator() ( const SARMusic& score1, const SARMusic& score2 )
{
	if (!score1) return 0;
	if (!score2) return 0;

	durationvisitor dv;
	rational s1d = dv.duration (score1);
	rational s2d = dv.duration (score2);
	rational r = s2d / s1d;
	Sguidoelement elt = stretch (score1, r.rationalise());
	return dynamic_cast<ARMusic*>((guidoelement*)elt);
}

//_______________________________________________________________________________
Sguidoelement durationOperation::stretch ( const Sguidoelement& score, const rational& stretchFactor )
{
	if (!score) return 0;
	fFactor = stretchFactor;
	Sguidoelement outscore;
	if (score) {
		fCurrentDurationIn = fCurrentDurationOut = rational(1,4);
		tree_browser<guidoelement> browser(this);
		browser.browse (*score);
		outscore = fStack.top();
		fStack.pop();
	}
	return outscore;
}

//________________________________________________________________________
rational durationOperation::float2rational ( float val ) const
{
	int denum = 1000000; // to keep 6 significant numbers
	int num = int(val * denum);
	rational r(num, denum);
	r.rationalise();
	return r;
}

//________________________________________________________________________
// a bounded multiplication operation:
// constraints the denominator to values <= maxDenom
// computes the closest approximation when necessary
rational durationOperation::boundedMult ( const rational& dur1, const rational& dur2, int maxDenom ) const
{
	rational result = dur1 * dur2;
	result.rationalise();
	if (result.getDenominator() > maxDenom) {
		float val = float(result);
		float err = 9999;			// used to store the minimum error
		rational approx(0,1);		// used to store the corresponding rational

		for (int div=1; div<kMaxDivider; div++) {
			if (fPrimeNumbers[div]==true) { // avoid prime numbers
				bool done = false;
				float previousErr = 9999;			// used to detect closest approximations
				for (int num=1; !done; num++) {
					rational r(num, div);
					float localVal = float(r);
					float localErr = (localVal > val) ? localVal - val : val - localVal;
					if ( localErr > previousErr) done = true;
					else {
						previousErr = localErr;
						if (localErr < err) {
							err = localErr;
							approx = r;
						}
					}
				}
			}
		}
//cerr << "boundedMult scan for " <<  string(result) << " -> " << string(approx) << endl;
		result = approx;		
	}
	return result;
}

//________________________________________________________________________
rational durationOperation::stretch ( const rational& duration )
{
	
	if (!ARNote::implicitDuration (duration))
		fCurrentDurationIn = duration;
	rational stretched = fCurrentDurationIn;
	stretched.rationalise();
	stretched = boundedMult(stretched, fFactor);

	if (stretched == fCurrentDurationOut)
		stretched.set (ARNote::kUndefinedDuration, 4);
	else
		fCurrentDurationOut = stretched;
	return stretched;
}

//________________________________________________________________________
// The visit methods
//________________________________________________________________________
void durationOperation::visitStart ( SARNote& elt )
{
	SARNote note = copy(elt);
	rational duration = stretch(elt->duration());
	if (note) {
		*note = duration;
		push( note );
	}
}

//________________________________________________________________________
void durationOperation::visitStart ( SARVoice& elt )
{
	fCurrentDurationIn = fCurrentDurationOut = rational(1,4);
	clonevisitor::visitStart(elt);
}

}
