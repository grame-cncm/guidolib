#ifndef GRNoteHead_H
#define GRNoteHead_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003, 2004 	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GRNotationElement.h"

/** \brief not yet documented
*/
class GRNoteHead : public GRNotationElement  
{
public:
			  	           GRNoteHead(); 
    virtual 			  ~GRNoteHead();

    virtual const NVPoint& getOffset() const              { return mOffset; }
	virtual       void     setOffset(const NVPoint &noff) { mOffset = noff; }
	virtual       void     addToOffset(const NVPoint &pt) { mOffset += pt; }
	virtual       float    getSize() const                { return mSize; }
	virtual const unsigned char *getColRef() const        { return mColRef; }

protected:
    NVPoint mOffset;

	float mSize;
	unsigned char * mColRef;
};

#endif
