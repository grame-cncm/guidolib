/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2003, 2004  Grame

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/


#include "MyHeaders.h"

#include "GUIDOEngine.h"
#include "GUIDOInternal.h"

extern int gCanDeinit;	
HINSTANCE gInstance = 0;

// --------------------------------------------------------------
//		DLLMain entry point for Microsoft Windows platforms
// --------------------------------------------------------------

int __stdcall DllMain(HINSTANCE hInstance, DWORD fdwReason,
					PVOID pvReserverd)
{
	gInstance = hInstance;

	if (fdwReason == DLL_PROCESS_ATTACH 
		|| fdwReason == DLL_THREAD_ATTACH)
	{
		// guido_cleanup();	useless	
	}
	else if (fdwReason == DLL_PROCESS_DETACH
		|| fdwReason == DLL_THREAD_DETACH)
	{
		guido_cleanup();
	//	guido_deinit();		//(JB) added
	}

	return TRUE;
}



