/*
  GUIDO Library
  Copyright (C) 2016  Grame


  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "MeterVisitor.h"
#include "ARMeter.h"

using namespace std;

#ifndef WIN32
#warning ("TODO: adapat to complex meters with several units");
#endif

//------------------------------------------------------------------------------
void MeterVisitor::reset() {
	for (int i=0; i<kMaxGuidoMeterCounts; i++)
		fLastMeter.count[i] = 0;
	fLastMeter.unit = 4;
}

//------------------------------------------------------------------------------
void MeterVisitor::visitIn (ARMusicalTag* obj)
{
	const ARMeter* meter = dynamic_cast<ARMeter*>(obj);
	if (meter) {
		TYPE_TIMEPOSITION date = meter->getRelativeTimePosition();
		if (date <= fLimit) {
			for (int i=0; i<kMaxGuidoMeterCounts; i++)
				fLastMeter.count[i] = 0;
			fLastMeter.unit = meter->getDenominator();

			const vector<Fraction>& mlist = meter->getMeters();
			size_t n = mlist.size();

			if (n > kMaxGuidoMeterCounts) {		// meter division is bigger than the limit
				int sum = 0;
				for (size_t i = kMaxGuidoMeterCounts-1; i < n; i++)	// computes the sum of the last divs
					sum += mlist[i].getNumerator();
				fLastMeter.count[kMaxGuidoMeterCounts-1] = sum;	// and stores the sum to the last position
				n = kMaxGuidoMeterCounts-1;
			}

			for (size_t i=0; i < n; i++)
				fLastMeter.count[i] = mlist[i].getNumerator();

//			vector<int> counts = meter->getNumeratorsVector();
//			size_t n = counts.size();
//
//			if (n > kMaxGuidoMeterCounts) {		// meter division is bigger than the limit
//				int sum = 0;
//				for (size_t i = kMaxGuidoMeterCounts-1; i < n; i++)	// computes the sum of the last divs
//					sum += counts[i];
//				fLastMeter.count[kMaxGuidoMeterCounts-1] = sum;	// and stores the sum to the last position
//				n = kMaxGuidoMeterCounts-1;
//			}
//
//			for (size_t i=0; i < n; i++)
//				fLastMeter.count[i] = counts[i];

		}
	}
}

//------------------------------------------------------------------------------
void MetersVisitor::reset() {
	fMeters.clear();
}

//------------------------------------------------------------------------------
void MetersVisitor::visitIn (ARMusicalTag* obj)
{
	const ARMeter* meter = dynamic_cast<ARMeter*>(obj);
	if (meter) {
		TYPE_TIMEPOSITION date = meter->getRelativeTimePosition();
		if (date <= fLimit) {
			fMeters.clear();

			const vector<Fraction>& mlist = meter->getMeters();
			size_t n = mlist.size();
			for (size_t i=0; i<n; i++) {
				GuidoMeter meter;
				meter.count[0] = mlist[i].getNumerator();
				meter.count[1] = 0;
				meter.unit = mlist[i].getDenominator();
				fMeters.push_back (meter);
			}
		}
	}
}
//------------------------------------------------------------------------------
GuidoMeters MetersVisitor::getMeters() const
{
	size_t n = fMeters.size();
	GuidoMeters meters = new GuidoMeter[n+1];
	for (size_t i=0; i<n; i++)
		meters[i] = fMeters[i];
	meters[n].count[0] = 0;
	meters[n].unit = 0;
	return meters;
}


