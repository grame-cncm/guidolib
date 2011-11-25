
#include <iostream>
#include "GuidoPrefs.h"
#include "GuidoQuartzViewerApp.h"

using namespace std;

// --------------------------------------------------------------------------------------------
pascal OSStatus PrefsWindowEventHandler( EventHandlerCallRef inCallRef, EventRef inEvent, void * inUserData )
{
	GuidoPrefs * prefs = (GuidoPrefs *)inUserData;
	return prefs->HandleWindowEvent( inEvent );
}

// --------------------------------------------------------------------------------------------
OSStatus GuidoPrefs::HandleWindowEvent (EventRef event)
{
	const UInt32 eventKind = ::GetEventKind( event );
	const UInt32 eventClass	= ::GetEventClass( event );
	long ctrlval;

	switch ( eventClass )
	{
		case  kEventClassCommand:
			if( eventKind == kEventProcessCommand )	{
				HICommand command;
				::GetEventParameter( event, kEventParamDirectObject, kEventParamHICommand, 
															NULL, sizeof(command), NULL, &command ); 
				switch( command.commandID ) {
					case kHICommandOK:
						::HideWindow (fWindow);
						QuitAppModalLoopForWindow(fWindow);
						break;

					case kHICommandCancel:
						::HideWindow (fWindow);
						*fPrefs = fSavedPrefs;
						QuitAppModalLoopForWindow(fWindow);
						break;

					case kHICommandClear:
						GuidoGetDefaultLayoutSettings (fPrefs);
						InitDialog (fPrefs);
						fAppl->UpdateAllGRs();
						break;

					case kSysDistance:
						fPrefs->systemsDistance = ctrlval = GetCtrl(kSysDistanceSlider);
						SetCtrl (kSysDistanceText, ctrlval, true);
						fAppl->UpdateAllGRs();
						break;

					case kMaxDistance:
						fPrefs->systemsDistribLimit = ctrlval = GetCtrl(kMaxDistanceSlider);
						SetCtrl (kMaxDistanceText, ctrlval, true);
						fPrefs->systemsDistribLimit = ctrlval / 100.0;
						fAppl->UpdateAllGRs();
						break;

					case kSysDistribution:
						fPrefs->systemsDistribution = GetCtrl(kSysDistributionPopup);
						fAppl->UpdateAllGRs();
						break;

					case kOptPageFill:
						fPrefs->optimalPageFill = GetCtrl(kOptPageFillItem);
						fAppl->UpdateAllGRs();
						break;

					case kNeighborhoodSpacing:
						fPrefs->neighborhoodSpacing = GetCtrl(kNeighborhoodSpacingItem);
						fAppl->UpdateAllGRs();
						break;

					case kSpring:
						fPrefs->spring = ctrlval = GetCtrl(kSpringSlider);
						SetCtrl (kSpringText, ctrlval, true);
						fAppl->UpdateAllGRs();
						break;

					case kForce:
						fPrefs->force = ctrlval = GetCtrl(kForceSlider);
						SetCtrl (kForceText, ctrlval, true);
						fAppl->UpdateAllGRs();
						break;
				}
			}
	}    
    return eventNotHandledErr;
}

// --------------------------------------------------------------------------------------------
long GuidoPrefs::GetCtrl (int ctrlid) 
{
	ControlRef ctrl; ControlID id = { 0, 0 };
	id.id = ctrlid;
	OSStatus err = GetControlByID (fWindow, &id, &ctrl);
	return (err == noErr) ? GetControlValue (ctrl) : 0; 
}

// --------------------------------------------------------------------------------------------
void GuidoPrefs::SetCtrl (int ctrlid, long val, bool text) 
{
	ControlRef ctrl; ControlID id = { 0, 0 };

	id.id = ctrlid;
	OSStatus err = GetControlByID (fWindow, &id, &ctrl);
	if (err == noErr) {
		if (!text)
			SetControlValue (ctrl, val); 
		else {
			char str[20];
			sprintf (str, "%ld", val);
			SetControlData (ctrl, kControlEntireControl, kControlEditTextTextTag, strlen (str), str);
		}
	}
	else cerr << "GuidoPrefs::SetCtrl err " << err << endl;
}

// --------------------------------------------------------------------------------------------
void GuidoPrefs::InitDialog (GuidoLayoutSettings * settings) 
{
	int val = (int)settings->systemsDistance;
	SetCtrl (kSysDistanceSlider, val);
	SetCtrl (kSysDistanceText, val, true);
	val = (int)(settings->systemsDistribLimit * 100);
	SetCtrl (kMaxDistanceSlider, val);
	SetCtrl (kMaxDistanceText, val, true);

	SetCtrl (kSysDistributionPopup, settings->systemsDistribution);
	SetCtrl (kOptPageFillItem, settings->optimalPageFill);
	SetCtrl (kNeighborhoodSpacingItem, settings->neighborhoodSpacing);

	val = (int)settings->spring;
	SetCtrl (kSpringSlider, val);
	SetCtrl (kSpringText, val, true);
	val = (int)settings->force;
	SetCtrl (kForceSlider, val);
	SetCtrl (kForceText, val, true);
}

// --------------------------------------------------------------------------------------------
void GuidoPrefs::OpenDialog (IBNibRef ref, CFStringRef dlgname, GuidoQuartzViewerApp *app)
{
	if (!fWindow) {
		OSErr err	= ::CreateWindowFromNib( ref, dlgname, &fWindow );
		if ( err != noErr )	return;

		const EventTypeSpec	windowEvents[] = { { kEventClassCommand,  	kEventProcessCommand } };
		void * userData = this;
		EventTargetRef targetRef = ::GetWindowEventTarget( fWindow );
		fEvtHandler = ::NewEventHandlerUPP( PrefsWindowEventHandler );
		::InstallEventHandler( targetRef, fEvtHandler,
								GetEventTypeCount( windowEvents ), windowEvents, userData, NULL );
	}
	fAppl = app;
	fSavedPrefs = *fPrefs;
	InitDialog (fPrefs);
	::ShowWindow( fWindow );
	RunAppModalLoopForWindow (fWindow);
}

