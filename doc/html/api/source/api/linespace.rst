Getting lines spacing
---------------------

.. index::
  single: linespace

**Path**
	``linespace``

**Response body**
	* in case of success: 
		| Gives the distance between two staff lines formatted as JSON with 'linespace' as key.
		| This value is constant (= 50). It does not depend on the context and will probably never change in future versions of the library.
	  	| Example: ``{ "linespace" : 50 }``

**Response code**
	* 200 ("Success")

Example
^^^^^^^^^^^

Using jQuery::

	$.get('http://guido.server.org/linespace');


Internals
^^^^^^^^^^^

The ``linespace`` request correspond to the ``GuidoGetLineSpace`` C/C++ API.
