#ifndef CWinVersion_H
#define CWinVersion_H

/*
 * File: CWinVersion.h		
 * Copyright (c) 2004  Grame
 *
 *	
 */

class CWinVersion
{
	public:

				CWinVersion();
		bool	WinNTOrGreater() const;

	private:

		OSVERSIONINFO mInfos;
};

// ----------------------------------------------------------------------------
CWinVersion::CWinVersion()
{
	::ZeroMemory( &mInfos, sizeof( OSVERSIONINFO ));
	mInfos.dwPlatformId = VER_PLATFORM_WIN32_NT;

	mInfos.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );

   BOOL result = ::GetVersionEx( &mInfos );
}
 
// ----------------------------------------------------------------------------
bool 
CWinVersion::WinNTOrGreater() const
{
	return ( mInfos.dwPlatformId >= VER_PLATFORM_WIN32_NT );
}


#endif