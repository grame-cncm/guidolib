#ifndef ARNote_H
#define ARNote_H

/*
GUIDO Library
Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr*/

#include <string>

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
    ARNote( const std::string& inName, int theAccidentals, int theRegister, int theNumerator, 
        int theDenominator, int theIntensity );
    ARNote( const ARNote & arnote );
    virtual ~ARNote();

    virtual int		CompareNameOctavePitch( const ARNote & nt );
    virtual void	setDuration( const TYPE_DURATION & newdur );
    virtual bool	CanBeMerged( const ARMusicalEvent * ev2 );

    virtual ARMusicalObject * Copy() const;

	virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
	virtual void printParameters(std::ostream& os) const;

    virtual void browse(TimeUnwrap& mapper) const;

    // start time position has been introduced to get correct time position for notes in chords [DF 2012-03-19]
    virtual void						setStartTimePosition(const TYPE_TIMEPOSITION  & pos)	{ fStartPosition = pos; }
    virtual const TYPE_TIMEPOSITION&	getStartTimePosition() const;

    void		 addFlat();
    void	 	 addSharp();
    void	 	 setRegister( int newRegister );

    const ARNoteName & getName() const;

    int		     getOctave() const;
    int	    	 getPitch() const;

    void		 setPitch( int newpitch );
    void		 setOctave( int newOctave )				{fOctave = newOctave;}
    void		 offsetpitch( int steps );

    void		 setAccidentals( int theAccidentals );
    int		     getAccidentals() const					{ return fAccidentals; }
    void		 setDetune( float detune )				{ fDetune = detune; }
    float		 getDetune() const						{ return fDetune; }
    float		 getPitchShift() const					{ return fDetune + fAccidentals; }
    void		 setOrnament(ARTrill *newOrnament);
    ARCluster   *setCluster(ARCluster *inCluster, bool inClusterHaveToBeDrawn = false,
                                                  bool inHaveToBeCreated = false);
    ARTrill     *getOrnament()		    				{ return fOrnament;}
    ARCluster   *getARCluster() 			           	{ return fCluster;}

    void         setIsLonelyInCluster()                 { fIsLonelyInCluster = true; }
    bool         isLonelyInCluster()                    { return fIsLonelyInCluster; }
    bool         doesClusterHaveToBeDrawn()             { return fClusterHaveToBeDrawn; }
    void         enableSubElements(bool enabled)        { fSubElementsHaveToBeDrawn = enabled; }
    bool         haveSubElementsToBeDrawn()  const      { return fSubElementsHaveToBeDrawn; }
	void 		 setTremolo(ARTremolo* trem)            { fTremolo = trem; }
    ARTremolo*   getTremolo()                           { return fTremolo; }
    NVstring     getNoteAppearance()                    { return fNoteAppearance; }

    int		     getMidiPitch() const;

    static int	detune2Quarters(float detune);
    
    // Used when dispNote param is set on \tuplet-tag to force note appearance
    void forceNoteAppearance(NVstring noteAppearance);

    /**** Function to avoid dynamic_cast ****/
    virtual ARMusicalObject *isARNote() { return this; }
    /*****************************************/

  private:
    ARNoteName fName;

    int		   fPitch;
    int		   fOctave;
    int		   fAccidentals;
    float	   fDetune;
    int 	   fIntensity;
    ARTrill   *fOrnament;
    ARCluster *fCluster;
    bool       fOwnCluster;
    bool       fIsLonelyInCluster;
    bool       fClusterHaveToBeDrawn;
    bool       fSubElementsHaveToBeDrawn;
    ARTremolo *fTremolo;

    TYPE_TIMEPOSITION	fStartPosition;

    NVstring   fNoteAppearance;
};

#endif
