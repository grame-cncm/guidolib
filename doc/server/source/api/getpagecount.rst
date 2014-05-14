Getting the pages count
-----------------------

.. index::
  single: getpagecount

**Path**
	*ScoreID*/``getpagecount``

**Response body**
	* in case of success: 
		| Gives the count of pages of the score identified by ``ScoreID`` formatted as JSON with 'pagecount' as key.
	  	| Example: ``{ "ScoreID" : {"pagecount" : 1} }``
	* in case of error:
		| An error message formatted as JSON with 'Error' as key.
	  	| Example: ``{ "ScoreID" : {"Error" : "incorrect score ID."} }``

**Response code**
	* 200 ("Success")
	* 404 ("Not Found") in case of incorrect score ID.

Example
^^^^^^^^^^^

Using jQuery::

	$.get('http://guido.server.org/linespace');


Internals
^^^^^^^^^^^

The ``getpagecount`` request corresponds to the ``GuidoGetPageCount`` C/C++ API.
