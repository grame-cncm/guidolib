/*
 * file : MacFileSelector.cpp
 * 
 *	 Uses Apple Carbon "navigation services" to display standard file 
 *		selector dialogs
 *
 *
 *	 Light version !
 *
 *
 */
 
// #include <Navigation.h>

#include "MacFileSelector.h"
#include "MacTools.h"

//#include "MacStringTools.h"
// ---------------------------------------------------------------------------
//		* MyNavObjectFilterCB		
// ---------------------------------------------------------------------------
// Renvoie true (afficher l'item), par defaut.
pascal Boolean 
MacFileSelector_FilterCB( AEDesc * /*theItem*/, void * /*info*/, void * callBackUD, 
    													NavFilterModes /*filterMode*/ )
{
	MacFileSelector * mfs = (MacFileSelector *)callBackUD;
	if( mfs == nil )	return true;
	
//	FileFilter * filter = mfs->GetFileFilter();
//	if( filter == nil ) return true;
	
	/* .. a finir: extraire les infos de AEDesc et filtrer en fonction ... */

	return true;
}

pascal void 
MacFileSelector_EventCB( NavEventCallbackMessage selector, NavCBRecPtr params, void * callBackUD )
{
	MacFileSelector * mfs = (MacFileSelector *)callBackUD;
	if( mfs )
		mfs->HandleNavEvent( selector, params );
}

#pragma mark -
// ---------------------------------------------------------------------------
//		* MacFileSelector			
// ---------------------------------------------------------------------------
MacFileSelector::MacFileSelector()
{
	// - cleanup
	mMacDialogRef = 0;
	mEventProc = nil;
	mFilterProc = nil;
	mQueryMessage = 0;
	mDefaultSaveFileName = 0;
	mParentWindow = 0;
	
	// - Init
	mEventProc = ::NewNavEventUPP( MacFileSelector_EventCB );
	mFilterProc = ::NewNavObjectFilterUPP( MacFileSelector_FilterCB );
}

// ---------------------------------------------------------------------------
//		* ~MacFileSelector			
// ---------------------------------------------------------------------------
MacFileSelector::~MacFileSelector()
{
	if( mEventProc ) ::DisposeNavEventUPP( mEventProc );
	if( mFilterProc ) ::DisposeNavObjectFilterUPP( mFilterProc );

	if( mQueryMessage )			::CFRelease( mQueryMessage );
	if( mDefaultSaveFileName )	::CFRelease( mDefaultSaveFileName );
}

// ---------------------------------------------------------------------------
//		* SetQueryMessage			
// ---------------------------------------------------------------------------
void			
MacFileSelector::SetQueryMessage( const char * inMessage )
{
	SetCFString( mQueryMessage, inMessage );
}

// ---------------------------------------------------------------------------
//		* SetDefaultSaveFileName			
// ---------------------------------------------------------------------------
void			
MacFileSelector::SetDefaultSaveFileName( const char * inMessage )
{
	SetCFString( mDefaultSaveFileName, inMessage );
}

// ---------------------------------------------------------------------------
//		* SetDefaultSaveFileName			
// ---------------------------------------------------------------------------
void			
MacFileSelector::SetDefaultSaveFileName( CFStringRef inName )
{
	if( mDefaultSaveFileName ) ::CFRelease( mDefaultSaveFileName );
	
	if( inName )
		::CFRetain( inName );
	
	mDefaultSaveFileName = inName;
}
	
// ---------------------------------------------------------------------------
//		* SetCFString			
// ---------------------------------------------------------------------------
void			
MacFileSelector::SetCFString( CFStringRef & outRef, const char * inMessage )
{
	if( outRef )
		::CFRelease( outRef );

	outRef = 0;
	
	if( inMessage )
		outRef = ::CFStringCreateWithCString( 0, inMessage,  kCFStringEncodingUTF8 );
						// kCFStringEncodingASCII );
}

