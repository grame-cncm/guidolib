#ifndef GDeviceWin2000_H
#define GDeviceWin2000_H

/*
	GUIDO Library
	Copyright (C) 2003 Grame

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

	Win2000 implementation of VGDevice drawing services. 
	Requires visual c++ 7 (.net) or greater.
*/

/*
#ifndef _WINDOWS_
#include <afx.h>
#endif
*/

//***************************************************************//
//**************** WARNING - WARNING - WARNING ******************//
//																 //
//		 			   UN-MAINTAINED DEVICE						 //
//				Doesn't match the new VGDevice API				 //
//																 //
//						-- Grame 2007 --						 //
//***************************************************************//


#include "GDeviceWin32.h"
// --------------------------------------------------------------
class GDeviceWin2000 : public GDeviceWin32
{
	public:
							//	GDeviceWin2000( GWindowRef h );
								GDeviceWin2000( HDC hdc, int inWidth, int inHeight );
		virtual					~GDeviceWin2000();

	protected:

		// - Implementation

		virtual void			DoSetScale( float prevX, float prevY, float newX, float newY );
		virtual void			DoSetOrigin( float prevX, float prevY, float newX, float newY );

//		virtual void			DoSelectPen( const GColor & c, float width = 1 ); 	// Stroke color 
//		virtual void			DoSelectBrush( const GColor & c );					// Fill color
		virtual void			SelectPen( const VGColor & c, float width = 1 ); 	// Stroke color 
		virtual void			SelectBrush( const VGColor & c );					// Fill color

		virtual	bool			AllowNonProportional() const 
									{ return true; } // we must override the GDeviceWin32Win32 "false"

		// - Win32 specific
		virtual	void			SetupWinPenAndBrush();
		virtual void			RestoreWinPenAndBrush();
		
		virtual void			Win32SetupTransformMode();
		virtual bool			Win32ApplyTransform();

	private:

				HPEN			mNullPen;
				bool			mMustDeletePen;
};

#endif