#ifndef GRNoteDot_H
#define GRNoteDot_H

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
#include "ARTHead.h"

class GREvent;
class GRSingleNote;


/** \brief This class provides a note with a stem and a correct flag 

*/
class GRNoteDot : public GRNotationElement
{
  public:
		friend class GREvent;
		friend class GRSingleNote;
		using GObject::addToOffset;
	 
					GRNoteDot( GREvent * inNote, const TYPE_DURATION & duration, 
																float notebreite );

					GRNoteDot( GREvent * inNote, float notebreite, int inDotCount );
	
	 virtual		~GRNoteDot();

	 virtual void	GGSOutput() const;
	 virtual void	OnDraw( VGDevice & hdc ) const;
	 // virtual void setPosition(const NVPoint &position);

	 virtual int	getNumDots() const						{ return mDots; }

	 virtual const	NVPoint & getReferencePosition() const	{ return refpos; }
	 
     virtual const	NVPoint & getOffset() const				{ return mOffset; }
     virtual void   addOffsetX(float inX)                   { mOffset.x += inX; }
     virtual void   addOffsetY(float inY)                   { mOffset.y += inY; }
	 
     virtual const	unsigned char * getColRef() const		{ return mColRef; }
	 virtual float	getSize() const	 						{ return size; }

	  static int	DurationToDotCount( const TYPE_DURATION & duration );

	  virtual void  adjustHorizontalDotPosition(float inNoteSize, ARTHead::HEADSTATE inHeadState, GDirection inGlobalStemDirection);

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