// ---------------------------------------------------------------------------
//		* SelectOneFile			
// ---------------------------------------------------------------------------
bool
MacFileSelector::SelectOneFile( FSRef * outPath )
{
	if( outPath == 0 ) return false;
	bool result = false;

	// - Initialisations du dialog
	NavDialogCreationOptions 	options;
	MacSetupDialogOptions( &options );

	NavTypeListHandle 			list = nil;
	NavEventUPP 				eventProc = mEventProc;
    NavPreviewUPP 				previewProc = nil; 
    NavObjectFilterUPP 			filterProc = mFilterProc;
    void *						callbackUD = this; 
    NavDialogRef *				outDialog = &mMacDialogRef;
	
	OSStatus status = ::NavCreateChooseFileDialog( &options, list, eventProc, previewProc,
												filterProc, callbackUD, outDialog );
	if( status != noErr ) return false;
		
	// - Ouverture du dialog 
	status = ::NavDialogRun( mMacDialogRef );
	if( status == noErr )
	{
		// - Traitement du resultat 
		NavUserAction userAction = ::NavDialogGetUserAction( mMacDialogRef );
		NavReplyRecord replyRec;
		switch( userAction )
		{
			case kNavUserActionChoose:
			case kNavUserActionOpen:
			{
				status = ::NavDialogGetReply( mMacDialogRef, &replyRec );
	 			if( status == noErr )
	 			{
	 				if( replyRec.validRecord )
	 				{
	 					// Extraction de n "AEDesc" a partir d'une structure "AEDescList"
	 					SInt32 itemCount = 0;
	 					OSErr err = ::AECountItems( &replyRec.selection, &itemCount );
	 					if( err == noErr )
	 					{
	 						AEKeyword keyWord;
	 						AEDesc resultDesc;
 						 	err = ::AEGetNthDesc( &replyRec.selection, 1,
	     										typeWildCard /*typeFSRef*/, &keyWord, &resultDesc );
   							if( err == noErr )
   							{
  								if( MacTools::AEDescToFSRef( &resultDesc, outPath ))
     								result = true;

	   						 	::AEDisposeDesc( &resultDesc );
	 						}
	 					}
	 				}
 					::NavDisposeReply( &replyRec );
	 			} 			
				break;
			}
			case kNavUserActionNone:		break;
			case kNavUserActionNewFolder:	break;
			case kNavUserActionCancel:		break;
		}
	}	
	::NavDialogDispose( mMacDialogRef );
	return result;
}

// ---------------------------------------------------------------------------
//		* SelectSaveFile			
// ---------------------------------------------------------------------------
bool
MacFileSelector::SelectSaveFile( CFURLRef * outPath )
{
	if( outPath == 0 ) return false;

	bool result = false;
	*outPath = 0;

	// - Initialisations du dialog
	NavDialogCreationOptions options;
	MacSetupDialogOptions( &options );

	OSType 			fileType = kNavGenericSignature;
	OSType 			fileCreator = kNavGenericSignature;
	NavEventUPP 	eventProc = mEventProc;
   	void *			callbackUD = this; 
    NavDialogRef *	outDialog = &mMacDialogRef;

	OSStatus status = ::NavCreatePutFileDialog( &options, fileType, fileCreator, 
													eventProc, callbackUD, outDialog );
	if( status != noErr ) return false;

	// - Ouverture du dialog 
	status = ::NavDialogRun( mMacDialogRef );
	if( status == noErr )
	{
		// - Traitement du resultat 
		NavUserAction userAction = ::NavDialogGetUserAction( mMacDialogRef );
		NavReplyRecord replyRec;
		switch( userAction )
		{
			case kNavUserActionSaveAs:
			{
				status = ::NavDialogGetReply( mMacDialogRef, &replyRec );
	 			if( status == noErr )
	 			{
	 				if( replyRec.validRecord )
	 				{
	 					// Extraction de n "AEDesc" a partir d'une structure "AEDescList"
	 					SInt32 itemCount = 0;
	 					OSErr err = ::AECountItems( &replyRec.selection, &itemCount );
	 					if( err == noErr )
	 					{
	 						*outPath = NavReplyToCFURL( &replyRec );
	 						result = (*outPath != 0);
	 					}
	 				}
 					::NavDisposeReply( &replyRec );
	 			} 			
				break;
			}
			
			case kNavUserActionNone:		break;
			case kNavUserActionNewFolder:	break;
			case kNavUserActionCancel:		break;
		}
	}
	::NavDialogDispose( mMacDialogRef );
	return result;
}


