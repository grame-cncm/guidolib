// ===========================================================================
//	GuidoPrefs.h
// ===========================================================================
#ifndef __GuidoPrefs__
#define __GuidoPrefs__

#include <Carbon/Carbon.h>
#include "GUIDOEngine.h"

class GuidoQuartzViewerApp;
// ---------------------------------------------------------------------------
class GuidoPrefs
{
	GuidoLayoutSettings *	fPrefs;
	GuidoLayoutSettings		fSavedPrefs;
	WindowRef		fWindow;
	GuidoQuartzViewerApp * fAppl;
	EventHandlerUPP fEvtHandler;
	
	enum { 
		kSysDistance = 'dist',
		kMaxDistance = 'mdis',
		kSysDistribution = 'dstr',
		kOptPageFill = 'oppf',
		kNeighborhoodSpacing = 'nspa',
		kSpring = 'spri',
		kForce = 'forc'
	};
	
	enum { 
		kSysDistanceSlider = 1,		kSysDistanceText = 2,
		kMaxDistanceSlider = 3,		kMaxDistanceText = 4,
		kSysDistributionPopup = 5,
		kOptPageFillItem = 6,
		kNeighborhoodSpacingItem = 7,
		kSpringSlider = 8,		kSpringText = 9,
		kForceSlider = 10,		kForceText = 11
	};
	
		void	InitDialog (GuidoLayoutSettings * settings);
		void	SetCtrl (int id, long val, bool text=false);
		long	GetCtrl (int id);

	public:
				 GuidoPrefs (GuidoLayoutSettings* prefs) : fPrefs(prefs), fWindow(0) {}
		virtual ~GuidoPrefs() {}
		
		void		OpenDialog(IBNibRef ref, CFStringRef dlgname, GuidoQuartzViewerApp *app);
		OSStatus	HandleWindowEvent (EventRef event);
};

#endif