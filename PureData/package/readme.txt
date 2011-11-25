============================================================================
 GUIDO Engine Library
----------------------------------------------------------------------------
 guido external for Pure Data
============================================================================
 Copyright Grame (c) 2010
============================================================================


The 'guido' external displays music scores.
It is based on the Guido Music Notation language and accepts litteral Guido 
code or Guido files as input. The score layout is achieved using the 
Guido Engine library.

In addition and when the libmusicxml2 library is available, it also supports 
the MusicXML format as file input.

You must install the Guido shared library and the guido2.ttf font installed
to use the 'guido' external.
Installation of the libmusicxml2 shared library is optional.

The 'guido' external is part of the Guido Engine open source project.
You can get the source code from the project repository 
(https://sourceforge.net/projects/guidolib/).

More information about Guido at http://guidolib.sourceforge.net
More information about the musicxml library at http://libmusicxml.sourceforge.net/

============================================================================
Limitations
============================================================================
The 'guido' external could become very slow in drawing the music score, 
depending on the drawing area size and on your platform (windows seems not to 
be very efficient). This is due to the way to give the score image to Tcl/Tk:
actually the score is rendered offscreen using native graphic devices (Cairo 
on Linux, CGContext on MacOS and GDIPlus on Windows). The resulting bitmap is
given to Tcl/Tk with a command string to set each pixel data. 
============================================================================
