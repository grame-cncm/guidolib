---- sibelius2guido.plg ------------------------------------
Sibelius plugin for exporting scores to GUIDO MusicNotation.
Version 0.40
Written 	9/2003 by 	Juergen Kilian (kilian@noteserver.org)
Redesigned	4/2011		D. Fober (fober@grame.fr)
---------------------------------------------------------------

Installation:
- 	copy sibelius2guido.plg into your Sibelius plugin folder.
 	The precise location for installing a plug-in varies depending on the version 
 	of Sibelius you are using and on your operating system.
 	see at http://www.sibelius.com/download/plugins/index.html?help=install

Usage:
- 	Open a score in Sibelius and call the plugin "Export to GUIDO"
- 	A dialog box opens where you need to specify the filename for the output file
- 	ATTENTION: If a file with that name already exists, it will be overridden
- 	Use the output file with the GUIDO NoteServer or NoteViewer or other GUIDO 
  	applications

=======================
 Changes log
=======================

(ver 0.41)
- correct tuplets handling
- correct slurs termination
- beaming export
- option dialog to control beaming export

(ver 0.40)
- plugin redesigned, code structuration 
- exports key signature, main articulations
- tie problem should be resolved

(ver 0.31)
- more information added to output gmn file header
- default output file is now user documents folder + score file name + '.gmn'

(ver 0.3)
- grace note support (13.12.03)
- slurs (up to 4 nested/overlapping layers)


(ver 0.21beta)
- removed \key<>*) bug

(ver 0.2beta):
- Works now with Sibelius 2
- The Sibelius Plugin interface allows only the creation of unicode files.
  Therefore is the output .gmn file a unicode file! If the latest noteviewer version
  is not been used, it must be converted to ASCII manually!
- voices 1-4 of each staff can now be converted
- export of lyrics and text (added by jean-noel.rivasseau)

(ver 0.1):
- Convert complete score
- Export of notes, rests, ties, time-signatures and barlines


What it can not do 
(ver 0.2):
- Convert selected parts of scores
- Export of key signatures, slurs, additional elements (staccato, fermata)

Known problems:
- Ties to and from chords causes some problems
--------------------------------------------------------------
Please have a look at
	http://www.noteserver.org and
	http://guidolib.sf.net
for more information and updates
