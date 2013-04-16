#ifndef GRAccolade_H
#define GRAccolade_H

/*
  GUIDO Library
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

  (JB) 04/2004. Creation of GRAccolade to separate the accolade code from
				the class GRSystem. It currently derives from GObject, but
				it should probably be a GRTagARNotationElement

*/

#include "GObject.h"


/** \brief The system accolade


*/
class GRAccolade : public GObject // GRTagARNotationElement ?
{
	public:

		enum { kAccoladeNone, kAccoladeStraight, kAccoladeCurly, kAccoladeThin };
		
						GRAccolade( int inAccoladeType = kAccoladeCurly, float inDx = 0 );
		virtual			~GRAccolade();

		virtual void	setAccoladeType( int in ) { mAccoladeType = in; }
		virtual void	setDx( float in ) { mDx = in; }

		virtual void	draw( VGDevice & hdc, const NVPoint & leftTop, 
								const NVPoint & leftBottom ) const;

		virtual void	OnDraw( VGDevice &) const { }
		
		
	protected:


		void		DrawEmulatedAccolade( VGDevice & hdc, float x, 
											float yTop, float yBottom ) const;
	
		void		DrawStraightAccolade( VGDevice & hdc, float x, float yTop, 
									float yBottom, bool drawEndCurves = true ) const;
	

		
		int			mAccoladeType;
		float		mDx;
		
};

#endif
