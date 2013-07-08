.. _layout:

Score layout options
------------------------------

.. index::
  single: layout
  single: sysDistance
  single: sysDistribLimit
  single: spring
  single: neighborhoodSpacing
  single: optimalPageFill

Score layout options may be used whenever a conversion from the abstract representation to the graphic representation is required  (see the C/C++ library `documentation <http://guidolib.sourceforge.net/doc/guidolib/group__Engine.html>`_ 

**Options**

	- ``sysDistance``: 
		a float value to control the distance between systems, expressed in internal units
		
		*default value*: 75

	- ``sysDistribution``: 
		a string value to control the systems distribution. Accepted values are:

		- ``auto``: automatic systems distribution
		- ``always``: force systems distribution
		- ``never``: prevent systems distribution

		*default value*: "auto"

	- ``sysDistribLimit``: 
		a float value to control the maximum distance allowed between two systems for automatic distribution mode. Distance is relative to the height of the inner page.

		*default value*: 0.25 (that is: 1/4 of the page height)

	- ``force``: 
		a float value to indicate the force value of the Space-Force function. Typical values range from 400 to 1500.
		
		*default value*: 750
	
	- ``spring``: 
		a float value for the spring parameter. Typical values range from 1 to 5.
		
		*default value*: 1.1

	- ``neighborhoodSpacing``: 
		a boolean string ("yes"|"no") to tell the engine to use the neighborhood spacing algorithm or not.
		
		*default value*: "no"

	- ``optimalPageFill``: 
		a boolean string ("yes"|"no") to tell the engine to use the optimal page fill algorithm or not.
		
		*default value*: "yes"


Example:
^^^^^^^^^^^

Using jQuery::

	$.get('http://guido.server.org/XXX?optimalPageFill="no"&sysDistance=100');


Internals
^^^^^^^^^^^

The layout options correspond to the `GuidoLayoutSettings <http://guidolib.sourceforge.net/doc/guidolib/structGuidoLayoutSettings.html>`_ data structure.
