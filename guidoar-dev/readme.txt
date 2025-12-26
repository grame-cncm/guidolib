----------------------------------------------------------------------

      GuidoAr library

      A library providing a simple memory representation and a consistent
      way to browse and transform Guido Music Notation scores. 
      It provides also a set of score level manipulation operations 
      (like putting scores in sequence, in parallel, stretching etc.).

      Copyright 2009-2016 (c) Grame 

----------------------------------------------------------------------

======================================================================
1) Compiling the GuidoAR library
----------------------------------------------------------------------
GuidoAR relies on CMake, a cross-platform, open-source build 
system ( see http://www.cmake.org/).
The build folder contains the project description and is used to generate 
native projects. 
	
You should run 'make' from the 'build' folder.
Your target platform is inferred from the Makefile.

OPTIONS
To embed Midi file export in the library, you should call 'make' with MIDIEXPORT='yes | no'
as argument.
		
Note about MIDI export:
-------------------------------
    MIDI export requires libmidisharelight. For MacOS and Windows, the library is embedded 
    in binary form in the src/midisharelight folder. Thus there is no additional step.
    On linux, you must get the library source code, compile and install.

Supporting MIDI export on linux:
-------------------------------
   you must get the midishare source code that includes the midisharelight library:
	    git://git.code.sf.net/p/midishare/code 
   You don't need to compile midishare but only the midisharelight library.
   midisharelight is a recent addition to the project and for the moment, it is only
   available from the 'dev' branch. It is located at the project root folder.
   midisharelight is cmake based:
	> cd midisharelight/cmake
	> cmake -G "Unix Makefiles"
	> make
	> sudo make install

Note for Windows platforms:
-------------------------------
	MSYS is recommended to get a unix like shell and commands (like make)
	see at http://www.mingw.org/wiki/msys
	You should use Visual Studio to compile. If you have Visual Studio 12 2013 or
	Visual Studio 10 installed, the makefile should detect your version and compile correctly.
	To use another version, you'll have to override the MSVC and WIN32VS vairables.
	See the corresponding section in the Makefile. 

======================================================================
In case of trouble, contact me: <fober@grame.fr>
----------------------------------------------------------------------
Copyright 2009-2016 (c) Grame 
======================================================================
