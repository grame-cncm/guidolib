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

    virtual const char* getSymbolPath() const				{ return aFilePath ? aFilePath->getValue() : 0; }

    virtual void print(int &indent) const;
    virtual void PrintName(std::ostream & os) const;
    virtual void PrintParameters(std::ostream & os) const;
    float		 getSize() const                            { return aSize; }
    const char*  getPositionString() const                  { return aPosition ? aPosition->getValue() : NULL; }
    int          getFixedWidth() const                           { return aFixedWidth ? aFixedWidth->getValue() : 0; }
    int          getFixedHeight() const                          { return aFixedHeight ? aFixedHeight->getValue() : 0; }

            void setCurrentARMusic(ARMusic *inARMusic);
    const std::vector<std::string> &getPath() const        { return aCurrentARMusic->getPath(); }

protected:

    virtual const char *getTagFormat() const;

	TagParameterString *aFilePath;
	             float  aSize;
    TagParameterString *aPosition;
    TagParameterInt    *aFixedWidth;
    TagParameterInt    *aFixedHeight;

               ARMusic *aCurrentARMusic;

	static ListOfTPLs ltpls;
};

#endif
