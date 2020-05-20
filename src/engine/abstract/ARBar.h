#ifndef ARBAR_H
#define ARBAR_H

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

#include <vector>

#include "ARMTParameter.h"

/*@mkdoc

@group:Barlines

@tagname:\bar
@tagalias:\|
@tagtype:P
@tagnotation:a simple bar line
@tagdesc
@tagend

@params:
@param:displayMeasNum:boolean:displays measure number:false:true
@param:hidden:boolean:hide bar and prevent measure num increment:false:true
@param:numDx:unit:measure number displacement:0:true
@param:numDy:unit:measure number displacement:0:true
@paramdesc
Measure numbering is attached to barlines. The **displayMeasNum** parameter activates or inhibits measures numbering.

Measure numbering may also be activated at [\meter](../ClefKeyMeter) level.

See the [Barlines](@EXAMPLES/barlines/) example.
@paramend

*/

/** \brief Bar tag
*/
class ARBar : public ARMTParameter
 {
 public:
		typedef std::vector<std::pair<int, int> >	TRanges;
		enum { kNoNum, kNumAll, kNumPage, kNumSystem };
		         ARBar(const TYPE_TIMEPOSITION & timeposition);
		         ARBar();
		virtual ~ARBar() {}

		virtual const char*	getParamsStr() const	{ return kARBarParams; };
		virtual const char*	getTagName () const		{ return "ARBar"; };
		virtual std::string getGMNName () const		{ return "\\bar"; };

		virtual void setTagParameters (const TagParameterMap& params);

		void  setMeasureNumber(int inMeasureNumber)   { fMeasureNumber = inMeasureNumber; }
		int   getMeasureNumber()          const       { return fMeasureNumber; }
        int   getMeasureNumberDisplayed() const       { return fMeasureNumberDisplayed; }
        void  setMeasureNumberDisplayed(int mode)	  { fMeasureNumberDisplayed = mode; }
        bool  isMeasureNumberDisplayedSet() const     { return fMeasureNumberDisplayedIsSet; }

        bool isMeasureNumSkipped() const              { return fSkippedMeasureNum || fHidden; }
        bool hidden() const              			  { return fHidden; }
     
		float getMeasureNumberDxOffset() const        { return numDx; }
		float getMeasureNumberDyOffset() const        { return numDy; }
		
		const ARBar* previousBar () const				{ return fLastBar; }
		void		 setPreviousBar (const ARBar* bar)	{ fLastBar = bar; }
	 
        const ARMusicalObject *isARBar() const			{ return this; }
		const TRanges&	getRanges () const				{ return fRanges; }
		void			setRanges (const TRanges& r)	{ fRanges = r; }

  private:
		TRanges	fRanges;

		int   fMeasureNumber;
        int   fMeasureNumberDisplayed;
        bool  fSkippedMeasureNum;
		float numDx;
		float numDy;
		const ARBar*	fLastBar;

        bool fMeasureNumberDisplayedIsSet;
        bool fHidden = false;
};

#endif
