#ifndef ARTremolo_H
#define ARTremolo_H

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
#include "ARNote.h"

class TagParameterInt;

/** \brief not yet documented
*/

class ARTremolo : public ARMTParameter, public ARPositionTag
{		
public:
			 ARTremolo();
             ARTremolo(const ARTremolo * tremolo);
	virtual ~ARTremolo();

	virtual void setTagParameterList(TagParameterList & tpl);
	virtual TagParameterList * getTagParameterList() const;

	virtual bool 	MatchEndTag(const char * s);
	bool getParSet() const	{ return mParSet; }

	const TagParameterString * getStyle() const	{ return fStyle; }
	const TagParameterInt * getSpeed() const	{ return fSpeed; }
	const TagParameterString * getPitch() const	{ return fPitch; }
	const TagParameterFloat * getDx() const	{ return dx; }
	const TagParameterFloat * getDy() const	{ return dy; }
	const TagParameterFloat * getThickness() const	{ return fThickness; }
	const TagParameterString * getText() const	{ return fText; }
    
    bool isSecondPitchCorrect();
    int getNumberOfStrokes();

	virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
	virtual void printParameters(std::ostream& os) const;

protected:

	// TagParameters ..
	// TagParameterString *textformat;
	// TagParameterFloat  *fsize;
	const TagParameterString *fStyle;
	const TagParameterInt *fSpeed;
	const TagParameterString *fPitch;
	const TagParameterFloat *dx;
	const TagParameterFloat *dy;
	const TagParameterFloat *fThickness;
	const TagParameterString *fText;
	bool mParSet;

	// this is important for allowed
	// TagParameters ....
	static ListOfTPLs ltpls;
};

#endif
