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

#include <cmath>
#include <string.h>

#include "VGDevice.h"
#include "GRDefine.h"
#include "FontManager.h"

// - Guido AR
#include "ARMusicalEvent.h"
#include "ARRest.h"
#include "ARNote.h"

// - Guido GR
#include "GRTuplet.h"
#include "GRSingleNote.h"
#include "GRSingleRest.h"
#include "GRStaff.h"

#include "secureio.h"

// ===========================================================================
// 		GRTupletState
// ===========================================================================
GRTupletState::GRTupletState(const GRTuplet & inTuplet )
{
	*this = inTuplet.mTupletState;
	if (inTuplet.mTupletElements)
		elements = new NEPointerList(*inTuplet.mTupletElements);
	else
		elements = 0;
}

GRTupletState::~GRTupletState()
{
	delete elements;
}

// ===========================================================================
// 		GRTuplet
// ===========================================================================
GRTuplet::GRTuplet(GRStaff * inStaff, const TYPE_DURATION & p_basenote, Fraction p_relation)
	: mTupletElements(NULL)
{
	staff = inStaff;

	// here the base-notes are "cleaned" (normalised) 
	// .> only exponents of 2 for the numerator

	// This is done away with. We allow arbitrary stuff.
	// Whatever this implies (i don't know)
	// long i = floor ( log(p_basenote.getNumerator()) /log(2) );
	// i = pow(2,i);
	// mTupletState.base = TYPE_DURATION( i, p_basenote.getDenominator() );

	mTupletState.base = p_basenote;
	mTupletState.base.normalize();

	mTupletState.relation = p_relation;

	TYPE_DURATION tmp (mTupletState.base * mTupletState.relation);
	const int expstd = tmp.isMultiple(2); // standarddarstellbar ...
	if (expstd == -1)
	{
		mTupletState.relation = calcRelation();
	}

	// how do I react to overflow ... ?
	mTupletState.duration = mTupletState.base * mTupletState.relation.getNumerator();

	mTupletState.starttimepos = DURATION_0;
	mTupletState.endtimepos = DURATION_0;
}

GRTuplet::GRTuplet(GRStaff * inStaff, const GRTupletState & tpls)
	: mTupletElements(NULL)
{
	staff = inStaff;
	mTupletState = tpls;
	mTupletState.starttimepos = tpls.endtimepos;
	mTupletState.endtimepos = mTupletState.starttimepos;
	
	if (mTupletState.elements)
	{
	  	mTupletElements = new NEPointerList(*mTupletState.elements);
	}
}

GRTuplet::~GRTuplet()
{
	if (mTupletElements)
	{
		// delete the associations
		GuidoPos pos = mTupletElements->GetHeadPosition();
		while (pos)
		{
			GRNotationElement * el = mTupletElements->GetNext(pos);
			if( el ) el->removeAssociation(this);
		}
		delete mTupletElements;
		mTupletElements = 0;
	}
}

TYPE_DURATION GRTuplet::calcRelation()
{
	// calculates/derives from base a relation so that
	// durtempl can be displayed with standard notes (if they fit to base)

	// idea: starting with base it will be searched 
	// for the next bigger standardnote (base=2) then 
	// do a multiplication with the inversion of the base note

	const int tmpl = mTupletState.base.getDenominator();
	if (tmpl >= 129)
		return TYPE_DURATION(1,1);

	const int k = (int) floor( -log( (double) mTupletState.base) / log(2.0) );

	int power;
	if (k<0)
		power = 1;
	else
		power = (int) pow(2.0, k);

	const TYPE_DURATION res ( TYPE_DURATION(mTupletState.base.getDenominator(),
										mTupletState.base.getNumerator()) *
										TYPE_DURATION(1,power));
	return res;

}

