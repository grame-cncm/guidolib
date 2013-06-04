#ifndef ARGlissando_H
#define ARGlissando_H

/*
  GUIDO Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARMTParameter.h"
#include "ARPositionTag.h"



class ARGlissando : public ARMTParameter, public ARPositionTag
{
public:

				ARGlissando();
				ARGlissando(const ARGlissando * glissando);
	virtual		~ARGlissando(void);

	
	virtual void setTagParameterList(TagParameterList & tlist);
	virtual TagParameterList * getTagParameterList() const;
	virtual void 	print() const;
	virtual void	browse(TimeUnwrap& mapper) const;
	virtual bool 	MatchEndTag(const char * s);
	bool getParSet() const	{ return mParSet; }

	const TagParameterFloat * getDx1() const	{ return dx1; }
	const TagParameterFloat * getDy1() const	{ return dy1; }
	const TagParameterFloat * getDx2() const	{ return dx2; }
	const TagParameterFloat * getDy2() const	{ return dy2; }
	const TagParameterFloat * getThickness() const	{ return fThickness; }
	const TagParameterString * getLineStyle() const	{ return fLineStyle; }
	bool isWavy() {return mWavy;}
	bool isFill() {return mFill;}
	
protected:
	const TagParameterFloat * dx1;
	const TagParameterFloat * dy1;
	const TagParameterFloat * dx2;
	const TagParameterFloat * dy2;
	const TagParameterFloat * fThickness;
	const TagParameterString * fLineStyle;
	const TagParameterString * fill;
	bool mWavy;
	bool mFill;
	bool mParSet;

	static ListOfTPLs ltpls;
};

#endif
