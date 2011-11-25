#ifndef GRNoteDot_H
#define GRNoteDot_H

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

class GREvent;
class GRSingleNote;
class GRChord;


/** \brief This class provides a note with a stem and a correct flag 

*/
class GRNoteDot : public GRNotationElement
{
  public:

		friend class GREvent;
		friend class GRSingleNote;
		friend class GRChord; 
	 
					GRNoteDot( GREvent * inNote, const TYPE_DURATION & duration, 
																float notebreite );

					GRNoteDot( GREvent * inNote, float notebreite, int inDotCount );
	
	 virtual		~GRNoteDot();

	 virtual void	GGSOutput() const;
	 virtual void	OnDraw( VGDevice & hdc ) const;
	 // virtual void setPosition(const NVPoint &position);
	 virtual void	print() const;

	 virtual int	getNumDots() const						{ return mDots; }

	 virtual const	NVPoint & getReferencePosition() const	{ return refpos; }
	 virtual const	NVPoint & getOffset() const				{ return mOffset; }
	 virtual const	unsigned char * getColRef() const		{ return mColRef; }

	 virtual float	getSize() const	 						{ return size; }

	  static int	DurationToDotCount( const TYPE_DURATION & duration );

  protected:

			void	GRNoteDotInit( GREvent * inNote, float notebreite );

	  // long notebreite;
	 int				mDots;
	 static NVPoint		refpos;
	 NVPoint			mOffset;
	 unsigned char *	mColRef;
	 float				size;
};

#endif
