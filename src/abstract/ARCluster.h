#ifndef ARCluster_H
#define ARCluster_H

/*
GUIDO Library
Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#include "ARMTParameter.h"
#include "ARPositionTag.h"
#include <string>

class TagParameterInt;
class TagParameterString;

/** \brief not yet documented
*/

class ARCluster : public ARMTParameter, public ARPositionTag
{		
public:

	ARCluster();

	virtual		 	~ARCluster();

	virtual void print() const {};
	virtual void PrintName(std::ostream & os) const;
	virtual void PrintParameters(std::ostream & os) const;

	virtual void    setTagParameterList(TagParameterList & tpl);

    void setNotePitchAndOctave(int inPitch, int inOctave, int inAccidental);

    int *getFirstNoteParameters() {return mFirstNote;}
    int *getSecondNoteParameters() {return mSecondNote;}

    float        getadx() const;
    float	     getady() const;
    float        getahdx() const;
    float	     getahdy() const;
    float        getSize() const;
    TagParameterString *getColor() const;

protected:

    float adx;
    float ady;
    float ahdx;
    float ahdy;
    float aSize;
    TagParameterString *aColor;

    static ListOfTPLs ltpls;

    int mFirstNote[2];
    int mSecondNote[2];
};

#endif
