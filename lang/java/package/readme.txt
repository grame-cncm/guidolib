============================================================================
 GUIDO Engine Library
----------------------------------------------------------------------------
 JNI Interface
============================================================================
 Copyright Sony-CSL Paris (c) 2010, Grame (c) 2011-2017
============================================================================

The GUIDO Engine library provides music score layout to client applications.
It is based on the GUIDO format and support Guido Music Notation files (gmn)
as input but allows to build dynamic scores as well.

The JNI library provides a Java access to the engine C/C++ API. 
Apart encapsulation in java classes, the java API is similar to the C/C++ API. 
The Guido JNI requires the Guido Engine version 1.40 or later.

The JNI library also supports the MusicXML format thru the libMusicXML library 
when this library is available (see http://libmusicxml.sourceforge.net).

The Java package provides native interfaces for Mac OS X and Windows under the 
form of shared libraries. These libraries depend on the shared GUIDOEngine 
library. You must install the GUIDOEngine library and  make sure that all the 
libraries are on the java library path before using the Guido JNI.
You must  also install the guido2.ttf music font.

Mac OS X: you should copy the the GUIDOEngine.framework into /Library/Frameworks
		  copy libjniGUIDOEngine.dylib to /usr/local/lib or /usr/lib  
Windows : the dlls can be put with your java application

This package includes sample java code:
 - guidoviewer.java : a simple viewer application
 - factorysample.java : shows how to use the guido factory API

For more information about Guido, see at http://guidolib.sourceforge.net
