#ifndef MacFileSelector_H
#define MacFileSelector_H

/*
 * file : MacFileSelector.h
 * 
 *	Light version !
 *
 */

#include <Carbon/Carbon.h>
// #include <Navigation.h>

// #include "CString.h"
//#include "FileSelector.h"
//#include "MacStringTools.h"

// ---------------------------------------------------------------------------
pascal Boolean MacFileSelector_FilterCB( AEDesc * theItem, void * info, void * callBackUD, 
    														NavFilterModes filterMode );

// ---------------------------------------------------------------------------
pascal void MacFileSelector_EventCB( NavEventCallbackMessage selector, NavCBRecPtr params, 
															void * callBackUD );

// ---------------------------------------------------------------------------
class MacFileSelector // : public VFileSelector
{
	public:
			
								MacFileSelector();
		virtual					~MacFileSelector();

		// -- FileSelector heritage

		virtual bool			SelectOneFile( FSRef * outPath );
		virtual	bool			SelectSaveFile( CFURLRef * outPath );

	//	virtual bool			SelectOneFolder( FilePath * ioPath );
	//	virtual bool			SelectOneVolume( FilePath * ioPath );

		virtual void			SetQueryMessage( const char * inMessage );
		virtual void			SetDefaultSaveFileName( const char * inMessage );

		// -- Mac Specific
		virtual void			HandleNavEvent( NavEventCallbackMessage selector, NavCBRecPtr params );
	
	//	static bool				NavReplyToFSSpec( NavReplyRecord * inReply, FSSpec * outSpecs );
		virtual CFURLRef 		NavReplyToCFURL( const NavReplyRecord * navReply );
		virtual void			SetParentWindow( WindowRef in ); 
		virtual void			SetDefaultSaveFileName( CFStringRef inName );
	
	protected:

		virtual	bool			MacSetupDialogOptions( NavDialogCreationOptions * options );
		virtual	void			SetCFString( CFStringRef & outRef, const char * inMessage );
	
		NavDialogRef			mMacDialogRef;
		NavEventUPP				mEventProc;
		NavObjectFilterUPP		mFilterProc;
		CFStringRef				mQueryMessage;
		CFStringRef				mDefaultSaveFileName;
		WindowRef				mParentWindow;	// optional parent window.
		
};

#endif
