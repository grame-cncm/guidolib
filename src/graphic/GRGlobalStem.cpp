/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARShareStem.h"
#include "ARTStem.h"
#include "ARDisplayDuration.h"
#include "ARNoteFormat.h"

#include "TagParameterString.h"
#include "TagParameterFloat.h"

#include "GRGlobalStem.h"

#include "GRStem.h"
#include "GRStaff.h"
#include "GREmpty.h"
#include "GRSpring.h"
#include "GRFlag.h"
#include "GRSingleNote.h"
#include "GRSystem.h"
#include "GRSystemTag.h"
#include "GRStdNoteHead.h"
#include "GRVoice.h"
#include "GRSystemSlice.h"


GRGlobalStem::GRGlobalStem( GRStaff * inStaff,
						   ARShareStem * pshare,
						   ARTStem * curstemstate,
						   ARDisplayDuration * curdispdur,
						   ARNoteFormat * curnoteformat ) 
: GRPTagARNotationElement(pshare),
  mFlagOnOff(true),stemdirset(false),stemlengthset(false),
  stemdir(dirOFF), lowerNote(NULL), higherNote(NULL) // , colref(NULL)
{
	if (curdispdur && curdispdur->getDisplayDuration() > DURATION_0)
	{
		dispdur = curdispdur->getDisplayDuration();
	}

	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;

	sse->grsystem = inStaff->getGRSystem();
	sse->startElement = NULL;
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	sse->endElement = NULL;
	sse->p = NULL;

	mStartEndList.AddTail(sse);

	stemstate = curstemstate;
	theStem = NULL;
	theFlag = NULL;
	stemlengthset = false;

	if (curnoteformat)
	{
		const TagParameterFloat * tmp = curnoteformat->getSize();
		if (tmp)
			mTagSize = tmp->getValue();
		else
			mTagSize = inStaff->getSizeRatio();

		// color ...

		const TagParameterString * tmps = curnoteformat->getColor();
		if (tmps)
		{
			if (!mColRef)
				mColRef = new unsigned char[ 4 ];
			tmps->getRGB(mColRef);
		}

		// offset ....
		const TagParameterFloat * tmpdx = curnoteformat->getDX();
		const TagParameterFloat * tmpdy = curnoteformat->getDY();

		if (tmpdx)
		{
			mTagOffset.x = (GCoord)(tmpdx->getValue(inStaff->getStaffLSPACE()));
		}
		if (tmpdy)
		{
			mTagOffset.y = (GCoord)(tmpdy->getValue(inStaff->getStaffLSPACE()));
            mTagOffset.y = -mTagOffset.y;
		}
	}
	else
	{
		mTagSize = inStaff->getSizeRatio();
	}

	mFirstEl = NULL;
}

GRGlobalStem::~GRGlobalStem()
{
	// we just remove any association manually
	if (mAssociated)
	{
		GuidoPos pos = mAssociated->GetHeadPosition();
		while(pos)
		{
			GRNotationElement * el = mAssociated->GetNext(pos);
			if( el ) el->removeAssociation(this);
		}
	}
	if (mFirstEl)
	{
		mFirstEl->removeAssociation(this);
	}

	delete theStem;	theStem = 0;
	delete theFlag; theFlag = 0;
//	if (colref)	delete [] colref;
}

