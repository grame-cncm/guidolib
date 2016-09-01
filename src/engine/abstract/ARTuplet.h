#ifndef ARTuplet_H
#define ARTuplet_H

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

#include "ARMTParameter.h"
#include "ARPositionTag.h"
#include "TagParameterFloat.h"
#include "TagParameterString.h"
#include <cstring>

class ARBase;

/** \brief Abstract representation of a tuplet.
*/
class ARTuplet : 
	// public ARMusicalObject,
	public ARMTParameter,
	public ARPositionTag
{
public:
                    ARTuplet(); 
    virtual 	   ~ARTuplet() {}

            void    setAuto();
    const NVstring& getName() const { return fTupletFormat; }

    virtual void  setTagParameterList(TagParameterList & tl);

    virtual void  printName(std::ostream& os) const;
    virtual void  printGMNName(std::ostream& os) const;
    virtual void  printParameters(std::ostream& os) const;

            void  setupTuplet( ARBase * inBase );
            void  parseTupletFormatString();

            int	  getNumerator()    const { return fBaseNumerator; }
            int   getDenominator()  const { return fBaseDenominator; }

			/*!
			 * \brief isPositionAbove
			 * \return 0 if position is not explicitely set in the tag or -1 if set below or 1 if set above.
			 */
			int  isPositionAbove() const;
            float getDy1()          const { return fDy1; }
            float getDy2()          const { return (fDy2TagIsSet ? fDy2 : fDy1); }		
            float getThickness()    const { return fLineThickness; }
            bool  isTextBold()      const { return fTextBold; }
            float getTextSize()     const { return fTextSize; }
            bool  getLeftBrace()    const { return fLeftBrace; }
            bool  getRightBrace()   const { return fRightBrace; }
            NVstring getDispNote()  const { return fDispNote; }

            bool  isFormatSet()     const { return fFormatSet; } 
            bool  isDySet()         const { return (fDy1TagIsSet || fDy2TagIsSet); }

protected:
    NVstring fTupletFormat;	// format string <"-x:y-">
    NVstring fPosition;
	bool	 fPositionIsSet;
    float    fDy1;
    float    fDy2;
    float    fLineThickness;
    bool     fTextBold;
    float    fTextSize;
    NVstring fDispNote;

    int	  fBaseNumerator;
    int	  fBaseDenominator;
    bool  fLeftBrace;
    bool  fRightBrace;

    bool  fFormatSet;
    bool  fDy1TagIsSet;
    bool  fDy2TagIsSet;

    static ListOfTPLs ltpls;
    // a string ...
};

#endif
