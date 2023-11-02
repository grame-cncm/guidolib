#ifndef ARNote_H
#define ARNote_H

/*
GUIDO Library
Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr*/

#include <string>

#include "ARAlter.h"
#include "ARNoteName.h"
#include "ARMusicalEvent.h"
#include "GuidoDefs.h"


class ARTrill;
class ARCluster;
class ARTremolo;

/** \brief Representation of a GUIDO note.
*/
class ARNote : public ARMusicalEvent
{
	public:
				 ARNote( const TYPE_DURATION & durationOfNote );
				 ARNote( const TYPE_TIMEPOSITION & relativeTimePositionOfNote, const TYPE_DURATION & durationOfNote );
				 ARNote( const std::string& name, int accidentals, int octave, int numerator, int denominator, int intensity );
				 ARNote( const ARNote & arnote, bool istied=false );
		virtual ~ARNote();

		virtual void operator=(const ARNote* note);

		virtual ARNote*	Clone(bool istied = false ) const;
		virtual int		CompareNameOctavePitch( const ARNote & nt );
		virtual void	setDuration( const TYPE_DURATION & newdur );
		virtual bool	CanBeMerged( const ARMusicalEvent * ev2 );

		virtual ARMusicalObject * Copy() const;

		virtual void print(std::ostream & os) const;
		virtual const char*	getParamsStr() const	{ return ""; };
		virtual const char*	getTagName() const		{ return "ARNote"; };
		virtual std::string getGMNName() const;

		virtual void browse(TimeUnwrap& mapper) const;

		void		 addFlat();
		void	 	 addSharp();

		const ARNoteName & getName() const					{ return fName; }
		int		     getOctave() const						{ return fOctave; }
		int	    	 getPitch() const						{ return fPitch; }
		int			 getOctava() const						{ return fOctava; }
	
		void		 setOctava(int octava)					{ fOctava = octava; }
		void		 setPitch( int newpitch );
		void		 setOctave( int newOctave )				{ fOctave = newOctave;}
		void		 offsetpitch( int steps );

		int		     getAccidentals() const					{ return fAccidentals; }
		float		 getDetune() const						{ return fAlter ? fAlter->getDetune() : 0; }
		const ARAlter * getAlter() const					{ return fAlter; }
		float		 getPitchShift() const					{ return getDetune() + fAccidentals; }
		const ARTrill*   getOrnament() const		    	{ return fOrnament;}
		ARCluster *	     getARCluster()						{ return fCluster;}
		const ARCluster* getARCluster() const				{ return fCluster;}

		bool         isLonelyInCluster() const              { return fIsLonelyInCluster; }
		bool         doesClusterHaveToBeDrawn() const       { return fClusterHaveToBeDrawn; }
		void         enableSubElements(bool enabled)        { fSubElementsHaveToBeDrawn = enabled; }
		bool         haveSubElementsToBeDrawn()  const      { return fSubElementsHaveToBeDrawn; }
		const ARTremolo* getTremolo()						{ return fTremolo; }
		NVstring     getAppearance() const                  { return fNoteAppearance; }
		int		     getMidiPitch() const;

		void 		 setTremolo(const ARTremolo* trem)      { fTremolo = trem; }
		void         setIsLonelyInCluster()					{ fIsLonelyInCluster = true; }
		void		 setAlter( const ARAlter * alter )		{ fAlter = alter; }
		void		 setOrnament(const ARTrill *newOrnament, bool trillOwner=true);
		void		 setAccidentals( int theAccidentals );
		ARCluster*	 setCluster(ARCluster *inCluster, bool inClusterHaveToBeDrawn = false,
													  bool inHaveToBeCreated = false);

		static int	detune2Quarters(float detune);
	
		// Used when dispNote param is set on \tuplet-tag to force note appearance
		void forceNoteAppearance(NVstring noteAppearance);

		/**** Function to avoid dynamic_cast ****/
		virtual ARNote*			isARNote()				{ return this; }
		virtual const ARNote*	isARNote() const		{ return this; }
		virtual bool			isEmptyNote() const		{ return getName() == "empty"; }
		bool		isAuto() const						{ return fAuto; }
		void		setAuto(bool val)					{ fAuto = val; }


	private:
		ARNoteName fName;

		int		   fPitch;
		int		   fOctave;
		int		   fAccidentals;
		int 	   fIntensity;
		const ARTrill   *fOrnament;
		ARCluster *fCluster;
		bool       fOwnCluster;
		bool       fIsLonelyInCluster;
		bool       fClusterHaveToBeDrawn;
		bool       fSubElementsHaveToBeDrawn;
		bool	   fAuto;			// a flag to discriminate notes created by the auto stuff (e.g. due to barlines)
		const ARTremolo * fTremolo 	= 0;
		const ARAlter *   fAlter	= 0;

		bool		fTrillOwner = false;
		NVstring   fNoteAppearance;
		int		   fOctava;			// for octava signs
};

#endif
