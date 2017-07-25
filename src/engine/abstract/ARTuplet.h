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

#include <string>

#include "ARMTParameter.h"
#include "ARPositionTag.h"
#include "TagParameterFloat.h"

class ARBase;
class TagParameterString;

#define kDefaultThickness	4.f
/** \brief Abstract representation of a tuplet.
*/
class ARTuplet :  public ARMTParameter, public ARPositionTag
{
	public:
						ARTuplet(); 
		virtual 	   ~ARTuplet() {}

				void		setAuto();
		const std::string	getName() const { return fTupletFormat; }

		virtual void  setTagParameters (const TagParameterMap& params);

		virtual const char*	getParamsStr() const	{ return kARTupletParams; };
		virtual const char*	getTagName() const		{ return "ARTuplet"; };
		virtual std::string getGMNName() const		{ return "\\tuplet"; };
 
            void  setupTuplet( ARBase * inBase );
            void  parseTupletFormatString();

            int	  getNumerator()    const { return fBaseNumerator; }
            int   getDenominator()  const { return fBaseDenominator; }

			/*!
			 * \brief isPositionAbove
			 * \return 0 if position is not explicitely set in the tag or -1 if set below or 1 if set above.
			 */
			int  isPositionAbove() const;
            float getDy1()          const { return fDy1 ? fDy1->getValue() : 0.f; }
            float getDy2()          const { return fDy2 ? fDy2->getValue() : 0.f; }
            float getThickness()    const { return fLineThickness; }
            float defaultThickness()const { return kDefaultThickness; }
            bool  isTextBold()      const { return fTextBold; }
            float getTextSize()     const { return fTextSize; }
            bool  getLeftBrace()    const { return fLeftBrace; }
            bool  getRightBrace()   const { return fRightBrace; }
            const std::string& getDispNote()  const { return fDispNote; }
            bool  isDySet()         const { return (fDy1 && fDy1->TagIsSet()) || (fDy2 && fDy2->TagIsSet()); }

	protected:
		std::string fTupletFormat;	// format string <"-x:y-">
		const TagParameterString* fPosition;
		const TagParameterFloat* fDy1;
		const TagParameterFloat* fDy2;
		float		fLineThickness;
		bool		fTextBold;
		float		fTextSize;
		std::string fDispNote;

		int	  fBaseNumerator;
		int	  fBaseDenominator;
		bool  fLeftBrace;
		bool  fRightBrace;
};

#endif
