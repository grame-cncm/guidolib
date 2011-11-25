/*
 * file : ExportImageDialog.cpp
 * 
 *
 */

#include <Quicktime/ImageCompression.h>

// - Toolkit
#include "MacTools.h"

//
#include "ExportImageDialog.h"

// ---------------------------------------------------------------------------------
//		* ExportImageDialog
// ---------------------------------------------------------------------------------
ExportImageDialog::ExportImageDialog() // DialogCaller * inCaller ) : MacDialog( inCaller )
{
	mParentWindow = 0;
	mDialogWindow = 0;
	mResultCmd = 0;
	mPageRatio = 1;
}

// ---------------------------------------------------------------------------------
//		* ~ExportImageDialog
// ---------------------------------------------------------------------------------
ExportImageDialog::~ExportImageDialog()
{
  	if( mDialogWindow )
   		::DisposeWindow( mDialogWindow );
 
 	mDialogWindow = 0;
}

// ---------------------------------------------------------------------------------
//		* Setup
// ---------------------------------------------------------------------------------
bool
ExportImageDialog::Setup( IBNibRef inNibRef, CFStringRef inNibSheetName,
								WindowRef inParentWindow, double inRatio )
{
	mParentWindow = inParentWindow;
	mPageRatio = inRatio;

	OSErr err = ::CreateWindowFromNib( inNibRef, inNibSheetName, &mDialogWindow );

   	if( err != noErr ) return false;
   	if( mDialogWindow == 0 ) return false;
   	 	
 	// - Get the available graphic exporters
 	BuildExporterList();
 	
 	// - 
 	InstallNumericKeyFilter( GetDialogControl( kExportImageControl_Width ));
	InstallNumericKeyFilter( GetDialogControl( kExportImageControl_Height ));
	 	
    // - Our events table for the sheet window
    const EventTypeSpec windowEvents [] =
    {
    	{ kEventClassControl, kEventControlHit },
    //	{ kEventClassCommand, kEventProcessCommand },
    //	{ kEventClassTextInput, kEventTextInputUnicodeForKeyEvent }, // <- ok fr edit fields

    	 
    	{ kEventClassWindow,  kEventWindowClose }
    };
    
//    WindowRef wind = 0;
    void * userData = this; //static_cast<ExportImageDialog *>(this);
    
   	err = ::InstallWindowEventHandler( mDialogWindow,
                                ::NewEventHandlerUPP( MyExportImageDialogSheetHandler ),
                                GetEventTypeCount( windowEvents ), windowEvents, userData, NULL );

	return true;
}

// ---------------------------------------------------------------------------------
//		* BuildExporterList
// ---------------------------------------------------------------------------------
int 
ExportImageDialog::BuildExporterList()
{
	mFormatList.clear();

	// - Find the image format popup menu.	
	ControlRef popupControl = GetDialogControl( kExportImageControl_Format );
	if( popupControl == 0 ) return 0;
	
	MenuRef menu = ::GetControlPopupMenuHandle( popupControl );
	::DeleteMenuItem ( menu, 1 );	// Empty the menu.

	const OSType myDefaultFormat = 'JPEG';
	int defaultItemIndex = 1;
	
	ComponentDescription desc;
	ComponentDescription temp;
	Component c = 0;
	
	desc.componentType = GraphicsExporterComponentType;
	desc.componentSubType = 0;
	desc.componentManufacturer = 0;
	desc.componentFlags = 0;
	desc.componentFlagsMask = 0; // graphicsExporterIsBaseExporter;
	
	const long compCount = ::CountComponents( &desc );
	int okConverterCount = 0;
	
	Handle pStringH = ::NewHandle(4);	// the component name
	
	for( long i = 0; i < compCount; ++i )
	{
		c = ::FindNextComponent( c, &desc );

		::GetComponentInfo( c, &temp, pStringH, NULL, NULL );
  		OSType formatCode = temp.componentSubType;
  		if( formatCode != 'base' )
		{
			::InsertMenuItem( menu, (unsigned char *)(*pStringH), ::CountMenuItems( menu ));
 			mFormatList.push_back( formatCode );
			
			if( formatCode == myDefaultFormat ) 
				defaultItemIndex = i + 1;

  			++ okConverterCount;
  		}
	}
	::DisposeHandle( pStringH ); // must be ?

	// - Adjust the popup value range.
	::SetControl32BitMinimum( popupControl, 1 );
	::SetControl32BitMaximum( popupControl, okConverterCount );
	::SetControl32BitValue( popupControl, defaultItemIndex );

	return compCount; // return the number of exporters.
}
 
