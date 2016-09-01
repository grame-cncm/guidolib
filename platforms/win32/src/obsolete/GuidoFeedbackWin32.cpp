/*
	GUIDO Library
	Copyright (C) 2003, 2004  Grame

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

#include <cstdio>

#define WINVER 0x0410
#include "GuidoFeedbackWin32.h"
// #include "resrc1.h"	// Dialog consts
//#include "secureio.h"
#include <stdio.h>
#define snprintf _snprintf

GuidoFeedback * GuidoFeedbackWin32::sCurrFeedback = 0;
// --------------------------------------------------------------
static 
INT_PTR CALLBACK MyProgressDlgProc( HWND hDlg, UINT message, 
											WPARAM wParam,LPARAM lParam )
{
	GuidoFeedback * feedback = GuidoFeedbackWin32::GetFeedback( hDlg );
	if( feedback == 0 ) 
		return FALSE;

	switch( message )
	{
		case WM_INITDIALOG:
			feedback->InitDialog();
			return TRUE;
	
		case WM_COMMAND:
			switch( wParam )
			{
				case IDCANCEL :
					feedback->HandleUserAction( GuidoFeedback::kActionCancel );
						// old: EndDialog(hDlg,0);
					return TRUE;
			}
			break;
	}
	return FALSE;
}

// --------------------------------------------------------------
GuidoFeedbackWin32::GuidoFeedbackWin32()
{	 
	Init();
}

// --------------------------------------------------------------
GuidoFeedbackWin32::GuidoFeedbackWin32( HMODULE hmod, int inDefaultDialogID, int inStatusItemID )
{	 
	Init();
	Setup( hmod, inDefaultDialogID, inStatusItemID );
}

// --------------------------------------------------------------
void
GuidoFeedbackWin32::Setup( HMODULE hmod, int inDefaultDialogID, int inStatusItemID )
{
	mModule = hmod;
	mDefaultDialogID = inDefaultDialogID;
	mStatusItemID = inStatusItemID;
}
// --------------------------------------------------------------
void
GuidoFeedbackWin32::Init()
{	 
	mModule = 0;
	mDefaultDialogID = 0;
	mStatusItemID = 0;
	mStatusMsg[ 0 ] = 0;
	mProgDialogAbort = false;
	mHdlg_progress = 0;
	mPrevCursor = 0;
	sCurrFeedback = this;	// temp
}

// --------------------------------------------------------------
GuidoFeedbackWin32::~GuidoFeedbackWin32()
{
	if( this == sCurrFeedback  )	// temp
		sCurrFeedback = 0;			// temp
}

// --------------------------------------------------------------
void 
GuidoFeedbackWin32::Notify( int id )
{	 
	switch( id )
	{
		case kProcessing:
			mPrevCursor = ::SetCursor( ::LoadCursor( NULL, IDC_WAIT ));			
			break;
		
		case kIdle:
			if( mPrevCursor ) ::SetCursor( mPrevCursor );
			break;
	}	
}

// --------------------------------------------------------------
void 
GuidoFeedbackWin32::CreateDialogWindow( int inTitleStringID,  
											int inDialogID )
{
	int dialogID = inDialogID ? inDialogID : mDefaultDialogID;

//	mHdlg_progress = NULL;					
//	HMODULE hmod = ::LoadLibrary( "GUIDOEngine.dll" );
	mHdlg_progress = ::CreateDialog( mModule, MAKEINTRESOURCE( dialogID ),NULL,
						(DLGPROC) MyProgressDlgProc);
	//::FreeLibrary( hmod );
	
	SetDialogTitle( inTitleStringID );
}

// --------------------------------------------------------------
void
GuidoFeedbackWin32::InitDialog()
{
	mProgDialogAbort = false;
			
	HWND hstat = ::GetDlgItem( mHdlg_progress, mStatusItemID );
	::SetWindowText( hstat, mStatusMsg );
}

// --------------------------------------------------------------
void 
GuidoFeedbackWin32::DestroyDialogWindow()
{
	// ::Sleep( 1000 ); // debug	
	if( mHdlg_progress )
	{
		::DestroyWindow( mHdlg_progress );
		mHdlg_progress = NULL;
	}
}

// --------------------------------------------------------------
void 
GuidoFeedbackWin32::SetDialogTitle( int inStringID )
{
	::SetWindowText( mHdlg_progress, GetString( inStringID ));
}

// --------------------------------------------------------------
void 
GuidoFeedbackWin32::SetCancelButtonState( int state )
{
	::EnableWindow( GetDlgItem( mHdlg_progress,IDCANCEL ), state );
}

// --------------------------------------------------------------
void
GuidoFeedbackWin32::SetStatusMessage( int inStringID, int param1, int param2 )
{
	mStatusMsg[0] = 0;
	const char * theString = GetString( inStringID );
	if( theString )
		snprintf( mStatusMsg, 512, theString, param1, param2 );

	// ::Sleep( 1000 ); // debug		
}

// --------------------------------------------------------------
void 
GuidoFeedbackWin32::UpdateStatusMessage( int inStringID, int param1, int param2 )
{
	char paramstr [256] = "";
	const char * theString = GetString( inStringID );
	if( theString )
		snprintf( paramstr, 256, theString, param1, param2 );	

	if( mHdlg_progress )	
	{
		if( theString != NULL )
			::SendDlgItemMessage( mHdlg_progress, mStatusItemID /* was IDC_STATUS*/ , 
									WM_SETTEXT, (WPARAM) 0, (LPARAM) paramstr );

		MSG msgDesc;
		// now we process messages for the progress-dialog ...
		while( ::PeekMessage( &msgDesc, mHdlg_progress, 0, 0, PM_REMOVE ))
		{
			if( ::IsDialogMessage( mHdlg_progress, &msgDesc ))
			{
				::TranslateMessage( &msgDesc );
				::DispatchMessage( &msgDesc );
			}
		}
	}
	// ::Sleep( 1000 ); // debug	
}

