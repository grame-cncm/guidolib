MIDI File options
------------------------------

.. index::
  single: ticks
  single: chan
  single: intens
  single: accent
  single: marcato
  single: dur
  single: stacc
  single: slur
  single: tenuto
  single: fermata

MIDI File options may be used with the ``midi`` path of a score.

**Options**

	- ``ticks``: 
		ticks per quarternote
		
		*default value*: 960

	- ``chan``: 
		the MIDI channel

		*default value*: 1

	- ``intens``: 
		a floating point value in the range [0.0 ... 1.0]

		*default value*: 0.8

	- ``accent``: 
		a floating point value used for accents as ``intens`` multiplicator

		*default value*: 1.1

	- ``marcato``: 
		a floating point value used for marcato as ``intens`` multiplicator

		*default value*: 1.2

	- ``dur``: 
		a floating point value in the range [0.0 ... 1.0] used as duration factor.

		*default value*: 0.8

	- ``stacc``: 
		a floating point value used for staccato as ``dur`` multiplicator

		*default value*: 0.8

	- ``slur``: 
		a floating point value used for slurs as ``dur`` multiplicator

		*default value*: 1.0

	- ``tenuto``: 
		a floating point value used for tenuto as ``dur`` multiplicator

		*default value*: 0.9

	- ``fermata``: 
		a floating point value used for feramat as ``dur`` multiplicator

		*default value*: 2.0

Example:
^^^^^^^^^^^

Using jQuery::

	$.get('http://guido.server.org/XXX/midi?chan=5&intens=0.5');


Internals
^^^^^^^^^^^

The MIDI options correspond to the `Guido2MidiParams <http://guidolib.sourceforge.net/doc/guidolib/structGuido2MidiParams.html>`_ data structure.
