Getting the voices count
--------------------------

.. index::
  single: countvoices


**Path**
	*ScoreID*/``countvoices``

**Response body**
	* in case of success: 
		| Gives the voices count of the score identified by ``ScoreID`` formatted as JSON with 'voicecount' as key.
	  	| Example: ``{ "ScoreID" : {"voicecount" : 4} }``
	* in case of error:
		| An error message formatted as JSON with 'Error' as key.
	  	| Example: ``{ "ScoreID" : {"Error" : "incorrect score ID."} }``

**Response code**
	* 200 ("Success")
	* 404 ("Not Found") in case of incorrect score ID.

Example
^^^^^^^^^^^

Using jQuery::

	$.get('http://guido.server.org/XXX/countvoices');


Internals
^^^^^^^^^^^

The ``countvoices`` request corresponds to the ``GuidoCountVoices`` C/C++ API.
