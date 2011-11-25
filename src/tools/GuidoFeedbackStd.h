/*
	GUIDO Library
	Copyright (C) 2003  Grame

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

	Feedback notification class for GuidoEngine.
*/

#ifndef GuidoFeedbackStd_H
#define GuidoFeedbackStd_H

#include <cstdio>
#include <iostream>

#include "GuidoFeedback.h"
// --------------------------------------------------------------
class GuidoFeedbackStd : public GuidoFeedback
{
	public:		
						GuidoFeedbackStd( std::ostream & inStream = std::cout ) 
									: 	mOutStream( inStream ), mPrefix( 0 ),
										mCacheParam1(0), mCacheParam2(0),  mCacheStringID(-1) { }
									
		virtual			~GuidoFeedbackStd() { }
		
		virtual void	Notify( int /*inStatus*/ ) { }

		// - Dialog
		virtual void	CreateDialogWindow( int inTitleStringID = str_Title, int inDialogID = 0 ) 
							{ mPrefix = GetString( inTitleStringID ); }
		
		virtual void	InitDialog() { }
		virtual void	DestroyDialogWindow()  { mOutStream << std::endl; }

		virtual void	SetCancelButtonState( int /*state*/ ) { }

		virtual void	SetStatusMessage( int inStringID, int param1 = 0, int param2 = 0 );
		virtual	void	UpdateStatusMessage( int inStringID, int param1 = 0, int param2 = 0 );

		virtual void	HandleUserAction( int /*inAction*/ ) { }
		virtual bool	ProgDialogAbort() const { return false; }

	protected:

		std::ostream & 		mOutStream;
		
		int mCacheParam1;
		int mCacheParam2;
		int mCacheStringID;
		const char * mPrefix;
};

// --------------------------------------------------------------
inline void
GuidoFeedbackStd::SetStatusMessage( int inStringID, int param1, int param2 )
{
	UpdateStatusMessage( inStringID, param1, param2 );
}

// --------------------------------------------------------------
// control chars: '\b', '\t', '\v', '\f', '\n', '\r' and '\a'.
inline void 
GuidoFeedbackStd::UpdateStatusMessage( int inStringID, int param1, int param2 )
{
	if( inStringID == mCacheStringID ) 
	{
		if(( param1 == mCacheParam1 ) && ( param2 == mCacheParam2 ))
			return;
			
		mOutStream << '\r'; // Attemp to replace previous line (non portable)
	}
	else
		mOutStream << std::endl;
	
	// - Do not repeat identical messages
	mCacheStringID = inStringID;
	mCacheParam1 = param1;
	mCacheParam2 = param2; 

	// - Get a string for input ID
	const char * theString = GetString( inStringID );
	if( theString == 0 ) return;

	// - Format with parameters
	char tmpString [ 256 ];
	tmpString[ 0 ] = '\0';
	sprintf( tmpString, theString, param1, param2 );
	
	// - Output
	if( mPrefix )
		mOutStream << mPrefix << " ";
	
	mOutStream << tmpString;
}






#endif