// ---------------------------------------------------------------------------
//		* NavReplyToCFURL			
// ---------------------------------------------------------------------------
// Caller is then the owner of returned url.
CFURLRef 
MacFileSelector::NavReplyToCFURL( const NavReplyRecord * navReply ) 
{
    OSStatus err;
    FSRef parentFSRef;
    CFURLRef parentURLRef, fullURLRef;
    AEKeyword theAEKeyword;
    DescType typeCode;
    Size actualSize;

	// - Ensure locals are in a known state
    fullURLRef = NULL;

    // - Get the FSRef referring to the parent directory
    err = ::AEGetNthPtr( &navReply->selection, 1, typeFSRef,
       				 &theAEKeyword, &typeCode, &parentFSRef, sizeof(FSRef), &actualSize);

    if (err == noErr) 
    {
 		// - Convert the FSRef into a Core Foundation URL
        parentURLRef = ::CFURLCreateFromFSRef( NULL, &parentFSRef );
        if (parentURLRef != 0) 
        {
            // - Add the file name to the end of the url */
            fullURLRef = ::CFURLCreateCopyAppendingPathComponent( NULL, parentURLRef, 
				                //navReply->saveFileName, 
				                ::NavDialogGetSaveFileName( mMacDialogRef ),
				                false );

            // - Release the path to the parent
            ::CFRelease(parentURLRef);
        }
    }
        /* return the reference to the new URL */
    return fullURLRef;
}


/*
// ---------------------------------------------------------------------------
//		* NavReplyToFSSpec			
// ---------------------------------------------------------------------------
bool	
MacFileSelector::NavReplyToFSSpec( NavReplyRecord * inReply, FSSpec * outSpecs )
{
	if( inReply == 0 ) return false;
	if( outSpecs == 0 ) return false;
		
	// (From Apple sample code: )
	AEKeyword keyWord;
	DescType  theType;
	Size theSize;

	OSErr err = ::AEGetNthPtr (&inReply->selection, 1, typeFSS, &keyWord, &theType, outSpecs, sizeof(FSSpec), &theSize);
	if( err != noErr ) 
		return false;
	
	bool result = false;
	
	if(( theType == typeFSS ) && (theSize == sizeof(FSSpec)))
	{
		// - Guard against potential Nav braindeadedness.
		if (outSpecs->name[0] == 0)
			err = FSMakeFSSpec( outSpecs->vRefNum, outSpecs->parID, outSpecs->name, outSpecs );	
		
		result = (err == noErr);
	}
	else if( type == typeFSRef ) && (theSize == sizeof(FSSpec)))	
	{
		FSRef refDesc;
		err = ::AEGetDescData( inDesc, &refDesc, sizeof(FSRef));
		if( err == noErr )
			return FSRefToFSSpec( &refDesc, outSpecs );
	}


	return result;
}
*/

// ---------------------------------------------------------------------------
//		* MacSetupDialogOptions	
// ---------------------------------------------------------------------------
// Optional
void		
MacFileSelector::SetParentWindow( WindowRef in )
{
	mParentWindow = in;
} 

