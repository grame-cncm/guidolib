Getting GUIDO Server information
----------------------------

.. index::
  single: server

**Path**
	``server``

**Response body**
	* in case of success: 
		| a version number, formatted as JSON with 'server' as key.
	  	| Example: ``{ "server" : "1.0" }``

**Response code**
	* 200 ("Success")

Example
^^^^^^^^^^^

Using jQuery::

	$.get('http://guido.server.org/version');


Internals
^^^^^^^^^^^

The ``version`` request correspond to the ``GuidoGetVersionStr`` C/C++ API.
