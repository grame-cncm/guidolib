.. _format:

Formatting options
------------------------------

.. index::
  single: format
  single: page
  single: width
  single: height
  single: format
  single: resize
  single: margins


Score drawing options may be used when a graphic instance of the score is required.  

**Options**

	- ``page``: 
		a page number
		
		*default value*: 1

	- ``width``: 
		the drawing area width in centimeters. 
		
		*default value*: 15

	- ``height``: 
		the drawing area height in centimeters.
		
		*default value*: 5

	- ``marginleft``: 
		the left margin in centimeters. 
		
		*default value*: 1

	- ``marginright``: 
		the right margin in centimeters. 
		
		*default value*: 1

	- ``margintop``: 
		the top margin in centimeters. 
		
		*default value*: 0.5

	- ``marginbottom``: 
		the bottom margin in centimeters. 
		
		*default value*: 0.5

	- ``format``: 
		a string to request a specific image format. Accepted values are:

		- ``png``: to produce a png output
		- ``jpg``: to produce a jpeg output
		- ``svg``: to produce a svg output
		- ``binary``: to produce a binary output which can be parse with a external device to draw the score.
		- ``gif``: to produce a gif output
 
		*default value*: "png"

	- ``resize``:  
		a boolean string ("yes"|"no") to resize the page size to the music size. 
		
		*default value*: "yes"

Example
^^^^^^^^^^^

Using jQuery::

	$.get('http://guido.server.org/XXX?width=20&height=10&format=svg');
