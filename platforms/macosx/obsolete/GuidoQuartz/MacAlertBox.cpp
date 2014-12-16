/*
 * file : MacAlertBox.cpp
 * 
 *	07/04/2004	(JB)	Nouvelle version, API epuree.
 *
 */

#include "MacAlertBox.h"
#include "MStrings.h"

// #include <Dialogs.h>

#include <iostream>	// DEBUG only

// ---------------------------------------------------------------------------------
//		* MacAlertBox
// ---------------------------------------------------------------------------------
MacAlertBox::MacAlertBox()
{
	mShowOK = true;	// MacOS always display the OK button, regardless of this flag !
	mShowCancel = false;
	mShowOther = false;
	mDefaultButton = kButtonOK;
	
	// ::SetDialogFont ( SInt16 fontNum ); 
}

// ---------------------------------------------------------------------------------
//		* ~MacAlertBox
// ---------------------------------------------------------------------------------
MacAlertBox::~MacAlertBox()
{
}

// ---------------------------------------------------------------------------------
//		* SetupButtons
// ---------------------------------------------------------------------------------
// The Ok button is always displayed !
void			
MacAlertBox::SetupButtons( /*bool showOk,*/ bool showCancel, 
								bool showOther, int inDefaultButtonID )
{
	mShowOK = true; // showOk;
	mShowCancel = showCancel;
	mShowOther = showOther;
	mDefaultButton = inDefaultButtonID;
}

// ---------------------------------------------------------------------------------
//		* SetButtonText
// ---------------------------------------------------------------------------------
void			
MacAlertBox::SetButtonText( int inButtonID, const char * inString )
{
	switch( inButtonID )
	{
		case kButtonOK:		mOKText = inString;			break;
		case kButtonCancel:	mCancelText = inString;		break;
		case kButtonOther:	mOtherText = inString;		break;
	}
}

// ---------------------------------------------------------------------------------
//		* Alert
// ---------------------------------------------------------------------------------
// The Ok button is always displayed !
// Important: the default valid (return key) button and the default 
// cancel (esc key) button can't be the same. (if not, StandardAlert will
// return -50)
int			
MacAlertBox::Alert( const char * inError, const char * inInfo )
{
	// - Preparation
	SInt16 defaultButton;
	SInt16 cancelButton;
	switch( mDefaultButton )
	{
		case kButtonCancel:	
			defaultButton = kAlertStdAlertCancelButton;	
			cancelButton = 0;	
			break;
		
		case kButtonOther:	
			defaultButton = kAlertStdAlertOtherButton;	
			cancelButton = kAlertStdAlertCancelButton;	
			break;

		case kButtonOK:	
		default:
			defaultButton = kAlertStdAlertOKButton;
			cancelButton = kAlertStdAlertCancelButton;
			break;
	}
	
	// - Bouton OK
	unsigned char okString [ 64 ];
	unsigned char * okStringPtr = okString;				// -1 -> defaut  0 -> invisible
	if( mShowOK == false )
		okStringPtr = NULL;
	else if( mOKText.size() == 0 )
		okStringPtr = (unsigned char *)-1;
	else
		MStrings::CToPascalString( mOKText.c_str(), okString, 63 );
	
	// - Bouton Cancel
	unsigned char cancelString [ 64 ];
	unsigned char * cancelStringPtr = cancelString;		// -1 -> defaut  0 -> invisible
	if( mShowCancel == false )
		cancelStringPtr = NULL;
	else if( mCancelText.size() == 0 )
		cancelStringPtr = (unsigned char *)-1;
	else
		MStrings::CToPascalString( mCancelText.c_str(), cancelString, 63 );
	
	// - Bouton Other
	unsigned char otherString [ 64 ];
	unsigned char * otherStringPtr = otherString;	// -1 -> defaut  0 -> invisible
	if( mShowOther == false )
		otherStringPtr = NULL;
	else if( mOtherText.size() == 0 )
		otherStringPtr = (unsigned char *)-1;
	else
		MStrings::CToPascalString( mOtherText.c_str(), otherString, 63 );
	
	// - Prepare le Record
	AlertStdAlertParamRec rec;
    rec.movable = true;
    rec.helpButton = false; 
    rec.filterProc = 0; 
    rec.defaultText = okStringPtr; 
    rec.cancelText = cancelStringPtr; 
    rec.otherText = otherStringPtr; 
    rec.defaultButton = defaultButton; 
    rec.cancelButton = cancelButton; 
    rec.position = kWindowDefaultPosition;
	
	unsigned char errorString [ 256 ];
	unsigned char infoString [ 256 ];
	
	MStrings::CToPascalString( inError, errorString, 255 );
	MStrings::CToPascalString( inInfo, infoString, 255 );
	
	// - Affichage
   	SInt16 result = kResultError;
   	SInt16 outItemHit = 0;
	OSErr err = ::StandardAlert( kAlertNoteAlert, errorString, infoString, &rec, &outItemHit );
	if( err == noErr )
	{
		switch( outItemHit )
		{
			case kAlertStdAlertOKButton: 		result = kButtonOK;			break;
			case kAlertStdAlertCancelButton: 	result = kButtonCancel;		break;
			case kAlertStdAlertOtherButton: 	result = kButtonOther;		break;
			case kAlertStdAlertHelpButton: 		result = kResultError;		break;
		}
	}
	else
		std::cout << "Error: StandardAlert() returned " << err << std::endl; 
	
	return result;
}