void GRGlobalStem::addAssociation(GRNotationElement * grnot)
{
	if (error) return;
	
	GREvent * ev = GREvent::cast(grnot);
	if( ev )
	{
		TYPE_DURATION evdur (ev->getDuration());
		TYPE_DURATION durtempl;
		GRSingleNote * sngnot = dynamic_cast<GRSingleNote *>(ev);
		if (sngnot)
		{
			durtempl = sngnot->getDurTemplate();
		}
		
		// this changes the display-duration
		// (nested display-duration-tags!)
		if (durtempl>evdur && durtempl> dispdur)
			dispdur = durtempl;

		// take the length ...
		if (evdur > DURATION_0)
		{
			if (mFirstEl)
			{
				// then we already have one duration event. In this
				// case, the association is added because an other
				// voice uses this globalstem because of a \chord<label>-Tag
				ev->setGlobalStem(this);
				ev->setNeedsSpring(1);
			}
			else
			{
				// then our springconstant changes ...
				ev->setGlobalStem(this);
				if (!mFirstEl)
					ev->setNeedsSpring(1);
			}
		}
		else
		{
			ev->setGlobalStem(this);
			if (!mFirstEl)
				ev->setNeedsSpring(1);
		}
	}

	// This sets the first element in the range ...
	if (mFirstEl == 0)
	{
		// this associates the first element with
		// this tag .... 
		mFirstEl = grnot;
		mFirstEl->addAssociation(this);

		return;

		// the firstElement is not added to the associated
		// ones -> it does not have to told anything!?
	}

	GRPTagARNotationElement::addAssociation(grnot);

	// this is needed, because the share location can
	// be used in different staves. Elements that need
	// be told of the location can be deleted before this
	// tag, therefor we must know, if these elements are
	// deleted. 
	// The recursive cycle with tellPosition is broken,
	// because only the first element really sets the
	// position. And the first is not included in the
	// own mAssociated list.
	grnot->addAssociation(this);
}

void GRGlobalStem::removeAssociation(GRNotationElement * grnot)
{
    if (grnot == mFirstEl)
	{
		GRNotationElement::removeAssociation(grnot);
		mFirstEl = 0;
	}
	else
		GRPTagARNotationElement::removeAssociation(grnot);
}

GDirection GRGlobalStem::getStemDir() const
{
	if (theStem == 0) return dirOFF;
	return theStem->getStemDir();
}