// ---------------------------------------------------------------------------
//		* MacSetupDialogOptions	
// ---------------------------------------------------------------------------
bool
MacFileSelector::MacSetupDialogOptions( NavDialogCreationOptions * options )
{
  	if( options == 0 ) return false;
  	 	
   	// note: if the parent window is specified, then we use a "window modality"
   	// for the dialog to appear as a sheet.
   	
   	OSStatus status = ::NavGetDefaultDialogCreationOptions( options );
 
    options->message = mQueryMessage; 
    options->saveFileName = mDefaultSaveFileName; 
    options->modality = (false/*mParentWindow != 0*/) ? kWindowModalityWindowModal : kWindowModalityAppModal; 
   	options->parentWindow = mParentWindow; 
	

	/*if( mQueryMessage.Length() > 0 )
	{
	}	*/

   	return ( status == noErr );
  /* 	
   	options.version 			= kNavDialogCreationOptionsVersion;
    options.optionFlags			= ?; 
    options.location			= { -1, -1 }; 
 //   options.location			= -1; 

    options.clientName			= 0; 
    options.windowTitle			= 0; 
    options.actionButtonLabel	= 0; 
    options.cancelButtonLabel	= 0; 
     
    options.preferenceKey		= 0; 
    options.popupExtension		= 0; 
    options.modality 			= kWindowModalityAppModal; 
    options.parentWindow		=	0; 
    */
    
}

// ---------------------------------------------------------------------------
//		* HandleNavEvent	
// ---------------------------------------------------------------------------
void			
MacFileSelector::HandleNavEvent( NavEventCallbackMessage selector, NavCBRecPtr params )
{
	/*	
	switch( selector )
	{
		case kNavCBEvent: break;
		case kNavCBCustomize: break;
		case kNavCBStart: break;
		case kNavCBTerminate: break;
		case kNavCBAdjustRect: 	break; 	// Sent when the dialog has been resized 
		
		case kNavCBNewLocation: break;
		case kNavCBShowDesktop: break;
		case kNavCBSelectEntry: break;
		case kNavCBPopupMenuSelect: break;
		case kNavCBAccept: break;
		case kNavCBCancel: break;
		case kNavCBAdjustPreview: break;
		case kNavCBUserAction: break;
		case kNavCBOpenSelection: break;
	}*/
}

