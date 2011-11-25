

        GUIDO NoteViewer, Version 0.92 (May/05/2004)

--------------------------------------------------------------------------------
- Contents -
--------------------------------------------------------------------------------

. Important Notice
. Introduction
. System requirements 
. Updating old Versions
. What's new
. Getting Help and Reporting Bugs
. Features present in this version
. Supported GUIDO Tags (Basic GUIDO)
. Not yet supported GUIDO Tags
. Features planned for future versions
. Copyright and Disclaimer

--------------------------------------------------------------------------------
- Important Notice -
--------------------------------------------------------------------------------

By downloading the installation package or using the
software, you acknowlegde having read and understood
the information given under Section "Copyright and
Disclaimer" of this documents. Please read that section 
carefully before you proceed with the installation
of the GUIDO NoteViewer. 

--------------------------------------------------------------------------------
-  Introduction -
--------------------------------------------------------------------------------

The GUIDO NoteViewer is a tool to display and print musical 
scores, that are written in GUIDO Music Notation (GMN).
GUIDO Music Notation is a textbased, humanreadable format 
to represent music. It is intended to describe musical 
information as well as notational information. In GUIDO, 
simple music can be described in a simple way, whereas 
complex music sometimes requires more complex representations. 

The GuidoLib project is now hosted at sourceforge.net

http://sourceforge.net/projects/guidolib/

If you want to get more information on GUIDO Music Notation 
please visit our website at

http://www.salieri.org/guido

If you want to get online music notation, please visit our
free GUIDO NoteServer at

http://www.noteserver.org

--------------------------------------------------------------------------------
- System Requirements and Installation -
--------------------------------------------------------------------------------

GUIDO NoteViewer needs a PC with 
Windows 98, Windows NT, 4.0, XP (or higher Versions). 
It runs fine on 80486-Processors (or better) with at least 16 MB.

This program is standalone, no installer is provided. Just put the Guido 
NoteViewer directory wherever you want, i.e: in C:\Program Files\

A  NoteViewer w98 is provided for compatibility with Windows versions
older than NT/2000/XP.

--------------------------------------------------------------------------------
-  Updating old versions - 
--------------------------------------------------------------------------------

If you would like to replace an older version of the
GUIDO NoteViewer with the current version, it is
strongly recommended to FIRST UNINSTALL the previous 
version! Alternativly, you can install the new version 
into a new directory. 
Please make sure, that you save any changes to the 
example-files in the examples-directory of the previous
version, as they will be removed by the uninstall-procdure.


--------------------------------------------------------------------------------
- What's new in Release 0.92 (May/05/2004) -
--------------------------------------------------------------------------------

- Use the Guido Engine Library version 1.2.2:
	- Better automatic positioning for ties and slurs, articulations (staccato, 
	  accent...), and tuplets. 
	- Octava clefs and double treble clefs.
	- Pre-implementation of the \accol tag. Guido can now display the 
	  "straightBrace" accolade.
	- Text positionning ( \text tag ) implementation closer to the Guido specifications. 
	- Introduction of Longa notes, diamond and cross heads, and breath-marks.
	- Fixed an alignement problem for beams, during Gif export.
	- Fixed the intens "ff" and "pp" bugs.
	- Improved overall speed.

- NoteViewer: 
	- Improved Gmn text edit dialog.
	- Fixed a bug that could cause a crash after parse error.
	- Can export Gif files up to 3200x3200 pixels.

--------------------------------------------------------------------------------
- What's new in Release 0.91 (Jan/07/2004) -
--------------------------------------------------------------------------------

* Uses the new cross-platform version of the GuidoLib.

--------------------------------------------------------------------------------
- What's new in Release 0.9 -
--------------------------------------------------------------------------------

Release 0.9 of the GUIDO NoteViewer presents the following
new features:

Major changes:

- optimum page filling:
  all pages of a score are filled completly. This feature can be turned
  off in the Adjust Spacing Parameters Dialog.

- opimum line fill:
  The line break algorithm now follows Hegazy,Gourlay (which in 
  turn follows D.Knuth known from TeX). All lines of music are
  now broken in an optimum way, so that the overall appearance is smooth.

- MIDI import included:
  You can directly import MIDI files using a module provided
  by Jürgen Kilian. A frontend to this module shall be available
  soon.