void GRGlobalStem::RangeEnd( GRStaff * inStaff)
{
	if (error || mFirstEl == 0) return;

	GRPTagARNotationElement::RangeEnd(inStaff);
	if (inStaff == 0) return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct(inStaff->getGRSystem());

	// this checks, wether all associated elements are
	// on the same staff. If not, we build a new
	// GRSystemTag that gets added to the system
	// so that an update on all positions can be made ....
	// (this is taken from GRBeam)
	GuidoPos syststpos = sse->startpos;
	if (tagtype != GRTag::SYSTEMTAG && syststpos)
	{
		// this is all done so that I really get a correct first staff to
		// test my stuff ...
		while (syststpos && !/*ynamic_cast<GRNotationElement *>*/(mAssociated->GetAt(syststpos)))
		{
			mAssociated->GetNext(syststpos);
		}
//		const GRStaff *tststaff = mAssociated->GetNext(syststpos)->getGRStaff();
		int tststaffnum = mAssociated->GetNext(syststpos)->getStaffNumber(); 
		while (syststpos)
		{
			GRNotationElement * el = mAssociated->GetNext(syststpos);
			if (el)
			{
				if (el->getStaffNumber() != tststaffnum) // el->getGRStaff() != tststaff)
				{
					tagtype = GRTag::SYSTEMTAG;
					GRSystemTag * mysystag = new GRSystemTag(this);
//					sse->grsystem->addSystemTag(mysystag);
					el->getGRSystemSlice()->addSystemTag(mysystag);
					break;
				}
			}		
		}
	}

	if (tagtype != GRTag::SYSTEMTAG)
	{
		// check, whether firstel is on the same staff?
		if (mFirstEl && mAssociated && mAssociated->GetHead())
		{
			if (mFirstEl->getStaffNumber() != mAssociated->GetHead()->getStaffNumber() ) // getGRStaff() != mAssociated->GetHead()->getGRStaff())
			{
					tagtype = GRTag::SYSTEMTAG;
					GRSystemTag * mysystag = new GRSystemTag(this);
//					sse->grsystem->addSystemTag(mysystag);
					mFirstEl->getGRSystemSlice()->addSystemTag(mysystag);
			}
		}
	}

	
	GRNotationElement * el = /*dynamic cast<GRNotationElement *>*/(this);
	
	const NEPointerList * associated = el ? el->getAssociations() : 0;
	if (associated == 0) 
		return;

	// now I have the associations ...
	// I have to build the stem ....
	delete theStem;

	theStem = new GRStem(this);
	if (mColRef)
		theStem->setColRef( mColRef );

	// the Vertical position of the Notes that share a stem
	// must be already set ....

	// mHighestY = mLowestY = -32767;
	int highestlowestset = 0;

	// only if the stemdir has not been set ....
	if (stemdir == dirOFF)
	{
		stemdir = dirUP;
		if (stemstate)
		{
			if (stemstate->getStemState() == ARTStem::UP)
			{
				// we have to determine the direction ourselves.
				stemdir = dirUP;
				
			}
			else if (stemstate->getStemState() == ARTStem::DOWN)
			{
				// we have to determine the direction ourselves.
				stemdir = dirDOWN;
				
			}
			else if (stemstate->getStemState() == ARTStem::OFF)
			{
				// we have to determine the direction ourselves.
				stemdir = dirOFF;
				
			}
		}

		if ( ( stemstate && stemstate->getStemState() == ARTStem::AUTO  )
			|| !stemstate)
		{			
			// we have to determine the direction ourselves.
			
			// this needs to be done with 
			// the direction of notes ...
			GCoord middle = 0;
			int count = 0;
			// determine the lowest and highest position ...
			el = associated->GetTail();
			if (el)
			{
				middle = el->getPosition().y;
				if (tagtype == GRTag::SYSTEMTAG && el->getGRStaff())
				{
					middle += (GCoord)el->getGRStaff()->getPosition().y;
				}
				mHighestY = middle;
				mLowestY = middle;
				++ count;

                if (dynamic_cast<GRSingleNote *>(el))
				    higherNote = lowerNote = dynamic_cast<GRSingleNote *>(el);
			}
			
			GuidoPos pos = associated->GetHeadPosition();
			while (pos && pos != associated->GetTailPosition())
			{
				GRNotationElement * el = associated->GetNext(pos);
				if (el && !dynamic_cast<GREmpty *>(el))
				{
					GCoord ypos = el->getPosition().y;
					if (el->getGRStaff() && tagtype == GRTag::SYSTEMTAG)
						ypos += el->getGRStaff()->getPosition().y;
					middle += ypos;
					++count ;
					
					if (mLowestY > ypos)
					{
						mLowestY = ypos;
						lowerNote = (GRSingleNote *)el;
					}
					if (mHighestY < ypos)
					{
						mHighestY = ypos;
						higherNote = (GRSingleNote *)el;
					}
				}
			}
			
			highestlowestset = 1;
			
			if (count > 0)
				middle /= count;
			
			const float curLSPACE = (float)(inStaff->getStaffLSPACE());
			
			const float mylowesty = 2 * curLSPACE - mLowestY;
			const float myhighesty = mHighestY - 2 * curLSPACE;
			
			if (mylowesty > myhighesty)
			{
				stemdir = dirDOWN;
			}
			else if (myhighesty > mylowesty)
				stemdir = dirUP;
			else
			{
				if (middle >= curLSPACE * 2)
				{
					stemdir = dirUP;
				}
				else if (middle < curLSPACE * 2)
				{
					stemdir = dirDOWN;
				}
			}
		}
		else // - If stem's direction is fixed by the user, we have to determine lower and higher chord note all the same
		{
			GCoord middle = 0;
			int count = 0;
			el = associated->GetTail();

			if (el)
			{
				middle = el->getPosition().y;
				if (tagtype == GRTag::SYSTEMTAG && el->getGRStaff())
				{
					middle += (GCoord)el->getGRStaff()->getPosition().y;
				}
				mHighestY = middle;
				mLowestY = middle;
				++ count;

				lowerNote = (GRSingleNote *)el;
				higherNote = (GRSingleNote *)el;
			}

			GuidoPos pos = associated->GetHeadPosition();
			while (pos && pos != associated->GetTailPosition())
			{
				GRNotationElement * el = associated->GetNext(pos);
				if (el && !dynamic_cast<GREmpty *>(el))
				{
					GCoord ypos = el->getPosition().y;
					if (el->getGRStaff() && tagtype == GRTag::SYSTEMTAG)
						ypos += el->getGRStaff()->getPosition().y;

					if (mLowestY > ypos)
					{
						mLowestY = ypos;
						lowerNote = (GRSingleNote *)el;
					}
					if (mHighestY < ypos)
					{
						mHighestY = ypos;
						higherNote = (GRSingleNote *)el;
					}
				}
			}
		}
	}
	
    if (dispdur >= DURATION_1)
	{
		stemdir = dirOFF;
	}
	
	theStem->setStemDir(stemdir);
	

	// otherwise it has been set because of auto-stem.
	if (!highestlowestset)
	{
		// determine the lowest and highest position ...
		el = associated->GetTail();
		if (el)
		{
			mLowestY = el->getPosition().y;
			if (tagtype == GRTag::SYSTEMTAG && el->getGRStaff())
			{
				mLowestY += el->getGRStaff()->getPosition().y;
			}
			mHighestY = mLowestY;
		}
		
		GuidoPos pos = associated->GetHeadPosition();
		while (pos)
		{
			
			GRNotationElement * el = associated->GetNext(pos);
			if (el && !dynamic_cast<GREmpty *>(el))
			{
				NVPoint elpos (el->getPosition());
				if (tagtype == GRTag::SYSTEMTAG && el->getGRStaff())
				{
					elpos += el->getGRStaff()->getPosition();

				}
				
				if (mLowestY > elpos.y) 	mLowestY = elpos.y;
				if (mHighestY < elpos.y)	mHighestY = elpos.y;
			}
		}
		
	}

	// now we have the position of the lowest or highest note ...
	if (stemdir == dirUP)
	{
		theStem->setPosition(NVPoint(0, (GCoord)mHighestY));
	}
	else if (stemdir == dirDOWN)
	{
		theStem->setPosition(NVPoint(0, (GCoord)mLowestY));
	}

	// now we have to deal with the length...

	const TagParameterFloat * taglength = 0;
	if (stemstate)
		taglength = stemstate->getLength();
	if (stemstate && taglength  && taglength->TagIsSet())
	{
		// we have a length, that was definitly set...
		theStem->setStemLength((float)(stemstate->getLength()->getValue()));
		stemlengthset = true;
	}
	else
	{
		// length was not set ....
		float length = (float)(mHighestY - mLowestY + inStaff->getStaffLSPACE() * 3.5f * mTagSize);
		theStem->setStemLength( length );
	}

	delete theFlag;

	// here we have to add the flags ...
	theFlag = new GRFlag(this, dispdur,stemdir,theStem->getStemLength());
	if (mColRef)
		theFlag->setColRef(mColRef);

	if (!mFlagOnOff)
		theFlag->setFlagOnOff(mFlagOnOff);

	if (stemdir == dirUP)
	{
		theFlag->setPosition(NVPoint(0, (GCoord)mHighestY));
	}
	else if (stemdir == dirDOWN)
	{
		theFlag->setPosition(NVPoint(0, (GCoord)mLowestY));
	}

	if (tagtype != GRTag::SYSTEMTAG)
		updateGlobalStem(inStaff);

	const float curLSPACEtmp = (float)(inStaff->getStaffLSPACE());

	if (stemdir == dirUP)
	{
		NVPoint stemendpos (theStem->getPosition());
		stemendpos.y -= theStem->getStemLength();

		if (stemendpos.y > 2 * curLSPACEtmp)
		{
			const float newlength = (theStem->getPosition().y - 2 * curLSPACEtmp);
			changeStemLength(newlength);
		}
	}
	else if (stemdir == dirDOWN)
	{
		NVPoint stemendpos (theStem->getPosition());
		stemendpos.y += theStem->getStemLength();

		if (stemendpos.y < 2 * curLSPACEtmp)
		{
			const float newlength = (2 * curLSPACEtmp - theStem->getPosition().y);
			changeStemLength(newlength) ;
		}
	}
}