/** returns NULL if the event does not fit in the currenct tuplet-base.
*/
GREvent * GRTuplet::addEvent(ARMusicalEvent * inArEvent, TYPE_TIMEPOSITION von,
				TYPE_DURATION & bis)
  {
	// much logic in!

	// there exist three cases
	// 1. mTupletState.endtimepos + bis == duration
	// 	ready
	// 2. mTupletState.endtimepos + bis > duration
	// cut from "von" to "bis", until duration reached: ready
	// 3. mTupletState.endtimepos + bis < duration
	// try to get the complete group (complete smallest)
	// 	(e.g. relation.getDenominator()* standardnote (d.h. basis 2))
	//    is equal to n*1/2^k
	//	    if yes, change duration and base
	//		if not, just add, not ready
	//    Do "just add" so that
	//     smallest possible unit will be added


	// check first if denominator of new note fits to tuplet

	TYPE_DURATION tmpd1 (1, 1);

	if (bis != mTupletState.base)
	{
		tmpd1 = TYPE_DURATION(
			bis.getNumerator() * mTupletState.base.getDenominator(),
			bis.getDenominator() * mTupletState.base.getNumerator());

		tmpd1.normalize();
	}
	if (tmpd1.getDenominator()!=1 && tmpd1.getNumerator()!=1)
	{
		// ATTENTION we are not in the correct base any longer!
		return NULL;

	}

	// test if the duration of "bis" is smaller than 
	// the previous base; if yes a new base will be selected which
	// changes the tuplet length
	if (bis < mTupletState.base)
	{
		TYPE_DURATION tmpd = mTupletState.base, tmpd2 = mTupletState.duration;
		while ( tmpd2 > mTupletState.endtimepos)
		{
		  // tmpd is the new base
			tmpd = mTupletState.base.getBiggestFullNote( mTupletState.base.getDenominator() );
			if (tmpd == mTupletState.base)
			{
				break;
			}
			tmpd2 = tmpd * mTupletState.relation.getNumerator();
			if (tmpd2 > mTupletState.endtimepos)
			{
				mTupletState.base = tmpd;
				mTupletState.duration = tmpd2;
			}
		}
	}

	TYPE_TIMEPOSITION tpos ( mTupletState.endtimepos + bis);
	if (tpos >= mTupletState.duration)
	{
		bis = mTupletState.duration - mTupletState.endtimepos; // schneide wenn noetig etwas ab ...
	}
	else
	{   // check if it could be a smaller group
		int expstd = tpos.isMultiple(2); // standard representation
		if (expstd>=0)
		{ // smaller grouping possible
			mTupletState.duration = tpos;
			mTupletState.base = mTupletState.duration * TYPE_DURATION(1,
								mTupletState.relation.getNumerator());
		}
	}


	// dtempl is the duration template for the representation
	// because tuplets can be represented/displayed only with 
	// even valued notes (no dots) automatically
	// getBiggestFullNote will e called.
	// the duration of "bis" will be changed accordingly
	GREvent * newGREvent = 0;

	TYPE_DURATION dtempl = bis * mTupletState.relation;

	if (dtempl.getDenominator() < 100 )
	{
		dtempl = dtempl.getBiggestFullNote(2);

		if (dtempl.getDenominator() > 32 ) // ATTENTION: don't use hardcoded number
												// or get from .h file
		{
		  	dtempl = bis * mTupletState.relation; // I create a non displayable note
							// bis will not be changed
		}
		else
			bis = dtempl * TYPE_DURATION(mTupletState.relation.getDenominator(),
													mTupletState.relation.getNumerator());
	}

	// Attention,  what happens if the duration does not fit right?!?

	ARNote * arNote;
	ARRest * arRest;
	
	if(( arNote = dynamic_cast<ARNote *>(inArEvent)) != 0 )
	{
		newGREvent =  new GRSingleNote( staff, arNote, von, bis );
		static_cast <GRSingleNote *>(newGREvent)->doCreateNote( dtempl );
	}
	else if(( arRest = dynamic_cast<ARRest *>(inArEvent)) != 0 )
	{
		newGREvent = new GRSingleRest( staff, arRest, von, bis, dtempl );
	}

	newGREvent->addAssociation(this); // the tuplet will be associated with GREvent
	if (mTupletElements == 0)
		mTupletElements = new NEPointerList();

	mTupletElements->AddTail(newGREvent);

	inArEvent->addGRRepresentation(newGREvent);

	mTupletState.endtimepos += bis;

	return newGREvent;
}

bool GRTuplet::isFull() const
{
	if (mTupletState.endtimepos == mTupletState.duration)
		return true;
  
 	return false;
}

void GRTuplet::setPosition(const NVPoint & )
{
}

