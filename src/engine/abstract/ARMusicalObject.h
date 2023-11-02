#ifndef ARMusicalObject_H
#define ARMusicalObject_H

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

#include "defines.h"	// for TYPE_TIMEPOSITION
#include "ARVisitable.h"

class GObject;
class TimeUnwrap;
class AROctava;
class ARGrace;
class ARNote;
class ARSpace;
class ARTab;
class ARTempo;

/** \brief The base class for all AR objects. 
	It contains all musical information : duration and time position.
*/
class ARMusicalObject : public ARVisitable
{
  public:
				 ARMusicalObject();
				 ARMusicalObject( const TYPE_TIMEPOSITION & relativeTimepositionOfMusicalObject );
				 ARMusicalObject( const ARMusicalObject & armo);
		virtual ~ARMusicalObject() {}

		virtual ARMusicalObject * Copy() const;

		// should be re-designed!!! Should be put to another place but is needed here for spacing alg.
		// returns here 0, simple objects have no durations (Clefs, Bars, ...)
		virtual const TYPE_DURATION & getDuration() const						{ return fDuration; }
		virtual const TYPE_TIMEPOSITION & getRelativeTimePosition() const		{ return relativeTimePosition; }

		// must be re-designed!!! Should be put to another place  but is needed here from several alg.
		virtual TYPE_TIMEPOSITION getRelativeEndTimePosition() const;
		
		virtual void setRelativeTimePosition(const TYPE_TIMEPOSITION  & newRelativeTimePosition);
		virtual void setRelativeEndTimePosition(const TYPE_TIMEPOSITION & tp)	{ fDuration = tp - relativeTimePosition; }
		virtual void setDuration(const TYPE_DURATION & dur)						{ fDuration = dur; }

		virtual bool	isEventClass() const						{ return false; }
		virtual void	browse(TimeUnwrap& mapper) const			{}
		virtual int		getVoiceNum() const							{ return fVoiceNum; }
		virtual void	printGMNName(std::ostream& os) const		{}
		virtual std::string	getGMNName() const						{ return "\\musicalObject"; }
		virtual void	setVoiceNum(int num)						{ fVoiceNum = num; }

	static	bool	IsPowerOfTwoDenom(const TYPE_DURATION & dur);

	virtual void	setDrawGR(bool on)		{ fDrawGR = on;}
	virtual bool	getDrawGR() const		{ return fDrawGR;}

    /**** Functions to avoid dynamic_cast ****/
    virtual ARMusicalObject  *isARRepeatBegin()   { return NULL; }
    virtual ARNote			 *isARNote()          { return NULL; }
    virtual const ARNote	 *isARNote() const    { return NULL; }
    virtual ARTab			 *isARTab()           { return NULL; }
    virtual const ARTab	 	 *isARTab() const     { return NULL; }
    virtual ARMusicalObject  *isARCoda()          { return NULL; }
    virtual ARMusicalObject  *isARSegno()         { return NULL; }
    virtual ARMusicalObject  *isARNewPage()       { return NULL; }
    virtual ARMusicalObject  *isARNewSystem()     { return NULL; }
    virtual ARMusicalObject  *isARKey()           { return NULL; }
    virtual ARMusicalObject  *isARNaturalKey()    { return NULL; }
    virtual ARMusicalObject  *isARBeamState()     { return NULL; }
    virtual ARMusicalObject  *isARClef()          { return NULL; }
    virtual ARMusicalObject  *isARFinishBar()     { return NULL; }
    virtual ARMusicalObject  *isARMeter()         { return NULL; }
    virtual ARMusicalObject  *isARRepeatEnd()     { return NULL; }
    virtual ARMusicalObject  *isARRest()          { return NULL; }
    virtual ARMusicalObject  *isARSecondGlue()    { return NULL; }
    virtual ARMusicalObject  *isARPossibleBreak() { return NULL; }
    virtual AROctava		 *isAROctava()		  { return NULL; }
    virtual ARGrace			 *isARGrace()		  { return NULL; }
    virtual ARSpace			 *isARSpace()		  { return NULL; }
    virtual ARTempo			 *isARTempo()		  { return NULL; }

    virtual const ARMusicalObject  *isARDummyRangeEnd() const	{ return NULL; }
    virtual const ARMusicalObject  *isARNoteFormat() const		{ return NULL; }
    virtual const ARMusicalObject  *isARBar() const				{ return NULL; }
    virtual const ARMusicalObject  *isARStaff() const			{ return NULL; }
    virtual const ARMusicalObject  *isARMusicalTag() const		{ return NULL; }
    virtual const ARMusicalObject  *isARJump() const			{ return NULL; }
    virtual const ARMusicalObject  *isARChordComma() const		{ return NULL; }
    virtual const ARMusicalObject  *isARUserChordTag() const	{ return NULL; }
	virtual bool					isEmptyNote() const			{ return false; }

    /* Visitor design pattern */
    virtual void accept(ARVisitor& visitor);
	virtual void print(std::ostream & os) const;

  protected:
		TYPE_TIMEPOSITION	relativeTimePosition;
		int					fVoiceNum;		// voice number added for mapping info [DF - 10-11-09]
		bool	fDrawGR = true;				// a flag that reflects \staffOn/Off sequence

  private:
		TYPE_DURATION		fDuration;
};

std::ostream & operator<<(std::ostream& os, const ARMusicalObject* o);

#endif
