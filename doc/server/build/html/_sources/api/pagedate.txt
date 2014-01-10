Getting the date of a given page
--------------------------------

.. index::
  single: pagedate

**Path**
	*ScoreID*/``pagedate``

**Parameters**
	``page``: a page number (page numbers start at 1). Default page is 1.

**Response body**
	* in case of success: 
		| Gives the date of a page of the score identified by ``ScoreID`` formatted as JSON with 'page' and 'date' as keys. The date is expressed as a rational value.
	  	
	  	Example:: 
	  	
	  		{ "ScoreID" : {
					"page" : 1,
					"date" : "12/1"
	  			}
	  		}

	* in case of error:
		| An error message formatted as JSON with 'Error' as key.
	  	| Example: ``{ "ScoreID" : {"Error" : "incorrect score ID."} }``

**Response code**
	* 200 ("Success")
	* 400 ("Bad Request") when there is no such page.
	* 404 ("Not Found") in case of incorrect score ID.

Example
^^^^^^^^^^^

Using jQuery::

	$.get('http://guido.server.org/XXX/pagedate?page=2');


Internals
^^^^^^^^^^^

The ``pagedate`` request correspond to the ``GuidoGetPageDate`` C/C++ API.
