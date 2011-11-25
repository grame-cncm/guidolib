#ifndef GRNoteHead_H
#define GRNoteHead_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2003	Grame

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


#include "GRNotationElement.h"

/** \brief not yet documented
*/
class GRNoteHead : public GRNotationElement  
{
  public:

					  	GRNoteHead(); 
      virtual 			~GRNoteHead();


	  virtual const NVPoint & getOffset() const
	  {
		  return mOffset;
	  }
	  virtual void setOffset(const NVPoint &noff)
	  {
		  mOffset = noff;
	  }
	  virtual void addToOffset(const NVPoint &pt)
	  {
		mOffset += pt;
	  }
	  virtual float getSize() const { return mSize; }

	  virtual const  unsigned char * getColRef() const
	  {
		  return mColRef;
	  }

  protected:

	  NVPoint mOffset;

	  float mSize;
	  unsigned char * mColRef;

};


#endif


