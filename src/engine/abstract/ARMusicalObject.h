#ifndef ARMusicalObject_H
#define ARMusicalObject_H

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

#include "defines.h"	// for TYPE_TIMEPOSITION

#include "PrintVisitor.h"
#include "Visitable.h"

#define MIN_TIMEPOSITION Frac_0
#define DURATION_7_4  Frac_7_4
#define DURATION_3_2  Frac_3_2
#define DURATION_1	 Frac_1
#define DURATION_7_8  Frac_7_8
#define DURATION_3_4  Frac_3_4
#define DURATION_2    Frac_1_2
#define DURATION_7_16 Frac_7_16
#define DURATION_3_8  Frac_3_8
#define DURATION_4    Frac_1_4
#define DURATION_7_32 Frac_7_32
#define DURATION_3_16 Frac_3_16
#define DURATION_8    Frac_1_8
#define DURATION_7_64 Frac_7_64
#define DURATION_3_32 Frac_3_32
#define DURATION_3_64 Frac_3_64
#define DURATION_3_128 Frac_3_128
#define DURATION_7_128 Frac_7_128
#define DURATION_7_256 Frac_7_256
#define DURATION_1_12 Frac_1_12
#define DURATION_16   Frac_1_16
#define DURATION_32   Frac_1_32
#define DURATION_64	  Frac_1_64
#define DURATION_128  Frac_1_128
#define DURATION_0    Frac_0
#define DURATION_n1   Frac_n1
#define MIN_DURATION DURATION_0
#define MAX_DURATION Frac_Max


class GObject;
class TimeUnwrap;

/** \brief The base class for all AR objects. 
	It contains all musical information : duration and time position.
*/
class ARMusicalObject : public Visitable
{
  public:
				 ARMusicalObject();
				 ARMusicalObject( const TYPE_TIMEPOSITION & relativeTimepositionOfMusicalObject );
				 ARMusicalObject( const ARMusicalObject & armo);
		virtual ~ARMusicalObject();

		virtual ARMusicalObject * Copy() const;
		virtual void removeGRRepresentation(GObject * p_grep);

		// should be re-designed!!! Should be put to another place but is needed here for spacing alg.
		// returns here 0, simple objects have no durations (Clefs, Bars, ...)
		virtual const TYPE_DURATION & getDuration() const						{ return fDuration; }
		virtual const TYPE_TIMEPOSITION & getRelativeTimePosition() const		{ return relativeTimePosition; }

		// must be re-designed!!! Should be put to another place  but is needed here from several alg.
		virtual TYPE_TIMEPOSITION getRelativeEndTimePosition() const;

		virtual std::ostream & operator<<(std::ostream& os) const;

	    
		// introduced to get correct tie pos for notes in chords [DF 2012-03-19]
		// do nothing at this level
		virtual void setStartTimePosition(const TYPE_TIMEPOSITION  & pos)	{}
		
		virtual void setRelativeTimePosition(const TYPE_TIMEPOSITION  & newRelativeTimePosition);
		virtual void setRelativeEndTimePosition(const TYPE_TIMEPOSITION & tp)	{ fDuration = tp - relativeTimePosition; }
		virtual void setDuration(const TYPE_DURATION & dur)						{ fDuration = dur; }

		virtual GObject * getFirstGRRepresentation();
		virtual GObject * getLastGRRepresentation();

		virtual void *	getGRRepresentation()						{ return mGrObject; }
		virtual void	addGRRepresentation(GObject * p_grep);

		virtual void	resetGRRepresentation();
		virtual bool	isEventClass() const						{ return false; }

		virtual void	browse(TimeUnwrap& mapper) const			{}
		virtual void	setVoiceNum(int num)						{ fVoiceNum = num; }
		virtual int		getVoiceNum() const							{ return fVoiceNum; }

	static	bool	IsPowerOfTwoDenom(const TYPE_DURATION & dur);

	virtual void	setDrawGR(bool onoff){drawGR = onoff;}
	virtual bool	getDrawGR(){return drawGR;}

    virtual void print(std::ostream& os) const;

    /**** Functions to avoid dynamic_cast ****/
    virtual ARMusicalObject  *isARRepeatBegin()   { return NULL; }
    virtual ARMusicalObject  *isARMusicalTag()    { return NULL; }
    virtual ARMusicalObject  *isARNote()          { return NULL; }
    virtual ARMusicalObject  *isARBar()           { return NULL; }
    virtual ARMusicalObject  *isARCoda()          { return NULL; }
    virtual ARMusicalObject  *isARSegno()         { return NULL; }
    virtual ARMusicalObject  *isARNewPage()       { return NULL; }
    virtual ARMusicalObject  *isARNewSystem()     { return NULL; }
    virtual ARMusicalObject  *isARStaff()         { return NULL; }
    virtual ARMusicalObject  *isARKey()           { return NULL; }
    virtual ARMusicalObject  *isARBeamState()     { return NULL; }
    virtual ARMusicalObject  *isARChordComma()    { return NULL; }
    virtual ARMusicalObject  *isARClef()          { return NULL; }
    virtual ARMusicalObject  *isARDummyRangeEnd() { return NULL; }
    virtual ARMusicalObject  *isARFinishBar()     { return NULL; }
    virtual ARMusicalObject  *isARMeter()         { return NULL; }
    virtual ARMusicalObject  *isARNoteFormat()    { return NULL; }
    virtual ARMusicalObject  *isARRepeatEnd()     { return NULL; }
    virtual ARMusicalObject  *isARRest()          { return NULL; }
    virtual ARMusicalObject  *isARSecondGlue()    { return NULL; }
    virtual ARMusicalObject  *isARUserChordTag()  { return NULL; }
    virtual ARMusicalObject  *isARJump()          { return NULL; }
    virtual ARMusicalObject  *isARPossibleBreak() { return NULL; }
    /*****************************************/

    /* Visitor design pattern */
    virtual void accept(BaseVisitor *visitor) { visitor->visit(*this); }

  protected:
		TYPE_TIMEPOSITION	relativeTimePosition;
		int					fVoiceNum;		// voice number added for mapping info [DF - 10-11-09]
		
		bool	drawGR;

		void * mGrObject;  	// ptr to belonging graphical object or GRMultipleGRObject
							// There can be multiple graphical objects linked to a single object
							// (JB) currently, it's always a GRMultipleGRObject *

  private:
		TYPE_DURATION		fDuration;
};

#endif
