/*

*/
#ifndef MClock_H
#define MClock_H

// --------------------------------------------------------------
class MClock
{
	public:
						MClock();
		
		void			Reset() { mStartTime = clock(); }
		unsigned long	Seconds() { return (clock() - mStartTime) / CLOCKS_PER_SEC; }
		unsigned long	Milliseconds() { return (unsigned long)(( clock() - mStartTime ) * mClockToMilli ); }
		unsigned long	Microseconds() { return (unsigned long)(( clock() - mStartTime ) * mClockToMicro ); }

	private:
	
		unsigned long	mStartTime;
		
		double			mClockToMilli;
		double			mClockToMicro;
};

// --------------------------------------------------------------
inline
MClock::MClock() : mStartTime( 0 )
{
	mClockToMilli = 1000.0 / CLOCKS_PER_SEC;
	mClockToMicro = 1000000.0 / CLOCKS_PER_SEC;
	Reset();
}

#endif
