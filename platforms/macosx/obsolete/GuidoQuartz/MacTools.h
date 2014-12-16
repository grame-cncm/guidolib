/*
 * file : MacTools.h
 * 
 */

#ifndef MacTools_H
#define MacTools_H

#include <Carbon/Carbon.h>

namespace MacTools
{

// -- Files -- //

	bool 	CreateFile( CFURLRef inFileURL, FSRef * outFileRef = 0, FSSpec * outFileSpec  = 0 );

	bool 	AEDescToFSRef( const AEDesc * inDesc, FSRef * outRef );	

	void  	PrintURL( CFURLRef inURL );


// -- Strings -- //

		void 	PrintCFString( CFStringRef inStringRef );


// -- Carbon controls -- //

	// - Find a control in a window, from its control signature and id	
	ControlRef	FindControl( WindowRef inWinRef, const ControlID & inControlID );
	ControlRef	FindControl( WindowRef inWinRef, OSType inSignature, SInt32 inID );


	// - Set the text displayed by a control
	void SetControlString( WindowRef inWinRef, OSType inSignature, SInt32 inID, 
								CFStringRef inStringRef , bool refresh );


	void SetControlString( ControlRef inControlRef, CFStringRef inStringRef, bool refresh );

	// - Control values
	SInt32 	GetEditTextIntValue( ControlRef ctrl );
	void 	SetEditTextIntValue( ControlRef ctrl, int inValue );
	
	
} // namespace

#endif
