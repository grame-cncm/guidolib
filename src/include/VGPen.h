#ifndef VGPen_H
#define VGPen_H


/*
  GUIDO Library
  Copyright (C) 2006	Grame
	
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

class VGColor;

/*!
\addtogroup VGSys Virtual Graphic System
@{
*/

// --------------------------------------------------------------
// 		VGPen class
// --------------------------------------------------------------
/** \brief Generic class to manipulate device independant pens. 
*/				
class VGPen
{
	public:
				VGPen() : mWidth( 1 ) { }
		void	Set( const VGColor & inColor, float inWidth )	{ mColor = inColor; mWidth = inWidth; }
		void	Set( const VGColor & inColor )					{ mColor = inColor; }
		void	Set( float inWidth )							{ mWidth = inWidth; }
				
		VGColor mColor;
		float mWidth;
};	

/*! @} */

#endif /* VGPen_H */