void GRGlobalStem::updateGlobalStem(const GRStaff * inStaff)
{
	// now we can adjust the headoffsets for the notes ...
	// maybe we should sort the mAssociated list by y-position.
	// then we can travers the noteheads an make the
	// adjustion (semi-)automatic.
	mAssociated->sort( &compnotposy );

	const float curLSPACE = inStaff->getStaffLSPACE();

	ARTHead::HEADSTATE sugHeadState;
	ARTHead::HEADSTATE prevHeadState = ARTHead::NOTSET;
	float prevposy = 0;	// (JB) warning, was not initialized. I don't know if 0
						// is a good init value ! TODO: verify.

	// the first element is in most cases the empty-event!?
	GRSingleNote * note = dynamic_cast<GRSingleNote *>(mFirstEl);
	if (note)
	{
		note->adjustHeadPosition(ARTHead::NORMAL);
		note->updateBoundingBox();
	}

	if (stemdir == dirDOWN)
	{
		sugHeadState = ARTHead::RIGHT;
		GuidoPos pos = mAssociated->GetHeadPosition();
		while (pos)
		{
			note = dynamic_cast<GRSingleNote *>(mAssociated->GetNext(pos));
			if (note)
			{
				if (prevHeadState != ARTHead::NOTSET)
				{
					float cury = (float)note->getPosition().y;
					if (tagtype == GRTag::SYSTEMTAG)
						cury += (float)note->getGRStaff()->getPosition().y;

					// y-values are ascending.
					if (cury != prevposy && cury - prevposy < curLSPACE)
					{
						// then I have to reverse the headsuggestion.
						if (prevHeadState == ARTHead::RIGHT)
							sugHeadState = ARTHead::LEFT;
						else
							sugHeadState = ARTHead::RIGHT;
					}
					else if (cury - prevposy >= curLSPACE)
					{
						if (prevHeadState == ARTHead::LEFT)
							sugHeadState = ARTHead::RIGHT;
					}
				}
				ARTHead::HEADSTATE retHeadState = note->adjustHeadPosition(sugHeadState);
				// now we have a current headstate ....

				prevHeadState = retHeadState;
				prevposy = note->getPosition().y;
				if (tagtype == GRTag::SYSTEMTAG)
					prevposy += note->getGRStaff()->getPosition().y;

				note->updateBoundingBox();

				// - Adjust horizontal notehead position, for non-standard noteheads, when notes are close in a chord
				note->getNoteHead()->adjustPositionForChords(sugHeadState, stemdir);
			}
		}

		// - Adjust stem length if it's a cross/triangle notehead

		if (lowerNote)
		{
			ConstMusicalSymbolID lowerNoteSymbol = lowerNote->getNoteHead()->getSymbol();

			if (lowerNoteSymbol == kFullXHeadSymbol)
			{
				lowerNote->setFirstSegmentDrawingState(false);
				lowerNote->setStemOffsetStartPosition(-4);
			}
			else if (lowerNoteSymbol == kFullTriangleHeadSymbol || lowerNoteSymbol == kHalfTriangleHeadSymbol)
				lowerNote->setFirstSegmentDrawingState(false);
			else if (lowerNoteSymbol == kFullReversedTriangleHeadSymbol || lowerNoteSymbol == kHalfReversedTriangleHeadSymbol)
				higherNote->setStemOffsetStartPosition(-47);
		}
	}
	else if (stemdir == dirUP || stemdir == dirOFF)
	{
		sugHeadState = ARTHead::LEFT;
		GuidoPos pos = mAssociated->GetTailPosition();
		while (pos)
		{
			note = dynamic_cast<GRSingleNote *>(mAssociated->GetPrev(pos));
			if (note)
			{
				if (prevHeadState != ARTHead::NOTSET)
				{
					float cury = (float)note->getPosition().y;
					if (tagtype == GRTag::SYSTEMTAG)
						cury += note->getGRStaff()->getPosition().y;

					// y-values are decending.
					if (cury != prevposy && prevposy - cury < curLSPACE)
					{
						// then I have to reverse the headsuggestion.
						if (prevHeadState == ARTHead::RIGHT)
							sugHeadState = ARTHead::LEFT;
						else
							sugHeadState = ARTHead::RIGHT;
					}
					else if (prevposy - cury  >= curLSPACE)
					{
						if (prevHeadState == ARTHead::RIGHT)
							sugHeadState = ARTHead::LEFT;
					}

				}
				ARTHead::HEADSTATE retHeadState = note->adjustHeadPosition(sugHeadState);

				prevHeadState = retHeadState;
				prevposy = note->getPosition().y;
				if (tagtype == GRTag::SYSTEMTAG)
					prevposy += note->getGRStaff()->getPosition().y;
				
			 	note->updateBoundingBox();

				// - Adjust horizontal notehead position, for non-standard noteheads, when notes are close in a chord
				note->getNoteHead()->adjustPositionForChords(sugHeadState, stemdir);
			}
		}


		// - Adjust stem length if it's a cross/triangle notehead

		if (higherNote)
		{
			ConstMusicalSymbolID higherNoteSymbol = higherNote->getNoteHead()->getSymbol();

			if (higherNoteSymbol == kFullXHeadSymbol)
			{
				higherNote->setFirstSegmentDrawingState(false);
				higherNote->setStemOffsetStartPosition(4);
			}
			else if (higherNoteSymbol == kFullTriangleHeadSymbol || higherNoteSymbol == kHalfTriangleHeadSymbol)
				higherNote->setStemOffsetStartPosition(47);
			else if (higherNoteSymbol == kFullReversedTriangleHeadSymbol || higherNoteSymbol == kHalfReversedTriangleHeadSymbol)
				higherNote->setFirstSegmentDrawingState(false);
		}
	}
	else 
	{
		assert(false);
	}
}

