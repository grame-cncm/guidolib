/*

  MusicXML Library
  Copyright (C) 2006,2007  Grame

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

#ifndef __midiContextVisitor__
#define __midiContextVisitor__

#include <map>

#include "arexport.h"
#include "guidoelement.h"
#include "AROthers.h"
#include "ARTypes.h"
#include "guidorational.h"
#include "visitor.h"

namespace guido 
{

/*!
\addtogroup visitors
@{
*/

/*!
\brief An abstract interface to be used with midicontextvisitor
*/
class midiwriter {
	public:
		enum { kNone, kSlur, kStaccato=kSlur*2 };

		virtual ~midiwriter() {}
		
		virtual void startVoice () = 0;
		virtual void endVoice (long date) = 0;

		virtual void newNote (long date, int pitch, int vel, int duration, int art) = 0;
		virtual void tempoChange (long date, int bpm) = 0;
		virtual void progChange (long date, int prog) = 0;
		virtual void timeSignChange (long date, unsigned int num, unsigned int denom) = 0;
		virtual void keySignChange (long date, int signature, bool major=true) = 0;
};


/*!
\brief A Visitor that maintains a context for MIDI generation.
*/

class gar_export midicontextvisitor :
	// time management
	public visitor<SARSlur>,
	public visitor<SARSlurBegin>,
	public visitor<SARSlurEnd>,
	public visitor<SARStacc>,
	public visitor<SARStaccBegin>,
	public visitor<SARStaccEnd>,
	public visitor<SARTempo>,
	public visitor<SARTie>,
	public visitor<SARTieBegin>,
	public visitor<SARTieEnd>,

	// score information
	public visitor<SARKey>,
	public visitor<SARMeter>,

	// nuances rendering
	public visitor<SARIntens>,
	
	// notes and voices
	public visitor<SARInstr>,
	public visitor<SARVoice>,
	public visitor<SARChord>,
	public visitor<SARGrace>,
	public visitor<SAROct>,
	public visitor<SARNote>
{   
    protected:
		typedef ctree<guidoelement>::iterator guidoIterator;
		typedef std::vector<SARNote> ARNotes;

    private:
		enum { kNoTie, kInTie, kTiedNote, kTiedChord };
		std::map<Sguidoelement,int>	fTiedMap;	// used to store tied notes in a single tie sequence
		guidoIterator	fEndTie;				// end iterator for tie container browsing
		SARVoice		fCurrentVoice;
        bool	fInChord, fInSlur, fInStaccato, fInGrace;
		int		fTieState;

		void	reset ();
		void	startTie (Sguidoelement tie, bool storeEnd = false);
		void	stopTie	();
		void	storeNotes	( SARChord& elt, ARNotes& dest );
		void	lookupTied	(guidoIterator start, guidoIterator end, const SARNote& note, ARNotes&);
		rational totalDuration ( const ARNotes& list ) const;
   
    protected:

		midiwriter*	fMidiWriter;
        long		fCurrentDate;		// current date
        rational	fCurrentDuration;	// current notes duration
        int			fChordDuration;		// current chord duration (available at chord exit)
		int			fCurrentOctave;		// current notes octave
		int			fCurrentVel;		// current notes velocity
		int			fCurrentDots;		// current notes dots
        int			fTranspose;			// current transpose value
		long		fTPQ;				// ticks-per-quater value for date conversion
		
		int  moveTime (int dur);
		bool equalPitch (const SARNote&, const SARNote&) const;
		rational noteduration (const SARNote& elt, rational& currentDuration, int& currentDots ) const;	// computes a note duration
		int  rational2ticks (const rational& dur) const;		// converts guido rational time into ticks
		int  midiPitch (const SARNote& elt) const;
 
 		virtual void playMidiInstrument (int progChange);
 		virtual void playNote (long date, int pitch, int duration);
 		virtual void playTempoChange (long bmp);
 		virtual void playProgChange (long prog);
 		virtual void playMeterChange (unsigned int num, unsigned int denum);
 		virtual void playKeySignChange (int sign, bool major=true);

		virtual void visitStart( SARChord& elt );
		virtual void visitEnd  ( SARChord& elt );
		virtual void visitStart( SARVoice& elt );
		virtual void visitEnd  ( SARVoice& elt );
		virtual void visitStart( SARNote& elt );
		virtual void visitStart( SARInstr& elt );
		virtual void visitStart( SAROct& elt );
		virtual void visitEnd  ( SAROct& elt );
		virtual void visitStart( SARGrace& )		{ fInGrace = true; }
		virtual void visitEnd  ( SARGrace& )		{ fInGrace = false; }

	// time management
		virtual void visitStart( SARTempo& elt );

		virtual void visitStart( SARSlur& )			{ fInSlur = true; }
		virtual void visitEnd  ( SARSlur& )			{ fInSlur = false; }
		virtual void visitStart( SARSlurBegin& )	{ fInSlur = true; }
		virtual void visitStart( SARSlurEnd&  )		{ fInSlur = false; }

		virtual void visitStart( SARStacc& )		{ fInStaccato = true; }
		virtual void visitEnd  ( SARStacc& )		{ fInStaccato = false; }
		virtual void visitStart( SARStaccBegin& )	{ fInStaccato = true; }
		virtual void visitStart( SARStaccEnd& )		{ fInStaccato = false; }

		virtual void visitStart( SARTie& elt );
		virtual void visitEnd  ( SARTie& elt );
		virtual void visitStart( SARTieBegin& elt );
		virtual void visitStart( SARTieEnd& elt );

	// nuances rendering
		virtual void visitStart( SARIntens& elt );

	// score information
		virtual void visitStart( SARKey& elt );
		virtual void visitStart( SARMeter& elt );

    public:  
				 midicontextvisitor(long tpq, midiwriter* writer=0);
       	virtual ~midicontextvisitor();
		
		virtual void visit(Sguidoelement& elt);
};

/*! @} */

}

#endif
