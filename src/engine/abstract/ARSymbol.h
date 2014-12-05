#ifndef ARSymbol_H
#define ARSymbol_H

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
#include "TagParameterString.h"
#include "TagParameterInt.h"
#include "ARMusic.h"


class TagParameterList;

/** \brief The symbol tag
*/
class ARSymbol : public ARMTParameter, 
    public ARPositionTag
{
public:
    ARSymbol();
    virtual 		~ARSymbol();

    virtual void 	setTagParameterList(TagParameterList & theTagParameterList);

    virtual const char* getSymbolPath() const				{ return filePath ? filePath->getValue() : 0; }

	virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
	virtual void printParameters(std::ostream& os) const;

    float		 getSize() const                    { return size; }
    const char*  getPositionString() const          { return position ? position->getValue() : NULL; }
    int          getFixedWidth() const              { return width ? width->getValue() : 0; }
    int          getFixedHeight() const             { return height ? height->getValue() : 0; }
    
    void setCurrentARMusic(ARMusic *inARMusic)      { currentARMusic = inARMusic; }

    const std::vector<std::string> &getPath() const { return currentARMusic->getPath(); }

protected:

    virtual const char *getTagFormat() const;

	TagParameterString *filePath;
	             float  size;
    TagParameterString *position;
    TagParameterInt    *width;
    TagParameterInt    *height;

               ARMusic *currentARMusic;

	static ListOfTPLs ltpls;
};

#endif
