#ifndef STATICFONT_H
#define STATICFONT_H
/*
  GUIDO Library
  Copyright (C) 2019	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <string>
#include <map>

#include "VGFont.h"

#ifdef SMUFL
#define kMaxTblIndex	3647
#else
#define kMaxTblIndex	256
#endif

class StaticFont : public VGFont
{
   public:
        /*!
		 *  \brief A font with pre-computed static metrics.
		 *
         */
				 StaticFont(const char * name, int size, int properties)
							: fName(name), fSize(size), fProperties(properties) {}
        virtual ~StaticFont() { }

        const char *	GetName() const			{ return fName.c_str(); }
        int 			GetSize() const			{ return fSize; }
        int				GetProperties() const	{ return fProperties; }

        virtual void	GetExtent( const char * s, int inCharCount, float * outWidth, float * outHeight, VGDevice * context ) const;
        virtual void	GetExtent( int c, float * outWidth, float * outHeight, VGDevice * context ) const;

    protected:
		int			fExtends[kMaxTblIndex+1];	// the static extend values
		float 		fRefSize=1;		// the font size used to compute the extends map
		int 		fHeight=1;		// the static height value
		int			fTblOffset=0;

        virtual void initialize() = 0;		// must initialize the fExtends maps, the height and the ref size

    	inline float	GetExtend (int c) const
    		{  int i=c-fTblOffset; return ((i >= 0) && (i <= kMaxTblIndex)) ? fExtends[i] : 0.f; }

		std::string fName;			// the font name
		int 		fSize;			// the font size
        int 		fProperties;	// the font properties
};

#endif
