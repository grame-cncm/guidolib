--------------------------------------------------------------------------
                         GUIDO Engine Library
                            ==============
              Grame, Centre National de Creation Musicale
                          http://www.grame.fr
--------------------------------------------------------------------------

The GUIDO Engine library provides music score layout to client applications.
It is based on the GUIDO format and support Guido Music Notation files (gmn)
as input but allows to build dynamic scores as well.

The library is a C/C++ library supporting MacOS X, GNU-Linux and Windows. 
Portability is ensured by a set of Virtual Graphic Devices (VGDevice), 
implemented for each target platform. These devices are included in the 
binary libraries for MacOS X and Windows. 

The preferred solution for platform independence is currerntly to use the Qt 
environment included in this distribution; it provides Qt graphic devices as 
well as Qt widget especially designed to support guido graphic scores.

The GUIDO Engines uses the font 'guido2.ttf' (containing various musical 
symbols), so you should install this font on your system.

The source code of the library is available from sourceforge under the Mozilla 
Public License (see the license.txt file)
See at http://sourceforge.net/projects/guidolib for more information.


Acknowledgments
------------------
The Guido project originates from Ph.D.s conducted by Holger Hoos, Kai Renz and 
Jürgen Kilian at the TU Darmstadt.
The Guido engine has been designed by Kai Renz. It became an open source library in 
December 2002 at the initiative of the Grame Computer Music Research Lab. Since then,
many persons have been contributing to the Guido library project.
I would like to thank them and especially: 
J. Scott Amort, Jérôme Berthet, Samuel Brochot, Yannick Chapuis, Christophe Daudin,
Colas Decron, Torben Hohn, Camille Leroy, Stéphane Letz, François Levy, 
Benjamen Ruprechter, Mike Solomon

Dominique Fober
