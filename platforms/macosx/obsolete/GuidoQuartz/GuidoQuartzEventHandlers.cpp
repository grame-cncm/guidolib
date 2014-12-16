/*
 *
 * File: GuidoQuartzEventHandlers.cpp
 *
 *
 *
 */
 
#include "GuidoQuartzViewerApp.h"

class GuidoDocWindow;
// ----------------------------------------------------------------------------
// 		* MyAEOpenDocHandler
// ----------------------------------------------------------------------------
// AppleEvents callback for the whole application. Used for 'open document' events
pascal OSErr MyAEOpenDocHandler( const AppleEvent * theAppleEvent, AppleEvent * reply, long handlerRefCon )
{
	GuidoQuartzViewerApp * theApp = GuidoQuartzViewerApp::sTheApp;
	if( theApp == nil ) return 128;
	
//	FSSpec myFSS;
	AEDescList docList;
	OSErr err = noErr;
	long index, itemsInList;
	Size actualSize;
	AEKeyword keywd;
	DescType returnedType;
	
	// get the direct parameter--a descriptor list--and put it into a docList
	if (!err) err = ::AEGetParamDesc( theAppleEvent, keyDirectObject, typeAEList, &docList );
	
/*
	// check for missing parameters
//	if (!err) err = MyGotRequiredParams(theAppleEvent);
	
	// count the number of descriptor records in the list
	if (!err) err = AECountItems(&docList, &itemsInList);
	
	// now get each descriptor record from the list, coerce the returned
	// data to an FSSpec record, and open the associated file
	for (index = 1; index <= itemsInList && !err; index++) {
		err = AEGetNthPtr(&docList, index, typeFSS, &keywd, &returnedType, (Ptr)&myFSS, sizeof(myFSS), &actualSize);
		// if(!err)err = handleDroppedFSSpec(&myFSS);
	}
*/

	// - Determine the number of files to open
	if (!err) err = ::AECountItems( &docList, &itemsInList );

	if( theApp->MultiDocCapable() == false )
		itemsInList = 1;

	// - Open each file
	FSRef filePath;
	for (index = 1; index <= itemsInList && !err; index ++) 
	{
 		err = ::AEGetNthPtr( &docList, index, typeFSRef, &keywd, &returnedType, 
 									(Ptr)&filePath, sizeof(filePath), &actualSize );
		if( err == noErr && theApp )
			theApp->OpenFile( filePath );
	}
	
	if (!err) err = ::AEDisposeDesc(&docList);
	
	// cout << "Open File" << endl;
	
	return 128;
}

// ----------------------------------------------------------------------------
// 		* MyAppCarbonEventHandler
// ----------------------------------------------------------------------------
// Application level carbon event handler callback.
pascal OSStatus MyAppCarbonEventHandler( EventHandlerCallRef myHandler, EventRef inEvent, void * userData)
{
	GuidoQuartzViewerApp * theApp = GuidoQuartzViewerApp::sTheApp;
	if( theApp == nil ) return 128;

    OSStatus outResult = eventNotHandledErr;
    OSStatus result = noErr;
    UInt32 eventClass = ::GetEventClass( inEvent );
    UInt32 eventKind = ::GetEventKind( inEvent );

	switch( eventClass ) 
	{
		case kEventClassKeyboard:	break;
		case kEventClassMouse:		break;
		case kEventClassCommand:
		{
			switch( eventKind ) 
			{
				case kEventProcessCommand:
				{
					if( theApp->HandleProcessCommandEvent( inEvent ))
						outResult = noErr;
					/*
					result = ::GetEventParameter( event, kEventParamDirectObject, kEventParamHICommand, 
															NULL, sizeof(command), NULL, &command ); // get command
					if( theApp->HandleCommand( command ))
						outResult = noErr;
					*/
					break;
				}				
				//case kEventWindowClose:		cout << "app kEventClassCommand: kEventWindowClose" << endl; break;
				//case kEventWindowClosed:	cout << "app kEventClassCommand: kEventWindowClosed" << endl; break;
			}
			break;
		}
		
		case kEventClassMenu:	
		{	
			switch( eventKind ) 
			{
				case kEventMenuBeginTracking:	break;
				case kEventMenuOpening:
				{
				    MenuRef menuHitRef = NULL;
				    Boolean firstOpen = false;
				    
				    result = ::GetEventParameter( inEvent, kEventParamDirectObject, typeMenuRef,
				            					NULL, sizeof( ControlRef ), NULL, (void *)&menuHitRef );
				    
				    result = ::GetEventParameter( inEvent, kEventParamMenuFirstOpen, typeBoolean,
				                    			NULL, sizeof( Boolean ), NULL, (void *)&firstOpen );
				    
				    if( firstOpen )
				    	theApp->UpdateMenuState( menuHitRef );
					break;
				}
			}
			break;
		}	
				 				
//		case kEventClassAppleEvent:
//			break;

		/*case kEventClassWindow:
		{
			if( eventKind == kEventWindowClose )
				cout << "app kEventClassWindow: kEventWindowClose" << endl;
			else if( eventKind == kEventWindowClosed )
				cout << "app kEventClassWindow: kEventWindowClosed" << endl;
			break;
		}*/
	}
    return outResult;
}

// ----------------------------------------------------------------------------
// 		* GuidoWindowEventHandler
// ----------------------------------------------------------------------------
// This handler is attached to each guido window.
pascal OSStatus GuidoWindowEventHandler( EventHandlerCallRef inCallRef, EventRef inEvent, void * inUserData )
{
	GuidoQuartzViewerApp * theApp = GuidoQuartzViewerApp::sTheApp;
	GuidoDocWindow * docWindow = (GuidoDocWindow *)inUserData;

	return theApp->HandleWindowEvent( inEvent, docWindow );
}
