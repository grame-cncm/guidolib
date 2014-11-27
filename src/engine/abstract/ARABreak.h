#ifndef ARABreak_H
#define ARABreak_H

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

class ARMusicalObject;
class ARMusicalVoiceState;

/** \brief The Break tag
*/

class ARABreak : public ARMTParameter   // public ARMusicalObject
{
	friend class ARMusicalVoiceState;

public:

	enum breakstate { ON, OFF };

					ARABreak();
	virtual 		~ARABreak();

	breakstate getSystemBreakState() const { return mSystemBreakState; }
	breakstate getPageBreakState() const { return mPageBreakState; } 
	virtual void setTagParameterList(TagParameterList & tpl);
	virtual void PrintName( std::ostream & os) const;
	virtual void PrintParameters( std::ostream & os) const;

	virtual void print(int &indent) const { }
	virtual bool IsStateTag() const { return true; }

protected:

	breakstate mPageBreakState;
	breakstate mSystemBreakState;

	static ListOfTPLs ltpls;
};

#endif
