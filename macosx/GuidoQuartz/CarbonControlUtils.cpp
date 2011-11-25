/*
 *
 * File: CarbonControlUtils.cpp
 *
 *
 *
 */

#include <Carbon/Carbon.h>
#include "MacTools.h" // was  "CarbonControlUtils.h"

// ---------------------------------------------------------------------------
//		* FindControl
// ---------------------------------------------------------------------------
ControlRef	
MacTools::FindControl( WindowRef inWinRef, const ControlID & inControlID )
{
	ControlRef outControlRef = 0;
	::GetControlByID( inWinRef, &inControlID, &outControlRef );
	return outControlRef;
}

// ---------------------------------------------------------------------------
//		* FindControl
// ---------------------------------------------------------------------------
ControlRef	
MacTools::FindControl( WindowRef inWinRef, OSType inSignature, SInt32 inID )
{
	ControlID	controlID;
	controlID.id		= inID;
	controlID.signature	= inSignature;

	return FindControl( inWinRef, controlID );
}

// ---------------------------------------------------------------------------
//		* GetEditTextIntValue
// ---------------------------------------------------------------------------
SInt32	
MacTools::GetEditTextIntValue( ControlRef ctrl )
{
	const ControlPartCode partCode = kControlEditTextPart;
	const ResType tagName = kControlEditTextCFStringTag;
	
	Size actualSize = 0;
	CFStringRef text = 0;

	::GetControlData( ctrl, partCode, tagName, sizeof(CFStringRef), &text, &actualSize );
	
	const SInt32 value = ::CFStringGetIntValue( text );
	::CFRelease( text );
	return value;
}

// ---------------------------------------------------------------------------
//		* SetEditTextIntValue
// ---------------------------------------------------------------------------
void	
MacTools::SetEditTextIntValue( ControlRef ctrl, int inValue )
{
	const ControlPartCode partCode = kControlEditTextPart;
	const ResType tagName = kControlEditTextCFStringTag;

	CFStringRef text = ::CFStringCreateWithFormat( kCFAllocatorDefault, NULL, CFSTR("%d"), inValue );

	if( text )
	{
		::SetControlData( ctrl, partCode, tagName, sizeof(CFStringRef), &text );
 		::CFRelease( text );
	}
}


// ---------------------------------------------------------------------------
//		* SetControlString
// ---------------------------------------------------------------------------
void	
MacTools::SetControlString( WindowRef inWinRef, OSType inSignature, 
											SInt32 inID, CFStringRef inStringRef, bool refresh )
{
	const ControlRef theControlRef = FindControl( inWinRef, inSignature, inID );
	SetControlString( theControlRef, inStringRef, refresh );
}

// ---------------------------------------------------------------------------
//		* SetControlString
// ---------------------------------------------------------------------------
void	
MacTools::SetControlString( ControlRef inControlRef, CFStringRef inStringRef, bool refresh )
{
	if( inControlRef == 0 ) return;
	if( inStringRef == 0 ) return;

	::SetControlData( inControlRef, 0, kControlStaticTextCFStringTag, sizeof(CFStringRef), &inStringRef );
	
	if( refresh )
		::Draw1Control( inControlRef );
}
