.. _layoutpianoroll:

Pianoroll limits options
------------------------------

.. index::
  single: layout
  single: sysDistance
  single: sysDistribLimit
  single: spring
  single: neighborhoodSpacing
  single: optimalPageFill

Limits params can be use to limit a piano roll by setting start date / end date and lower pitch / higher pitch  (see the C/C++ library `documentation <http://guidolib.sourceforge.net/doc/guidolib/group__PianoRoll.html>`_)

**Options**

	- ``startDate``: 
		date is expressed as a rational value
		
		*default value*: "0/0" (start date is the score start date)

	- ``endDate``: 
		date is expressed as a rational value

		*default value*: "0/0" (end date is the score end date)

	- ``lowPitch``: 
		a int value to control the lower pitch expressed in midi notation.

		*default value*: -1 (score minimal pitch)

	- ``highPitch``: 
		a int value to control the higher pitch expressed in midi notation.

		*default value*: -1 (score maximal pitch)
	

Example:
^^^^^^^^^^^

Using jQuery::

	$.get('http://guido.server.org/XXX/pianoroll?startDate="2/4"&endDate="12/1"');


Internals
^^^^^^^^^^^

The layout options correspond to the `GuidoLayoutSettings <http://guidolib.sourceforge.net/doc/guidolib/structGuidoLayoutSettings.html>`_ data structure.
