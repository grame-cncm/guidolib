======================================================================
	GuidoEditor Copyright (c) 2009-2011 Grame
	GuidoSceneComposer Copyright (c) 2009-2011 Grame
======================================================================


======================================================================
	Introduction
======================================================================
GUIDO Music Notation is a text based, human readable format to represent 
music. It is intended to describe musical information as well as 
notational information. In GUIDO, simple music can be described in a 
simple way, whereas complex music sometimes requires more complex 
representations. 

This package contains applications that uses the GUIDOEngine.
These applications are built on top of the Qt framework. 
(see at http://qt.nokia.com/)

======================================================================
         GuidoEditor
======================================================================
GuidoEditor is a simple application to view, edit, export and print 
Guido Music Notation (.gmn) files.

======================================================================
         GuidoSceneComposer
======================================================================
GuidoSceneComposer is an application to write Guido Music Notation, and 
to see instantly the corresponding score. Moreover, it allows to compose 
a complex graphic scene with various scores, text annotations and images, 
and then export it.

======================================================================
         guido2image
======================================================================
guido2image 
	export GUIDO Music Notation to various image formats, including PDF 
	and svg.
	On MacOS, you should have the Qt libraries installed to use 
	guido2image. Note that the Qt libraries are included in the 
	GuidoEditor and GuidoSceneComposer bundles.

guido2svg
	export GUIDO Music Notation to svg.

guido2midi
	converts GUIDO Music Notation to MIDI file.

guidogetpagecount
	gives the page count of a GUIDO score.

guidogetvoicecount
	gives the voice count of a GUIDO score.

guidogetversion
	gives the current GUIDO engine version.

======================================================================
	Notes
======================================================================
The Guido Qt applications are part of the GuidoLib project. 
They demonstrate the use of the Guido Engine Library to render music 
scores using the Qt framework.

The GuidoLib Project is hosted on sourceforge.net project, download page:
	http://sourceforge.net/projects/guidolib/
home page:	
	http://guidolib.sourceforge.net/

If you want to get online music notation, visit the GUIDO NoteServer at
	http://www.noteserver.org

======================================================================
	Platform notes
======================================================================
Mac OS:
	The applications bundles are standalone: they embed all the required 
	libraries. The command line tools are expecting to find the 
	GUIDOEngine.framework in their folder, thus don't move them outside 
	the folder unless you also move the framework too.

Windows:
	You need to install Microsoft redistributable dlls using vcredist_sp1_x86.exe.
	You need to install the guido font : open the guido2.ttf file and click 'install'
	The Guido Qt folder is self contained : it includes all the libraries
	required to run the applications. Don't move the applications outside 
	the folder.


======================================================================
	Copyright and Disclaimer
======================================================================
This program is free software; you can redistribute it and/or
modify it under the terms of the Mozilla Public License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

======================================================================
Acknowledgments
======================================================================
The Guido project originates from Ph.D.s conducted by Holger Hoos, Kai Renz and 
Jürgen Kilian at the TU Darmstadt.
The Guido engine has been designed by Kai Renz. It became an open source library in 
December 2002 at the initiative of the Grame Computer Music Research Lab. Since then,
many persons have been contributing to the Guido library project.
I would like to thank them and especially: 
J. Scott Amort, Jérôme Berthet, Samuel Brochot, Yannick Chapuis, Christophe Daudin,
Colas Decron, Torben Hohn, Camille Le Roi, Stéphane Letz, François Levy, 
Benjamen Ruprechter, Mike Solomon

Dominique Fober
