Getting a page at a given date
------------------------------

.. index::
  single: pageat

**Path**
	*ScoreID*/``pageat``

**Parameters**
	``date``: a date expressed as a rational value.

**Response body**
	* in case of success: 
		| Gives the page number of the score identified by ``ScoreID`` that contains the given date, formatted as JSON with 'date' and 'page' as key.
	  	
	  	Example::
	  	
			{ "ScoreID" : {
					"date" : "15/2", 
					"page" : 2 
				}
			}
	  		
	* in case of error:
		| An error message formatted as JSON with 'Error' as key.
	  	| Example: ``{ "ScoreID" : {"Error" : "incorrect score ID."} }``

**Response code**
	* 200 ("Success")
	* 400 ("Bad Request") when the score doesn't contains the date.
	* 404 ("Not Found") in case of incorrect score ID.

Example
^^^^^^^^^^^

Using jQuery::

	$.get('http://guido.server.org/XXX/pageat?date="22/8"');


Internals
^^^^^^^^^^^

The ``pageat`` request correspond to the ``GuidoFindPageAt`` C/C++ API.