// ---------------------------------------------------------------------------
//		* SelectManyFiles			
// ---------------------------------------------------------------------------
/* A terminer:
bool
MacFileSelector::SelectManyFiles( array de FilePath * ioPath )
{
	if( !ioPath ) return false;
	bool result = false;


	NavDialogCreationOptions 	options;
	NavTypeListHandle 			list = nil;
	NavEventUPP 				eventProc = nil;	// a etudier
    NavPreviewUPP 				previewProc = nil; 
    NavObjectFilterUPP 			filterProc = ::NewNavObjectFilterUPP( MacFileSelectorFilterCallback );
    void *						callbackUD = this; 
    NavDialogRef *				outDialog = &mMacDialogRef;
	
	MacSetupDialogOptions( &options );
	
	
	OSStatus status = ::NavCreateGetFileDialog( &options, list, eventProc, previewProc,
												filterProc, callbackUD, outDialog );
	if( status != noErr ) return false;
		
	status = ::NavDialogRun( mMacDialogRef );

	NavUserAction userAction = ::NavDialogGetUserAction( mMacDialogRef );
	NavReplyRecord replyRec;
	switch( userAction )
	{
		case kNavUserActionNone:
			break;
			
		case kNavUserActionOpen:
			status = ::NavDialogGetReply( mMacDialogRef, &replyRec );
 			if( status == noErr )
 			{
 				if( replyRec.validRecord )
 				{
 					// Extraction de n AEDesc a partir d'une structure AEDescList
 					SInt32 itemCount = 0;
 					OSErr err = ::AECountItems( &replyRec.selection, &itemCount );
 					if( err == noErr )
 					{
 						
 						AEKeyword keyWord;
 						AEDesc resultDesc;
 						for( Int16 item = 1; item <= itemCount; item ++ )
 						{
 						 	err = ::AEGetNthDesc( &replyRec.selection, item,
     										typeWildCard, &keyWord, &resultDesc );
   								 
   							if( err == noErr )
   							{
   								
   							 	::AEDisposeDesc( &resultDesc );
							}
 						}
 					
 					}
 				}
 			
 				::NavDisposeReply( &replyRec );
 				result = true;
 			} 			
			break;
		
		case kNavUserActionNewFolder:
			break;

		case kNavUserActionCancel:
			break;
	}
	
	
	
	::NavDialogDispose( mMacDialogRef );
	
	return result;
	
}
*/
/*
// ---------------------------------------------------------------------------
//		* SelectOneFolder			
// ---------------------------------------------------------------------------
bool
MacFileSelector::SelectOneFolder( FilePath * ioPath )
{
	if( !ioPath ) return false;
	bool result = false;

	// - Initialisations du dialog
	NavDialogCreationOptions 	options;
	MacSetupDialogOptions( &options );

	NavEventUPP 				eventProc = mEventProc;
    NavObjectFilterUPP 			filterProc = mFilterProc;
    void *						callbackUD = this; 
    NavDialogRef *				outDialog = &mMacDialogRef;
	
	OSStatus status = ::NavCreateChooseFolderDialog( &options, eventProc, filterProc, 
																callbackUD, outDialog );
	if( status != noErr ) return false;
		
	// - Ouverture du dialog 
	status = ::NavDialogRun( mMacDialogRef );
	if( status == noErr )
	{
		// - Traitement du resultat 
		NavUserAction userAction = ::NavDialogGetUserAction( mMacDialogRef );
		NavReplyRecord replyRec;
		switch( userAction )
		{
			case kNavUserActionNone:		break;
				
			case kNavUserActionChoose:
			case kNavUserActionOpen:
			{
				status = ::NavDialogGetReply( mMacDialogRef, &replyRec );
	 			if( status == noErr )
	 			{
	 				if( replyRec.validRecord )
	 				{
	 					// Extraction de n "AEDesc" a partir d'une structure "AEDescList"
	 					SInt32 itemCount = 0;
	 					OSErr err = ::AECountItems( &replyRec.selection, &itemCount );
	 					if( err == noErr )
	 					{
	 						if( itemCount > 1 )	// On ne veut qu'un dossier. 
	 							itemCount = 1;	// Au cas ou: NavCreateChooseFolderDialog ne permet
	 											// de selectionner qu'un dossier de toute facon.
	 						AEKeyword keyWord;
	 						AEDesc resultDesc;
 						 	err = ::AEGetNthDesc( &replyRec.selection, 1,
	     										typeWildCard, &keyWord, &resultDesc );
   							if( err == noErr )
   							{
   								FSSpec fileSpec;
   								if( MacFileTools::AEDescToFSSpec( &resultDesc, &fileSpec ))
   								{
   									// - Recupere le nom du repertoire (NavDialogGetReply ne 
   									//	le donne pas, sous MacOS 9)
   									if( fileSpec.name[0] == 0 )
   										err = ::FSMakeFSSpec( fileSpec.vRefNum, fileSpec.parID, nil, &fileSpec );
   									   									
   									if( ioPath->SetPath( &fileSpec ))
   										result = true;
								}
	   						 	::AEDisposeDesc( &resultDesc );
	 						}
	 					}
	 				}
 					::NavDisposeReply( &replyRec );
	 			} 			
				break;
			}
			
			case kNavUserActionNewFolder:
				break;

			case kNavUserActionCancel:
				break;
		}
	}	
	
	::NavDialogDispose( mMacDialogRef );
	return result;
}

// ---------------------------------------------------------------------------
//		* SelectOneVolume			
// ---------------------------------------------------------------------------
// A faire
bool
MacFileSelector::SelectOneVolume( FilePath * ioPath )
{
	if( !ioPath ) return false;
	
	
	return false;
}

*/



