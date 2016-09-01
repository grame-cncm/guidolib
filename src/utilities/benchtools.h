/*
  GUIDO Library
  Copyright (C) 2010 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef __benchtools__
#define __benchtools__

#include <iostream>
//#include <vector>
//#include <map>

#ifdef __x86_64__
#define __uint32	unsigned int
#define __uint64	unsigned long int

#define __int32	int
#define __int64	long int

#else

#ifndef __uint32
#define __uint32	unsigned int
#endif

#ifndef __uint64
#define __uint64	unsigned long long int
#endif

#ifndef __int32
#define __int32		int
#endif

#ifndef __int64
#define __int64		long long int
#endif
#endif

#ifndef WIN32
__uint64  rdtsc(void);
#else
__uint64 __cdecl rdtsc(void);
#endif
__uint64  getTime (void);

//_________________________________________________________________________________
/*
class bench
{
	public:
		static float	usec2ticks(void);
		static void		init(const char* sigs[], unsigned short size = 3000);
		static void		start();
		static void		stop();
		static bool		running();
		static void		put(const std::string& sig, float val);
		static void		write(const std::string& toFile);
};
*/

//#define RUNBENCH

#ifdef RUNBENCH
#define  timebench(name,f)				\
{										\
	__uint64 start = getTime();			\
	f;									\
	__uint64 elapsed = getTime() - start;	\
	std::cout << name << "\t" << elapsed << std::endl; \
}
#else
#define  timebench(name,f) f
#endif

#endif
