#ifndef ARTStem_H
#define ARTStem_H

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

#include "ARMTParameter.h"
#include "ARPositionTag.h"

class TagParameterFloat;

/** \brief not yet documented
*/

class ARTStem : public ARMTParameter, public ARPositionTag
{
	public:

		enum STEMSTATE { UP ,DOWN ,AUTO, OFF };

		virtual bool IsStateTag() const { return true; }

		ARTStem( int st = ARTStem::AUTO,ARTStem * p_savestem = NULL,
												ARTStem * p_copystem = NULL);

		virtual ~ARTStem();

		ARTStem(const ARTStem * tstem);

		virtual ARMusicalObject * Copy() const;

	    virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
	    virtual void printParameters(std::ostream& os) const;

		virtual void setTagParameterList(TagParameterList & tpl);

		const TagParameterFloat * getLength() const { return mTpfLength; }

		virtual const STEMSTATE getStemState() const { return mStemState; }

		virtual ARMusicalObject * getEndTag() const { return new ARTStem(AUTO, NULL, mSaveStem); }

	protected:

		ARTStem * mSaveStem;

		STEMSTATE mStemState;

		static ListOfTPLs ltpls;

		TagParameterFloat * mTpfLength;

};

#endif
