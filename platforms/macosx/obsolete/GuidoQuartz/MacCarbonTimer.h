/*
 * file : MacCarbonTimer.h
 * 
 *	JB	05/2003
 *
 */

#ifndef MacCarbonTimer_H
#define MacCarbonTimer_H

#include <Carbon/Carbon.h>

// ---------------------------------------------------------------------------
class MacCarbonTimer
{	
	public:
								MacCarbonTimer();
		virtual					~MacCarbonTimer();

		virtual void			SetupTimes( double inDelay, double inInterval );	// seconds
//		virtual void			SetUserInfos( void * inInfos ) { mUserInfo = inInfos; }

		virtual void			StartTimer();
		virtual void			StartTimer( double inDelay, double inInterval );
		virtual void			StopTimer();
					
		virtual void			Fire() = 0;			
								
	protected:
		
		EventLoopTimerUPP		mTimerUPP;
		EventLoopTimerRef 		mTimerRef;
//		void * 					mUserInfo;
		
		EventTimerInterval		mFireDelay;
		EventTimerInterval		mTimeInterval;
		
	//	EventLoopTimerProcPtr	mUserRoutine;
		
};

#endif
