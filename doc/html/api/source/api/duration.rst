Getting a score duration
--------------------------

.. index::
  single: duration

**Path**
	ScoreID/``duration``

**Response body**
	* in case of success: 
		| Gives the duration of the score identified by ``ScoreID`` formatted as JSON with 'duration' as key and expressed as a rational value.
		| The rational value represents music time, where 1 is the whole note duration.
	  	| Example: ``{ "duration" : "32/4" }``
	* in case of error:
		| An error message formatted as JSON with 'Error' as key.
	  	| Example: ``{ "Error" : "incorrect score ID." }``

**Response code**
	* 200 ("Success")
	* 404 ("Not Found") in case of incorrect score ID.

Example
^^^^^^^^^^^

Using jQuery::

	$.get('http://guido.server.org/XXX/duration');


Internals
^^^^^^^^^^^

The ``duration`` request correspond to the ``GuidoDuration`` C/C++ API.
