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

#ifndef GuidoFeedbackWin32_H
#define GuidoFeedbackWin32_H

#include "GuidoFeedback.h"
#include "MyHeaders.h"

// --------------------------------------------------------------
class GuidoFeedbackWin32 : public GuidoFeedback 
{
	public:		
						 // Caller can use a HINSTANCE as HMODULE
						GuidoFeedbackWin32( HMODULE hmod, int inDefaultDialogID, int inStatusItemID );
						GuidoFeedbackWin32();
		virtual			~GuidoFeedbackWin32();

				void	Setup( HMODULE hmod, int inDefaultDialogID, int inStatusItemID );

		virtual void	Notify( int inStatus );

		// - Dialog
		virtual void	CreateDialogWindow( int inTitleStringID = str_Title, int inDialogID = 0 );
		virtual void	InitDialog();
		virtual void	DestroyDialogWindow();
		
		virtual void	SetCancelButtonState( int state );

		virtual void	SetStatusMessage( int inStringID, int param1 = 0, int param2 = 0 );							
		virtual	void	UpdateStatusMessage( int inStringID, int param1 = 0, int param2 = 0  );
			 
		virtual void	HandleUserAction( int inAction );
		virtual bool	ProgDialogAbort() const { return mProgDialogAbort; } 

		// To do: use inDlg to find and return the good object.
		static GuidoFeedback * GetFeedback( HWND inDlg ) { return sCurrFeedback; } 

	private:

		virtual void	SetDialogTitle( int inStringID );
				void	Init();		

		static	GuidoFeedback * sCurrFeedback;

		HMODULE mModule;
		char 	mStatusMsg [ 512 ];		// the status text for the dialog
		HWND	mHdlg_progress;			// Progress dialog handle
		HCURSOR mPrevCursor;
		bool	mProgDialogAbort;		// if a user hit the progdialogabort-button
		int		mDefaultDialogID;
		int		mStatusItemID;			// current status text widget
};


#endif