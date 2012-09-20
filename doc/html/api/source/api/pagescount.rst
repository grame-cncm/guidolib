Getting the pages count
-----------------------

.. index::
  single: pagescount

**Path**
	*ScoreID*/``pagescount``

**Response body**
	* in case of success: 
		| Gives the count of pages of the score identified by ``ScoreID`` formatted as JSON with 'pagescount' as key.
	  	| Example: ``{ "pagescount" : 1 }``
	* in case of error:
		| An error message formatted as JSON with 'Error' as key.
	  	| Example: ``{ "Error" : "incorrect score ID." }``

**Response code**
	* 200 ("Success")
	* 404 ("Not Found") in case of incorrect score ID.

Example
^^^^^^^^^^^

Using jQuery::

	$.get('http://guido.server.org/linespace');


Internals
^^^^^^^^^^^

The ``pagescount`` request correspond to the ``GuidoGetPageCount`` C/C++ API.
