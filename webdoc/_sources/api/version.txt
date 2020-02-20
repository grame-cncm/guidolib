Getting GUIDO Engine version
----------------------------

.. index::
  single: version

**Path**
	``version``

**Response body**
	* in case of success: 
		| a version number, formatted as JSON with 'version' as key.
	  	| Example: ``{ "version" : "1.5.0" }``

**Response code**
	* 200 ("Success")

Example
^^^^^^^^^^^

Using jQuery::

	$.get('http://guido.server.org/version');


Internals
^^^^^^^^^^^

The ``version`` request corresponds to the ``GuidoGetVersionStr`` C/C++ API.