void GRGlobalStem::setHPosition( float nx )
{
	if (error)	return; 

	if (tagtype == GRTag::SYSTEMTAG && !mIsSystemCall)
		return;

	// the first tells the element itself of its new position
	// and also the associated elements ... 
	GRPTagARNotationElement::setHPosition(nx + getOffset().x);
	
	if (theStem)	theStem->setHPosition(nx);
	if (theFlag)	theFlag->setHPosition(nx);
}

void GRGlobalStem::OnDraw( VGDevice & hdc) const
{
	if (error) return;
	if (theStem)	theStem->OnDraw(hdc);
	if (theFlag)	                theFlag->OnDraw(hdc);
}

float GRGlobalStem::changeStemLength( float inLen )
{
	if (stemlengthset)
	{
		GuidoWarn("Stemlength already set!");
		if (theStem)
			return theStem->getStemLength();
	}
	else if (theStem)
	{
		theStem->setStemLength( inLen );
		return theStem->getStemLength();
	}
	return 0;
}

void GRGlobalStem::tellPosition(GObject * obj, const NVPoint & pt)
{
	if (error)	 return;

	if (dynamic_cast<GRNotationElement *>(obj) == mFirstEl) // useless cast ?
	{
		if (mIsSystemCall)
		{
			// this is the staff, to which the stem belongs ....

			const GRStaff * stemstaff = mFirstEl->getGRStaff();
			// update the position of the stem and of the flag ....
			
			// determine the lowest and highest position ...

			GRNotationElement * el = mAssociated->GetTail();
			NVPoint offset;
			if (el)
			{
				offset = el->getGRStaff()->getPosition();
				mLowestY = el->getPosition().y + offset.y;
				mHighestY = mLowestY;
			}
			else
			{
				mLowestY  = 0;
				mHighestY = 0;
			}
			
			GuidoPos pos = mAssociated->GetHeadPosition();
			while (pos)
			{
				GRNotationElement * el = mAssociated->GetNext(pos);
				if (el && !dynamic_cast<GREmpty *>(el))
				{
					offset = el->getGRStaff()->getPosition();
					if (mLowestY > el->getPosition().y + offset.y)
						mLowestY = el->getPosition().y + offset.y;
					if (mHighestY < el->getPosition().y + offset.y)
						mHighestY = el->getPosition().y + offset.y;
				}
			}
			
			
			offset = stemstaff->getPosition();
			mLowestY -= offset.y;
			mHighestY -= offset.y;
	
			GDirection stemdir = theStem->getStemDir();
			// now we have the position of the lowest or highest note ...
			if (stemdir == dirUP)
			{
				theStem->setPosition(NVPoint(0, (GCoord)mHighestY));
			}
			else if (stemdir == dirDOWN)
			{
				theStem->setPosition(NVPoint(0, (GCoord)mLowestY));
			}
			
			// now we have to deal with the length ...
			
			if (stemstate && stemstate->getLength()->TagIsSet())
			{
				// we have a length, that was definitly set ....
				theStem->setStemLength( stemstate->getLength()->getValue());
				stemlengthset = true;
			}
			else
			{
				// length was not set ....
				const float theLength = (float)(mHighestY - mLowestY + stemstaff->getStaffLSPACE() * 3.5f);
				theStem->setStemLength(theLength);
			}
			
			// here we have to add the flags ...
			// theFlag = new GRFlag(this,
			//	dispdur,stemdir,theStem->getStemLength());
			
			if (stemdir == dirUP)
			{
				theFlag->setPosition(NVPoint(0, (GCoord)mHighestY));
			}
			else if (stemdir == dirDOWN)
			{
				theFlag->setPosition(NVPoint(0, (GCoord)mLowestY));
			}


			GRNotationElement * tmpel = mAssociated->GetHead();
			if (tmpel)
				updateGlobalStem(tmpel->getGRStaff());

		}
		setHPosition(pt.x);
	}
}