- MuseData import included:
  You can directly import Files in the MuseData format. Please
  make sure that you understand the copyright of museData files.

- Creating of GIF files included:
  It is no longer necessary to use the online NoteServer to get
  GIF pictures of a score. You can directly save pages of 
  a score as GIF files.

- Progress indicator when reading GUIDO files

--------------------------------------------------------------------------------
- Getting Help and Reporting Bugs -
--------------------------------------------------------------------------------

GUIDO NoteViewer,  is still a prototype. Not all of the 
GUIDO Specification is supported yet (See sections 
"Supported GUIDO Tags" and "Not yet supported GUIDO Tags"). 
If you need help or would like to report a bug 
(greatly appreciated!),  you can fill a support request or
a bug report on the GuidoLib sourceforge.net page.

http://sourceforge.net/projects/guidolib/

Please try to describe the bug as precise as possible. 

--------------------------------------------------------------------------------
- Features present in this version -
--------------------------------------------------------------------------------

- optimum page fill
- optimum line break
- gif export
- MIDI import
- MuseData import
- reload file feature
- better chord handling (including tags within chords) 
- support of a large number of Advanced GUIDO tags
- playback of GUIDO files using MIDI
- export of GUIDO files to MIDI files
- Justified margins and spacing using a spring-rod-model
- Display and printing of GUIDO files using arbitrary
  zoom-factors.
- More Examples of GUIDO files in the example-directory
- Many of the Basic GUIDO tags are supported.
- You can view and edit the GUIDO Music Description
  by using "Edit -> Edit GMN-Text" or by pressing
  Ctrl-E when viewing a file.
  The GUIDO-Text is displayed and you can edit it.
  If you press the "Accept"-button, your changes
  are regonized and the piece is drawn anew. 
  When the view-window is closed you are asked whether
  you want to save the changes. You can also save
  your changes by using the File -> Save Menu or by
  pressing Ctrl-S.
- EPS-output (Encapsulated Postscript) is available
  (as an alpha-version). Use "Save as" and select
  the correct-file-type.


--------------------------------------------------------------------------------
- Supported GUIDO Tags (Basic GUIDO) -
--------------------------------------------------------------------------------

\beamsAuto 
	sets beaming to automatic mode; 
	automatic should depend on the meter (not
	yet implemented in this way).

\beamsOff 
	switches beaming off, i.e., only explicitly 
	beamed notes will be beamed. 

\beam(notes) , \bm(notes)
	beam notes; 
	no nested beams are yet supported.
	\beam<dy1,dy2>(notes) is not yet supported!

\clef<s> 
	sets clef s, where s is a string indicating the 
	form of the clef ("g","f","c")
	and the position within the staff 
	("1" for lowest line, ..., "5" for highest 
	line). Optionally, this can be succeeded by 
	"+8","-8", "+15","-15" for transposed clefs 
	(octave or double octave up or down). 
	Also supported are "violin","bass","alto",
	"tenor". See file clef.gmn in the examples
	directory.

\cresc(notes) 
	indicates an unspecific crescendo on notes. 
	Supported (no collision detection yet)

\cresc<s>(notes), \cresc<s,r>(notes)
	indicates a crescendo starting from the 
	current dynamic marking to dynamic 
	marking s on notes. r is ignored.
	Supported for s like the Parameter for \intens<s>

\crescBegin, \crescBegin<s>, \crescBegin<s,r>
	indicates the beginning of a crescendo section which ends 
	at the next \crescEnd tag; the crescendo starts 
	from the current dynamic marking. 

\crescEnd 
	indicates the end of an unspecific crescendo 
	section which began at the last \crescBegin tag. 

\dim(notes) 
	indicates an unspecific diminuendo on notes. 

\dim<s>(notes)m \dim<s,r>(notes) 
	indicates a diminuendo starting from the 
	current dynamic marking to dynamic marking s 
	on notes. 
	Parameter r is ignored when present.
	Supported for s like \intens<s>.

\dimBegin, \dimBegin<s>, \dimBegin<s,r> \dimEnd
	as \crescBegin etc.

\fermata 
	indicates a fermata at the current metric position 
	(for use between notes)
	Spacing is not yet optimized.

\fermata(notes) 
	indicates fermatas for all notes 

