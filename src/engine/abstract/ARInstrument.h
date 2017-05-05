#ifndef ARInstrument_H
#define ARInstrument_H

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

class TagParameterString;

/** \brief not yet documented
*/
class ARInstrument : public ARMTParameter
{
	public:
				 ARInstrument(bool autopos);
		virtual ~ARInstrument();	

		virtual void setTagParameterList(TagParameterList & theTagParameterList);

		virtual int	 getOrder() const		{ return kInstrumentOrder; }
	    virtual void printName(std::ostream& os) const;
	    virtual void printGMNName(std::ostream& os) const;
	    virtual void printParameters(std::ostream& os) const;

		const char* getName() const;
		const TagParameterString *getTransp() const { return fTransp; }
		virtual bool IsStateTag() const 		    { return true; }

		const char* getFont() const;
		const char* getTextFormat() const;
		const char* getTextAttributes() const;
		float getSize() const;
		bool autoPos() const	{ return fAutoPos; }

	private:
		TagParameterString * fName;
		TagParameterString * fTransp;
		TagParameterString * fFont;
		TagParameterString * fTextFormat;
		TagParameterString * fTextAttributes;
		TagParameterFloat *	 fSize;
		bool fAutoPos;

		static ListOfTPLs ltpls;
};

#endif