// ---------------------------------------------------------------------------------
//		* DoDialog
// ---------------------------------------------------------------------------------
UInt32
ExportImageDialog::DoDialog()
{
	const WindowRef sheetRef = GetDialogWindow();
	const WindowRef parentRef = GetParentWindow();

	if( sheetRef == 0 ) return 0;
	
	OSStatus status = 0;

 	// - Setup default ok and cancel buttons
	ControlRef	okButton = GetDialogControl( kExportImageControl_Ok );
	ControlRef	cancelButton = GetDialogControl( kExportImageControl_Cancel );

	status = ::SetWindowDefaultButton( sheetRef, okButton );
	status = ::SetWindowCancelButton( sheetRef, cancelButton );

	status = ::ShowSheetWindow( sheetRef, parentRef );	
	
	// - Prepare
	
	mProportionnal = GetProportionnal();
	mPrevWidth = GetPixelWidth();
	mPrevHeight = GetPixelHeight();
	
//	double pageWidth = 800;		// TODO
//	double pageHeight = 600;	// TODO
//	mPageRatio = pageHeight / pageWidth; 
	
	// - Launch the main dialog loop. It will not return 
	//   until DialogFinished() is called.
	
	const double checkInterval = 0.100;
	StartTimer( checkInterval, checkInterval );
	
  	if( mProportionnal )
  		ApplyProportionnal( kExportImageControl_Width, kExportImageControl_Height, mPageRatio );

	status = ::RunAppModalLoopForWindow( sheetRef );

	StopTimer(); 
	Fire();



	// - The dialog has been finished
	::HideSheetWindow( sheetRef );

	return mResultCmd;
}

// ---------------------------------------------------------------------------------
//		* GetDialogControl
// ---------------------------------------------------------------------------------
ControlRef
ExportImageDialog::GetDialogControl( SInt32 inID ) const
{
	return MacTools::FindControl( GetDialogWindow(), 'Gido', inID );
}

// ---------------------------------------------------------------------------------
//		* EventCallback
// ---------------------------------------------------------------------------------
OSStatus
ExportImageDialog::EventCallback( EventRef inEvent )
{
	// cout << "event" << endl;

    ControlRef control = 0;
    UInt32 cmd = 0;
//    WindowRef sheet = 0;
//    WindowRef parent = 0;
    
    // get control hit by event
    ::GetEventParameter( inEvent, kEventParamDirectObject, typeControlRef, NULL,
    						sizeof(ControlRef), NULL, &control );
 
    // get the command for that control
    ::GetControlCommandID( control, &cmd );
    
   // sheet = ::GetControlOwner( control );
   // ::GetSheetWindowParent( sheet, &parent );
   
    
    switch(cmd)
    {
    /*	case 'Widt':
   			if( mProportionnal )
   				ApplyProportionnal( kExportImageControl_Width, kExportImageControl_Height, ratio );
    		break;

    	case 'Heig':
   			if( mProportionnal )
   				ApplyProportionnal( kExportImageControl_Height, kExportImageControl_Width , 1.0 / ratio );
    		break;
    */
   		case 'Prop':
   				// CallNextEventHandler() ?
   				mProportionnal = GetProportionnal();
   				if( mProportionnal )
   					ApplyProportionnal( kExportImageControl_Width, kExportImageControl_Height, mPageRatio );
   			break;
   		
   		case kHICommandCancel:
        case kHICommandOK:
      		DialogFinished( cmd );
          	break;
    }

	return noErr;
}