// --------------------------------------------------------------
void 
GuidoFeedbackWin32::HandleUserAction( int inAction )
{
	switch( inAction )
	{
		case kActionCancel:		mProgDialogAbort = true;	break;
	}
}

/*
// --------------------------------------------------------------
const char *	
GuidoFeedbackWin32::GetString( int inStringID ) const
{
//	const char * outStr;

//	if( inStringID >= 1 && inStringID < kLastFeedbackString )
//		return FeedbackStringsTable[ inStringID - 1 ];
	

	switch( inStringID )
	{
		case str_MusicalPreProcessing1:
			outStr = "Musical pre-processing I. Voice %d of %d";
			break;

		case str_MusicalPreProcessing2:
			outStr = "Musical pre-processing II. Voice %d of %d";
			break;

		case str_DeletingVoice:
			outStr = "Deleting Voice %d";
			break;

		case str_DeletingPages:
			outStr = "Deleting Pages... %d%%";
			break;

		case str_ARMusicCreated:
			outStr = "After parse. ARMusic has been created";
			break;

		case str_CreatingGR:
			outStr = "Now we are creating the GR";
			break;

		case str_RecreatingGraphElements:
			outStr = "Recreating Graphical elements";
			break;

		case str_CreatingGraphSymbols:
			outStr = "Creating Graphical symbols... %d%%";
			break;

		case str_CreatingLinesAndSystems:
			outStr = "Creating lines/systems of music... %d%%";
			break;

		case str_CalcLineAndPageBreak:
			outStr = "Calculating the optimum line and pagebreak.";	
			break;

		case str_BeginningOfParse:
			outStr = "Beginning of parse";
			break;
		
		case str_ClosingFile:
			outStr = "Closing file";
			break;

		case str_ClosingFileProgress:
			outStr = "Closing file - progress indicator";
			break;

		default:
			return outStr;
	}
	return outStr;
}
*/
/*
// --------------------------------------------------------------
void EnableAbortButton( int enable )
{
	if (hdlg_progress)	{
		::EnableWindow(::GetDlgItem(hdlg_progress,IDCANCEL),enable);
	}
}
*/