/** \brief Called when the linked GRSystemTag gets a position-update
*/
void GRGlobalStem::checkPosition(const GRSystem * grsys)
{
	if (error) return;

//	GRSystemStartEndStruct * sse = getSystemStartEndStruct(grsys);
//	assert(sse);

	mIsSystemCall = true;
	tellPosition( mFirstEl, mFirstEl->getPosition());
	mIsSystemCall = false;
}

int GRGlobalStem::getNumFaehnchen() const
{
	if (theFlag)
	{
		return theFlag->getNumFaehnchen();
	}
	// construct a temporary flag to get the
	// num faehnchen that would be present
	// if there were a flag yet.
	// the problem is, that the flag is constructed
	// at the very end of the \shareStem-Range
	// and getNumFaehnchen() is called right after
	// the event was created ....

	GRFlag tmpflag (dispdur);
	return tmpflag.getNumFaehnchen();
}

void GRGlobalStem::setFlagOnOff(bool i)
{
	mFlagOnOff = i;
	if (theFlag)
		theFlag->setFlagOnOff(i);
}

void GRGlobalStem::setStemDirection(GDirection dir)
{
	const GDirection olddir = stemdir;
	stemdir = dir;
	if (theStem)
	{
		theStem->setStemDir(dir);
		if (olddir != stemdir)
		{
			const float length = theStem->getStemLength();
			float diffy = (float)(mHighestY - mLowestY);
			theStem->setStemLength(length + diffy);
			NVPoint myposition = theStem->getPosition();
			if (stemdir == dirUP)
			{
				// is now upwards:
				myposition.y += diffy;
			}
			else if (stemdir == dirDOWN)
			{
				myposition.y -= diffy;
			}
			theStem->setPosition(myposition);
		}
	}
}

