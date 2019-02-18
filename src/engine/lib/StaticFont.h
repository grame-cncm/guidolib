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

class StaticFont : public VGFont
{
   public:
        /*!
		 *  \brief A font with pre-computed static metrics.
		 *
         */
				 StaticFont(const char * name, int size, int properties)
							: fName(name), fSize(size), fProperties(properties) { initialize(); }
        virtual ~StaticFont() {}

        const char *	GetName() const			{ return fName.c_str(); }
        int 			GetSize() const			{ return fSize; }
        int				GetProperties() const	{ return fProperties; }

        void	GetExtent( const char * s, int inCharCount, float * outWidth, float * outHeight, VGDevice * context ) const;
        void	GetExtent( int c, float * outWidth, float * outHeight, VGDevice * context ) const;

    protected:
		typedef std::map<int, float> TExtendMap;
		TExtendMap	fExtends;		// the static extend values
		float 		fRefSize=1;		// the font size used to compute the extends map
		int 		fHeight=1;		// the static height value

        virtual void initialize() {}		// must initialize the fExtends maps, the height and the ref size

	private:
		std::string fName;			// the font name
		int 		fSize;			// the font size
        int 		fProperties;	// the font properties

    	inline float	GetExtend (int c) const
    		{  TExtendMap::const_iterator i = fExtends.find(c); return (i==fExtends.end()) ? 0.f : i->second; }
};

#endif // GUIDOFONT_H