void GRTuplet::tellPosition(GObject * caller, const NVPoint &)
{
	// here the positions of the single tuplet elements can be retrieved

	// the alg- must prevent that the calculated line does not 
	// go through notes

	// 1. version: line with acceleration 0 below the note heads
	
	const NVRect & callerBox = caller->getBoundingBox();
	const NVPoint & pos = caller->getPosition();

	if ( caller == ((GObject*) mTupletElements->GetHead()))
	{
			// this is the first call
			mPosition.x = pos.x;
			mPosition.y = pos.y + callerBox.bottom;

			mTupletOffsets[0].x = 0;
			mTupletOffsets[0].y = (float(0.75) * LSPACE);
	}
	else	// another Element
	{ 
		if (pos.y + callerBox.bottom > mPosition.y)
				mPosition.y = pos.y + callerBox.bottom;
	}

	if( caller == ((GObject*)mTupletElements->GetTail()))	// last Element ...
	{ 
		mTupletOffsets[1].x = pos.x - mPosition.x + callerBox.right;
		mTupletOffsets[1].y = mTupletOffsets[0].y;
		mTupletOffsets[2].x = mTupletOffsets[1].x;

		if (isFull())
			mTupletOffsets[2].y = 0;
		else
			mTupletOffsets[2].y = mTupletOffsets[0].y; // realisation of the open tiplet 

		// now the text part
		mTupletOffsets[3].x = (mTupletOffsets[2].x - mTupletOffsets[0].x) / 2 - LSPACE;
		mTupletOffsets[3].y = 0;
		mTupletOffsets[4].x = mTupletOffsets[3].x + 2*LSPACE ; // actual textwidth
		mTupletOffsets[4].y = 2*LSPACE;
	}

}

TYPE_DURATION GRTuplet::getDurTemplate(TYPE_DURATION d) const
{
	return d;
}

void GRTuplet::print() const
{
	fprintf(stderr,"GRTuplet\n");
}

void GRTuplet::GGSOutput() const
{
}

/** \brief Draws the n-tuplet bow (or not).
*/
void GRTuplet::OnDraw( VGDevice & hdc ) const
{ 
	char buffer[30];

	if(( mTupletElements == 0 ) || ( mTupletElements->empty())) return;

	const Fraction & relation = mTupletState.relation;
	const int num = relation.getNumerator();

	if (num != 3 && num != 5 && num != 7)
		snprintf( buffer,30,"%d:%d", relation.getNumerator(), relation.getDenominator());
	else
		snprintf(buffer,30,"%d", relation.getNumerator());

	// draw an open bracket if tuplet is not complete
	int starter = 0;
	float x, y;
	if (mTupletState.starttimepos != DURATION_0)
	{
		x = mPosition.x + mTupletOffsets[0].x;
		y = mPosition.y + mTupletOffsets[0].y;
		starter = 1;
	}
	else {
		x = mPosition.x;
		y = mPosition.y;
	}

	// - Todo: SelectPen
	for (int i=starter; i < 3; i++) {
		hdc.Line( x, y, mPosition.x + mTupletOffsets[i].x , mPosition.y + mTupletOffsets[i].y );
		x = mPosition.x + mTupletOffsets[i].x;
		y = mPosition.y + mTupletOffsets[i].y;
	}

	// now the "n" at the bow
	hdc.SetTextFont( FontManager::gFontText );	// will be automatic soon

	float tmpx = 0;
	if (mTupletOffsets[3].x <= 0)
		tmpx = LSPACE - mTupletOffsets[3].x;

	hdc.SetFontAlign( VGDevice::kAlignLeft | VGDevice::kAlignBase );
	hdc.DrawString( mPosition.x + mTupletOffsets[3].x + tmpx,
					mPosition.y + mTupletOffsets[3].y + (2.5f * LSPACE), buffer, (int)strlen(buffer) );
}

void GRTuplet::setHorizontalSpacing()
{
}

const TYPE_DURATION & GRTuplet::getDuration() const
{
	return mTupletState.duration;
}

const TYPE_TIMEPOSITION & GRTuplet::getRelativeTimePosition() const
{
	return (TYPE_TIMEPOSITION &) DURATION_0;
}

TYPE_TIMEPOSITION GRTuplet::getRelativeEndTimePosition() const
{
	return (TYPE_TIMEPOSITION) DURATION_0;
}

void GRTuplet::finish()
{
	// now set the positions ...
	if (mTupletElements && !mTupletElements->empty())
	{
		GRNotationElement * el = mTupletElements->GetTail();
		tellPosition( el, el->getPosition());
	}
}

void GRTuplet::setTupletState(const GRTupletState & tpl)
{

	mTupletState = tpl;
}

void GRTuplet::removeEvent(GREvent * ev)
{
	if( ev )
	{
		ev->removeAssociation(this);
		if( mTupletElements )
			mTupletElements->RemoveElement(ev);
		
		mTupletState.duration -= ev->getDuration();
	}	
	finish();
}

void GRTuplet::removeAssociation(GRNotationElement * el)
{
	if (el && mTupletElements && mTupletElements->RemoveElement(el))
		mTupletState.duration -= el->getDuration();
}