// ---------------------------------------------------------------------------------
//		* DialogFinished
// ---------------------------------------------------------------------------------
//  Called when the user has finished using the dialog (validate, cancel, close...)
void
ExportImageDialog::DialogFinished( UInt32 inCommand )
{
	mResultCmd = inCommand;
	::QuitAppModalLoopForWindow( GetDialogWindow());	
}

// ---------------------------------------------------------------------------------
//		* GetPixelWidth
// ---------------------------------------------------------------------------------
int
ExportImageDialog::GetPixelWidth() const
{
	const ControlRef editControl = GetDialogControl( kExportImageControl_Width );
	return MacTools::GetEditTextIntValue( editControl );
}

// ---------------------------------------------------------------------------------
//		* GetPixelHeight
// ---------------------------------------------------------------------------------
int
ExportImageDialog::GetPixelHeight() const
{
	const ControlRef editControl = GetDialogControl( kExportImageControl_Height );
	return MacTools::GetEditTextIntValue( editControl );
}

// ---------------------------------------------------------------------------------
//		* GetExportFormat
// ---------------------------------------------------------------------------------
OSType
ExportImageDialog::GetExportFormat() const
{
	const ControlRef formatPopup = GetDialogControl( kExportImageControl_Format );
	SInt32 value = ::GetControl32BitValue( formatPopup );
		
	return mFormatList.at( value - 1 );	// menu values are 1-based
}

// ---------------------------------------------------------------------------------
//		* ApplyProportionnal
// ---------------------------------------------------------------------------------
// dest = src * ratio,  ratio = dest / src
int
ExportImageDialog::ApplyProportionnal( SInt32 srcControl, SInt32 destControl, double inRatio )
{
	// - Calculate new value for destination control
	const ControlRef srcRef = GetDialogControl( srcControl );
	const ControlRef dstRef = GetDialogControl( destControl );
	
	const double srcValue = (double)MacTools::GetEditTextIntValue( srcRef );

	const double dstNewValue = srcValue * inRatio;
	
	// - Apply the new value for the destination control
	const int newValue = (int)(dstNewValue + 0.5);
	const int oldValue = MacTools::GetEditTextIntValue( dstRef );
	
	if( newValue != oldValue )
	{
		MacTools::SetEditTextIntValue( dstRef, newValue);
		::Draw1Control( dstRef );
	}
	
	return newValue;
}

// ---------------------------------------------------------------------------------
//		* GetProportionnal
// ---------------------------------------------------------------------------------
bool
ExportImageDialog::GetProportionnal()
{
	return (::GetControl32BitValue( GetDialogControl( kExportImageControl_Proportional )) == 1 ); 
}

// ---------------------------------------------------------------------------------
//		* Fire
// ---------------------------------------------------------------------------------
// ratio = height / width
void			
ExportImageDialog::Fire()
{
	if( mProportionnal )
	{		
		// - First get the new size
		const int newWidth = GetPixelWidth();
	   	const int newHeight = GetPixelHeight();
		
		// - Check if the width has changed...
		if( newWidth != mPrevWidth )
		{
	   		mPrevHeight = ApplyProportionnal( kExportImageControl_Width, kExportImageControl_Height, mPageRatio );
			mPrevWidth = newWidth;			
		}
		
		// - Check if the height has changed...
	   	else if( newHeight != mPrevHeight )
	   	{
	   		mPrevWidth = ApplyProportionnal( kExportImageControl_Height, kExportImageControl_Width , 1.0 / mPageRatio );
			mPrevHeight = newHeight;
		}

		// - Security check
		// else if(((double)newHeight) / ((double)newWidth) != mPageRatio )
		//	ApplyProportionnal( kExportImageControl_Width, kExportImageControl_Height, mPageRatio );
	}
}			

// ---------------------------------------------------------------------------------
// Carbon Event handler for our sheet/dialog window

pascal OSStatus 
MyExportImageDialogSheetHandler( EventHandlerCallRef inRef,
									EventRef inEvent,  void * userData )
{
  	ExportImageDialog * myDialog = static_cast<ExportImageDialog *>(userData);

    return myDialog->EventCallback( inEvent );
}
    