\intens<s>, \i<s>, \intens<s,r>, \i<s,r>
	indicates a dynamic marking s 
	r is ignored for graphical output.
	supported for s in: "p","f","ff","fff",
		"ffff","mf","mp","sf","pp","ppp","pppp"

\key<i> 
	sets key signature to i sharps, if i is positive, 
	to |i| flats, if i is negative. 
	For i=0, the key signature contains no sharps or flats. 

\key<s> 
	indicates a key of s, where s is of the form 
	"n", "n#" or "n&" for any note name n. 
	Uppercase and lowercase letters are used to denote 
	major and minor modes, respectively. 
	(Example \key<"A"> and \key<3> is equivalent).
	Also supported is a free keysignature. See 
	key.gmn in the example directory for this.

\tie(notes)
	Ties the notes with a tie. (see file 
	tie.gmn in the examples-directory)

\slur(notes), \sl(notes)
	slur encompassing the group notes. The
	shape of the slur is determined by the
	stem-direction of the first note of
	the group.

\slur<dx1,dy1, dx2,dy2,h,r3>(notes) or
\sl<dx1,dy1, dx2,dy2, h,r3>(notes) 
	three-control-point bezier slur encompassing 
	notes; 
	
\meter<s> 
	sets time signature to s, where s is of the form "n/m" 
	for positive integers n,m. 
	Also allowed for s is "C" or "C/".

\staff<i> 
	assigns the voice it appears in to staff number i 
	\staff<str> is not yet supported.
	See file choral.gmn in the example-Directory
	for the usage of the staff-Tag.
	There is no automatic collision-detection yet!

\stemsAuto 
	sets the stem direction to auto mode, 
	i.e., notes have downward stems if they are 
	lower than the 3rd line (if any), upward 
	stems otherwise. 

\stemsUp 
	sets stem directions to upward mode, i.e, all 
	stems are drawn upwards. 
        You can also specify the stemlength using 
        \stemsUp<12hs>

\stemsDown 
	sets stem directions to downward mode, i.e, 
	all stems are drawn downwards. 
        You can also specify the stemlength using
        \stemsDown<5hs>

\stemsOff
	turns the stems off. (Graphical)

\bar, '|', \bar<i>
	indicates a barline at the position where it occurs. 
	'|' is not yet supported.
	The Parameter i is ignored.

\text<s>, \t<s>, \text<s,dy>, \t<s,dy>
	places text s at current metric position 
	If dy is specified. The text appears dy
	halfspaces above (positive value) or below
	(negative value) the lowest staff-line.
	CAREFUL, dy-Interpretation changed!

\lyrics<"this is ly-rics">( notes )
        The lyrics are placed below the notes. A " ","-", or "_"
        switches to the next event in the range.

\repeatBegin
	Places a repeat-Begin-sign at the current position

\repeatEnd, \repeatEnd<n>( )
 	places a repeat-End-sign at the current position.
	If the second form is used, a repeat-end-sign is
 	placed at the end of the range (iff n == 1).
	This will be improved in later versions.

\tempo
\stacc(notes) 
\accent(notes) 
\ten(notes) 
\marcato(notes) 
\grace(notes), \grace<i>(notes) 
\cue(notes), \cue<s>(notes) 
\oct<i>, \oct<i>(notes) 
\title<s> 
\composer<s> 
\mark<s> 
\label<s>, \label<s>(notes) 

Some of the following tags do not have nice visual representations
but are supported:

\accel, \accelBegin, \accelEnd
\rit, \ritBegin, \ritEnd
\instr
\trill(chords), \trill<i>(chords) 
\mord(chords), \mord<i>(chords) 
\turn(chords), \turn<i>(chords) 
\trem(notes), \trem<i>(notes) 
\doubleBar, \doubleBar<i> 
\tactus 

--------------------------------------------------------------------------------
- Not yet supported Basic GUIDO Tags -
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
- Features Planned for the Following Version -
--------------------------------------------------------------------------------

- Support for cutting and pasting of scores into other 
  applications (like e.g. Word)
- Saving program-parameters in the registry
- A help-file

--------------------------------------------------------------------------------
- Copyright and Disclaimer -
--------------------------------------------------------------------------------

GUIDO NoteViewer.
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.




