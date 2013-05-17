#ifndef ARGlissando_H
#define ARGlissando_H

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
	ARGlissando * getNextGliss() {return nextGlissando;}
	void setNextGliss(ARGlissando *next);
	
protected:
	const TagParameterFloat * dx1;
	const TagParameterFloat * dy1;
	const TagParameterFloat * dx2;
	const TagParameterFloat * dy2;
	const TagParameterFloat * fThickness;
	const TagParameterString * fLineStyle;
	const TagParameterString * fFill;
	bool mWavy;
	bool mFill;
	bool mParSet;
	ARGlissando * nextGlissando;

	static ListOfTPLs ltpls;
};

#endif
