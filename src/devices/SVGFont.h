#ifndef __SVGFont__
#define __SVGFont__

/*
  GUIDO Library
  Copyright (C) 2011 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "VGFont.h"
#ifdef INDEPENDENTSVG
#include "nanosvg.h"
#include <map>
#endif

/*!
\addtogroup VGSys Virtual Graphic System
@{
*/
class VGDevice;
// --------------------------------------------------------------
/** \brief a SVG font class.
*/
class SVGFont : public VGFont
{
	std::string	fGuidoFontFile;
	std::string	fName;
	int fSize;
	int fProperties;

#ifdef INDEPENDENTSVG
	struct NSVGimage *	fNSVGimage;
        static std::map<std::string, std::string> makeHexToCharMap();
        static const std::map<std::string, std::string> hexToCharMap;
#else
	const VGFont *	fFont;
	VGDevice *	fDevice;
#endif

	public:
					 SVGFont(const char * name, int size, int properties, const char * guidofontfile);
		virtual		~SVGFont();

		virtual	const char *	GetName() const			{
		#ifdef INDEPENDENTSVG
		  return fName.c_str();
		#else
		  return fFont->GetName();
                #endif
                }
		virtual	int 			GetSize() const {
		#ifdef INDEPENDENTSVG
		  return fSize;
                #else
		  return fFont->GetSize();
                #endif
                }
		virtual	int				GetProperties() const	{
		#ifdef INDEPENDENTSVG
		  return fProperties;
		#else
		  return fFont->GetProperties();
                #endif
		}

		virtual	void	GetExtent( const char * s, int inCharCount, float * outWidth, float * outHeight, VGDevice * context ) const;
		virtual	void	GetExtent( unsigned char c, float * outWidth, float * outHeight, VGDevice * context ) const;
};

/*! @} */

#endif
