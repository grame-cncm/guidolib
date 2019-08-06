#ifndef ARSymbol_H
#define ARSymbol_H

/*
GUIDO Library
Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
Copyright (C) 2002-2017 Grame

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.

Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
research@grame.fr

*/

#include "ARMTParameter.h"
#include "ARPositionTag.h"
#include "ARMusic.h"

/*
// documentation is hidden, the tag is not supported by the SVG backend
@mkdoc

@group:Miscellaneous

@tagname:\symbol
@tagalias:\s
@tagtype:RP
@tagnotation:intended to include arbitrary graphics.
@tagdesc
The **\symbol** tag allows to include arbitrary graphics using an image file.
@tagend

@params:
@param:file:string:a file name:*none*:false
@param:position:string:top, bottom, bot or over:over:true
@param:w:integer:the image width:*none*:true
@param:h:integer:the image height:*none*:true
@paramdesc
Note: the **\symbol** tag is not supported by the SVG backend.
@paramend

*/

/** \brief The symbol tag
*/
class ARSymbol : public ARMTParameter, public ARPositionTag
{
public:
				 ARSymbol();
    virtual 	~ARSymbol() {}

		virtual const char*	getParamsStr() const	{ return kARSymbolParams; };
		virtual const char*	getTagName() const		{ return "ARSymbol"; };
		virtual std::string getGMNName() const		{ return "\\symbol"; };

    const char*  getSymbolPath() const;
	const char*  getPositionString() const;
    int          getFixedWidth() const;
    int          getFixedHeight() const;
	
    void setCurrentARMusic(ARMusic *inARMusic)      { currentARMusic = inARMusic; }

    const std::vector<std::string> &getPath() const { return currentARMusic->getPath(); }

protected:
		ARMusic*	currentARMusic;
};

#endif