NVPoint GRGlobalStem::getStemStartPos() const
{ 
	const GRStem * stem = theStem;
	NVPoint pnt( mPosition );
	GDirection stemdir;
	float stemlength;
	float notebreite = 60;
	if (stem)
	{
		pnt = stem->getPosition();
		stemdir = theStem->getStemDir();
		stemlength = theStem->getStemLength();
		if (stemdir == dirUP)
		{
			pnt.x += (GCoord)(notebreite * 0.5f * mTagSize + mTagOffset.x - (4 * mTagSize));
			pnt.y -= (GCoord)(stemlength - mTagOffset.y);
		}
		else if (stemdir == dirDOWN)
		{		
			pnt.x -= (GCoord)(notebreite * 0.5f * mTagSize) - mTagOffset.x;
			pnt.y += (GCoord)(stemlength +  mTagOffset.y);
		}
	}
	return pnt;

}

NVPoint GRGlobalStem::getStemEndPos() const
{
	const GRStem * stem = theStem;
	GDirection stemdir;
	float notebreite = 60;
	float stemlength;
	NVPoint pnt( mPosition );
	if (stem)
	{
		pnt = stem->getPosition();
		stemdir = theStem->getStemDir();
		stemlength = theStem->getStemLength();
		if (stemdir == dirUP)
		{
			pnt.x += (GCoord)((notebreite * 0.5f * mTagSize) + mTagOffset.x - (1 * mTagSize));
			pnt.y -= (GCoord)(stemlength - mTagOffset.y);
		}
		else if (stemdir == dirDOWN)
		{		
			pnt.x -= (GCoord)((notebreite * 0.5f * mTagSize) - mTagOffset.x - (4 * mTagSize));
			pnt.y += (GCoord)(stemlength + mTagOffset.y);
		}
	}
	return pnt;
}

