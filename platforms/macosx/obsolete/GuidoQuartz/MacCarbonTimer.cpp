/*
 * file : MacCarbonTimer.cp
 * 
 *	15-05-2003	JB	 Creation
 *
 * Utilisation:	Deriver une classe de MacCarbonTimer, qui implementer la methode Fire().
 *	 Fire() sera appelee pour chaque declenchement du timer.
 *
 */

#include "MacCarbonTimer.h"

// ---------------------------------------------------------------------------------
// Timer EventLoopTimerProcPtr callback
static pascal void MyMacCarbonTimerCB( EventLoopTimerRef /*inTimer*/, void * userData )
{
	MacCarbonTimer * carbonTimer = reinterpret_cast<MacCarbonTimer *>(userData);
	
	//if( carbonTimer )
		carbonTimer->Fire();
}

// ---------------------------------------------------------------------------------
//		* MacCarbonTimer
// ---------------------------------------------------------------------------------
MacCarbonTimer::MacCarbonTimer(/* EventLoopTimerProcPtr userRoutine */)
{
	// mUserRoutine = userRoutine;
	
	//  - Cleanup
	mTimerUPP = 0;
	mTimerRef = 0;
//	mUserInfo = 0;
	mFireDelay = 0;
	mTimeInterval = 0;
	
	// - Setup
	mTimerUPP = ::NewEventLoopTimerUPP( MyMacCarbonTimerCB );
}

// ---------------------------------------------------------------------------------
//		* ~MacCarbonTimer
// ---------------------------------------------------------------------------------
MacCarbonTimer::~MacCarbonTimer()
{
	StopTimer();

	if( mTimerUPP )
		::DisposeEventLoopTimerUPP( mTimerUPP );
	
	mTimerUPP = 0;
}

// ---------------------------------------------------------------------------------
//		* SetupTimes
// ---------------------------------------------------------------------------------
// inDelay, inInterval: seconds
void
MacCarbonTimer::SetupTimes( double inDelay, double inInterval )
{
	mFireDelay = (EventTimerInterval)inDelay;
	mTimeInterval = (EventTimerInterval)inInterval;
}

// ---------------------------------------------------------------------------------
//		* StartTimer
// ---------------------------------------------------------------------------------
// inDelay, inInterval: seconds
void
MacCarbonTimer::StartTimer( double inDelay, double inInterval )
{
	SetupTimes( inDelay, inInterval );
	StartTimer();
}

// ---------------------------------------------------------------------------------
//		* StartTimer
// ---------------------------------------------------------------------------------
void
MacCarbonTimer::StartTimer()
{
	if( mTimerRef )
		StopTimer();
		
	void * userInfos = this;
	
	::InstallEventLoopTimer( ::GetCurrentEventLoop(), mFireDelay, mTimeInterval, 
												mTimerUPP, userInfos, &mTimerRef );
}

// ---------------------------------------------------------------------------------
//		* StopTimer
// ---------------------------------------------------------------------------------
void
MacCarbonTimer::StopTimer()
{
	if( mTimerRef )
	{
		::RemoveEventLoopTimer( mTimerRef );
		mTimerRef = 0;
	}
}













