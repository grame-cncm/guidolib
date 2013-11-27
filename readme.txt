----------------------------------------------------------------------
Compiling the Guidolib project
----------------------------------------------------------------------

======================================================================
1) Compiling the GUIDOEngine
----------------------------------------------------------------------
The GUIDOEngine relies on CMake, a cross-platform, open-source build 
system ( see http://www.cmake.org/).
The cmake folder contains the project description and is used to generate 
native projects. 

To compile:
	change to cmake directory
	type:  cmake -G "your target generator" OPTIONS
	run your project/makefile and compile

OPTIONS indocates optional components and is between:
	-DMIDIEXPORT='yes | no' to support MIDI export
	
Note about MIDI export:
--------------------------
    MIDI export requires libmidisharelight. For MacOS and Windows, the library is embedded 
    in binary form in the src/midisharelight folder. Thus there is no additional step.
    On linux, you must get the library source code, compile and install.
	
Note for Android:
-------------------------
    Download the Android SDK and NDK.

    From the build/tools directory of the NDK, invoke make-standalone-toolchain.sh
    to make a standalone toolchain in the directory of your choice (see NDK)
    documentation for how.  More precisely, read section 4 in the document
    STANDALONE-TOOLCHAIN.html that ships with the NDK. Make sure to use a recent
    platform (at least android-18). I will call the path to your standalone toolchain
    $PATH_TO_STANDALONE_TOOLCHAIN, the path to the SDK $PATH_TO_SDK and the
    path to android cmake $PATH_TO_ANDROID_CMAKE.

    Get android-cmake: https://github.com/taka-no-me/android-cmake.
    This is the best version for now - get the least buggy one...

    Modify your path to contain the following.  Otherwise, the compiler
    won't find things like the standard library.

    export PATH=$PATH:$PATH_TO_NDK
    export PATH=$PATH:$PATH_TO_SDK/tools
    export PATH=$PATH:$PATH_TO_SDK/platform-tools
    export PATH=$PATH:$PATH_TO_STANDALONE_TOOLCHAIN/bin

    export ANDROID_NDK_TOOLCHAIN_ROOT=$PATH_TO_STANDALONE_TOOLCHAIN
    export ANDTOOLCHAIN=$PATH_TO_ANDROID_CMAKE/android.toolchain.cmake
    you may have to do instead...
    export ANDTOOLCHAIN=$PATH_TO_ANDROID_CMAKE/toolchain/android.toolchain.cmake

    depending on where you get android.toolchain.cmake from.

    There are also several variables you'll need to define in the scheme
    below so that cmake finds everything.  You'll need to compile pixman
    and cairo for android.  This probably require having a different set of
    include and lib flags than normal system ones. These need to be defined
    manually (see below).
    For example, you could define CAIRO_ANDROID_INCLUDE_FLAGS as :
    export CAIRO_ANDROID_INCLUDE_FLAGS="-I/usr/local/android-libs/cairo/cairo/include -I/usr/local/android-libs/cairo-extra/include"
    For example, you could define PIXMAN_ANDROID_INCLUDE_FLAGS as :
    export PIXMAN_ANDROID_INCLUDE_FLAGS="-I/usr/local/android-libs/pixman/pixman/include -I/usr/local/android-libs/pixman-extra/include"

    A project that makes cairo for android can be found at:
      https://github.com/anoek/android-cairo

    Then, in your build directory, make an android folder.  From this folder, call:
    cmake -DCMAKE_TOOLCHAIN_FILE=$ANDTOOLCHAIN \
    -DANDROID=1 \
    -DLIBRARY_OUTPUT_PATH_ROOT=../../android/jni/
    -DANDROID_CAIRO_CPP_FLAGS=$CAIRO_ANDROID_INCLUDE_FLAGS \
    -DANDROID_PIXMAN_CPP_FLAGS=$PIXMAN_ANDROID_INCLUDE_FLAGS \
    -DANDROID_CAIRO_LIBS=$CAIRO_LIB_FLAGS \
    -DANDROID_PIXMAN_LIBS=$PIXMAN_LIB_FLAGS \
    -DSTATICLIB=yes ../../cmake
    make

    It should result in something like:
    Linking CXX shared library /Users/mikesolomon/devel/guidolib-code/cmake/libs/armeabi-v7a/libGUIDOEngine.so
    obviously with your path...

    After this, make sure that all libraries are linked against this.
    For example, for anything Qt related, use this library in the .pro file.

Note for Linux platforms:
--------------------------
	You need to have libcairo2-dev installed to compile the GUIDOEngine.
	The procedure to compile is close to the usual 'configure' 'make' 'make install'
	steps. Actually, you can simply do the following:
	> cd /your_path_to_the_project/cmake
	> cmake -G "Unix Makefiles"
	> make
	> sudo make install

   Supporting MIDI export on linux:
   -------------------------------
   you must get the midishare source code:
	   git://midishare.git.sourceforge.net/gitroot/midishare/midishare 
   midisharelight is a recent addition to the project and for the moment, it is only
   available from the 'dev' branch. It is located at the project root folder.
   midisharelight is cmake based:
	> cd midisharelight/cmake
	> cmake -G "Unix Makefiles"
	> make
	> sudo make install


Note for Windows platforms:
--------------------------
	The CMake project description is "Visual Studio" oriented. 
	Using MingW may require some adaptation.


======================================================================
2) Compiling the GUIDO Qt applications
----------------------------------------------------------------------
You need to have Qt SDK version 4.6 or later installed to compile the 
GUIDO Qt applications.
(see at http://qt.nokia.com/)
You can use QTCreator on all platforms to compile the applications.
See below if you want to use another development environment.

>>>>>> Mac OS
----------------------------
To compile the GUIDO Qt applications do the following:
  > cd /your_path_to_the_guido_project/Qt
  > make projects
Next you should find Xcode projects into every application folder.
Just open the projects and compile.

>>>>>> Windows
----------------------------
To compile the GUIDO Qt applications, the easiest way is probably to use QTCreator.
However, if you want to use Visual Studio you should:
- set the QMAKESPEC variable to the corresponding output specification
  (see Qt documentation)
- generate each project in each application folder using 'qmake'
Note that a solution for Visual Studio 2005 is provided at the root of the Qt folder.
Warning, the solution works only after the individual projects have been generated.

>>>>>> Linux
----------------------------
Qt can be installed from synaptic (on Ubuntu)
To compile the GUIDO Qt applications do the following:
  > cd /your_path_to_the_guido_project/Qt
  > make unix

======================================================================
In case of trouble, contact me: <fober@grame.fr>
======================================================================
