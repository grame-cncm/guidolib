#ifndef GuidoQuartzViewerApp_H
#define GuidoQuartzViewerApp_H

/*
 *
 * File: GuidoQuartzViewerApp.h
 *
 *
 *
 */

#include <vector>
#include <Carbon/Carbon.h>

//#include "GuidoFeedbackStd.h"
#include "GDeviceOSX.h"
#include "GUIDOEngine.h"
#include "GuidoPrefs.h"

class GuidoDocWindow;

// ----------------------------------------------------------------------------


pascal OSStatus MyAppCarbonEventHandler( EventHandlerCallRef myHandler,
									EventRef event, void * userData );

pascal OSErr MyAEOpenDocHandler( const AppleEvent * theAppleEvent, 
							AppleEvent * reply, long handlerRefCon );

pascal OSStatus GuidoWindowEventHandler( EventHandlerCallRef inCallRef, 
							EventRef inEvent, void * inUserData );

// ----------------------------------------------------------------------------
class GuidoQuartzViewerApp
{
	public:
							GuidoQuartzViewerApp();
		virtual				~GuidoQuartzViewerApp();

		virtual void 		Run();
		virtual void 		OpenFile();
		virtual void 		OpenFile( const FSRef & inFile );

		virtual bool		MultiDocCapable() { return true; }
			
	//	virtual	void		ChangeScorePage( int inOffset );	

		virtual	bool		HandleCommand( const HICommand & inCommand );		
		virtual OSStatus	HandleWindowEvent( EventRef inEvent, GuidoDocWindow * inDoc );	
				bool		HandleProcessCommandEvent( EventRef inCommandEvent );
		
		virtual	void		DisplayAboutBox();
		virtual	void		DisplayPrefsDialog();
		virtual	void		DisplayGuidoWebPage();

		virtual void		UpdateMenuState( MenuRef inMenuRef );
		virtual void		UpdateAllMenuStates();

		virtual void		UpdateAllGRs();
		
		static GuidoQuartzViewerApp * sTheApp;

		IBNibRef	 		GetNibRef() const 		{ return mNibRef; }	

	static WindowRef 		FindWindowByTitle( CFStringRef inTitle ); // TODO: move in toolkit
		
	protected:
				
		static 	void		SetupMenuItem( MenuRef inMenuRef, int inItem, bool enabled, bool checked = false );

		// - Carbon related
			
				bool 		Initialize();
				bool		InitMyEventHandlers();
	
			EventHandlerUPP GetGuidoWindowEventHandlerUPP();
			
		// - Custom
					
			
				bool		SetupMusicalFont();

		GuidoDocWindow *	CreateGuidoDocWindow( const FSRef * inPath = nil );
				void		CloseDocument( GuidoDocWindow * in );
		GuidoDocWindow * 	GetFrontDocument();
		GuidoDocWindow *	FindDocumentForPath( const FSRef & inPath );
	
				void		MakeInitGDevice();
				void		ReleaseInitGDevice();
				
				void		RefreshGuidoView( WindowRef inWindowRef );
				void		NotifyNewGuidoHandle( int inGuidoHandle );
				bool		AdjustWindowToScreen( WindowRef inWinRef );
			//			GDeviceOSX &	GetGDevice() { return mGDevice; }

				bool		InitGuido( /*VGDevice * inDevice */);
				void 		SetupGuidoWindow();

				void		OpenGuidoFile( const FSRef & inPath, GuidoDocWindow * inDoc = nil );
				void		OpenXMLFile( const FSRef & inPath, GuidoDocWindow * inDoc = nil, int mode=0 );

				void		HandlePrintCommand();
				void 		ExportBitmap();


				void		ReportGuidoError( GuidoErrCode inGuidoErr );
				void		ReportError( const char * inMessage, const char * inDetail = 0 );

				bool 		DisplayWebPage( CFStringRef inCFString );

				// - Debug
				void		TestGuidoFactory();
				void 		ShowDebugWindow();

		// - Carbon related		
				
				//		CFBundleRef mBundle;
				IBNibRef 			mNibRef;
		 		EventHandlerUPP 	mAppEventCB;			// carbon events application handler UPP
				EventHandlerUPP		mWindowEventCB;			// carbon events window handler UPP
				AEEventHandlerUPP 	mAEEventCB;				// apple events handler upp
				bool 				mIsSetupGood;

   		 		EventHandlerRef		mAppEventHandlerRef;
		
		// - Custom

			VGDevice*				mInitGDevice;
			VGSystem*				mInitGSystem;
			WindowRef 				mFooWindow;	// used for gdevice initialization.
			std::vector<GuidoDocWindow *> mDocList;
//			GuidoFeedbackStd		mFeedback;
			GuidoLayoutSettings		mSettings;
			GuidoPrefs				fPrefs;
};


#endif

