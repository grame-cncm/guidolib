.. _format:

Formatting options
------------------------------


.. index::
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
		the drawing area width in pixels. 
		
		*default value*: 400

	- ``height``: 
		the drawing area height in pixels. 
		
		*default value*: 600

	- ``format``: 
		a string to request a specific image format. Accepted values are:

		- ``png``: to produce a png output
		- ``jpg``: to produce a jpeg output
		- ``svg``: to produce a svg output
 
		*default value*: "png"

	- ``resize``:  
		a boolean string ("yes"|"no") to resize the page size to the music size. 
		
		*default value*: "yes"

	- ``margins``:  
		a comma separated list of 4 values separated by space, indicating the left, top, right and bottom margins size. Values are in pixels. Note that page formatting is ignored when the GMN code already contains a \pageFormat tag.
		
		*default value*: "10,10,10,10"

Example
^^^^^^^^^^^

Using jQuery::

	$.get('http://guido.server.org/XXX?width=220&height=100&format=svg');