float GRGlobalStem::getStemLength() const
{
	return theStem ? theStem->getStemLength() : 0;
}


/** \brief (not implemented) This sets the stemlength from the perspective
	 of a note ....

 	what needs to be done: I have to see what direction I am in and
	I then have to set the position of the event in relation to the
	 position of the stem.
*/
void GRGlobalStem::setNoteStemLength( GREvent * ev, float inLen )
{
}

void GRGlobalStem::setSize( float newsize )
{
	mTagSize = newsize;
}

void GRGlobalStem::setMultiplicatedSize(float newMultiplicatedSize)
{
    mTagSize *= newMultiplicatedSize;
}

void GRGlobalStem::setOffsetXY(float inOffsetX, float inOffsetY)
{
    mTagOffset.x += inOffsetX;
    mTagOffset.y += inOffsetY;
}

/** \brief Returns the highest and lowest notehead
	(with respect to y-position.
	this is needed for slurs from and to chords.
*/
int GRGlobalStem::getHighestAndLowestNoteHead(GRStdNoteHead ** highest,
											  GRStdNoteHead ** lowest) const
{
	*highest = *lowest = NULL;
	if (mAssociated == 0 )	return 0;

	GuidoPos pos = mAssociated->GetHeadPosition();
	while (pos)
	{
		GRSingleNote * sn = dynamic_cast<GRSingleNote *>(mAssociated->GetNext(pos));
		if (sn)
		{
			GRStdNoteHead * tmp = sn->getNoteHead();
			if (tmp)
			{
				if (*highest && *lowest)
				{
					if ((*highest)->getPosition().y > tmp->getPosition().y)
					{
						*highest = tmp;
					}
					if ((*lowest)->getPosition().y < tmp->getPosition().y)
					{
						*lowest = tmp;
					}
				}
				else 
				{
					*highest = tmp;
					*lowest = tmp;
				}
			}
		}
	}
	if (*highest != NULL)
		return 1;
	
	return 0;	// is it a stem dir ?
}
