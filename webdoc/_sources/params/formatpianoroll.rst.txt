.. _formatpianoroll:

Formatting options
------------------------------

.. index::
  single: keyboard
  single: autoVoicesColoration
  single: enableMeasureBars
  single: pitchLinesDisplayMode

Score drawing options may be used when a graphic instance of the score is required.  

**Options**

	- ``width``: 
		the drawing area width in pixels. 
		
		*default value*: 1024

	- ``height``: 
		the drawing area height in pixels.
		
		*default value*: 512

	- ``keyboard``: 
		Enables the keyboard or not. 
		
		- ``true``: keyboard enabled
		- ``false``: keyboard not enabled

		*default value*: false

	- ``autoVoicesColoration``: 
		Enables or not the automatic voices coloration

		- ``true``: automatic voices coloration enabled
		- ``false``: automatic voices coloration not enabled

		*default value*: false

	- ``enableMeasureBars``: 
		Enables or not measure bars

		- ``true``: measure bars enabled
		- ``false``: measure bars not enabled

		*default value*: false

	- ``pitchLinesDisplayMode``: 
		Sets the pitch lines display mode. Use Pitch lines display mode constants to pick lines which will be be displayed.
		
		*default value*: 0 (automatic mode)

	- ``format``: 
		a string to request a specific image format. Accepted values are:

		- ``png``: to produce a png output
		- ``jpg``: to produce a jpeg output
		- ``svg``: to produce a svg output
		- ``binary``: to produce a binary output which can be parse with a external device to draw the score.
		- ``gif``: to produce a gif output
 
		*default value*: "png"

Example
^^^^^^^^^^^

Using jQuery::

	$.get('http://guido.server.org/XXX/pianoroll?width=400&height=200&format=svg');
