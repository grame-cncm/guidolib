#ifndef ARNote_H
#define ARNote_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

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

*/

#include <string>

#include "ARNoteName.h"
#include "ARMusicalEvent.h"

#include "GuidoDefs.h"


class ARTrill;

/** \brief Representation of a GUIDO note.
*/
class ARNote : public ARMusicalEvent
{
  public:
      			ARNote( int theNumerator, int theDenominator );
				ARNote( const TYPE_DURATION & durationOfNote );
				ARNote( const TYPE_TIMEPOSITION & relativeTimePositionOfNote, const TYPE_DURATION & durationOfNote );
				ARNote( const std::string& inName, int theAccidentals, int theRegister, int theNumerator, 
						int theDenominator, int theIntensity );
				ARNote( const ARNote & arnote );
		virtual ~ARNote();

		virtual int		CompareNameOctavePitch( const ARNote & nt );
		virtual void	setDuration( const TYPE_DURATION & newdur );
		virtual bool	CanBeMerged( const ARMusicalEvent * ev2 );

		virtual ARMusicalObject * Copy() const;

		virtual void print() const;
		virtual std::ostream & operator<<(std::ostream & os ) const;
		virtual void	browse(TimeUnwrap& mapper) const;

      void		addFlat();
      void		addSharp();
      void		setRegister( int newRegister );

      const ARNoteName & getName() const;

	  int		getOctave() const;
      int		getPitch() const;

	  void		setPitch( int newpitch );
	  void		setOctave( int newOctave )					{fOctave = newOctave;}
      void		offsetpitch( int steps );

      void		setAccidentals( int theAccidentals );
	  int		getAccidentals() const						{ return fAccidentals; }
      void		setDetune( float detune )					{ fDetune = detune; }
	  float		getDetune() const							{ return fDetune; }
	  float		getPitchShift() const						{ return fDetune + fAccidentals; }
	  void		setOrnament( ARTrill * newOrnament );
	  ARTrill *	getOrnament( )								{ return fOrnament;}
	  
	  int		midiPitch() const;

	static int	detune2Quarters(float detune);

	private:
		ARNoteName fName;

		int		fPitch;
		int		fOctave;
		int		fAccidentals;
		float	fDetune;
      	int 	fIntensity;
		ARTrill * fOrnament;
};

#endif


