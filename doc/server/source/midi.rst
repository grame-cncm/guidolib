MIDI rendering
==============

.. index::
  single: midi

MIDI file export is available from the ``midi`` path and may use optional export parameters. 

.. toctree::
   :maxdepth: 1

   params/midi

**Path**
	*ScoreID*/``midi``

**Response body**
	* in case of success: 
		| a MIDI file.
	* in case of error:
		| An error message formatted as JSON with 'Error' as key.
	  	| Example: ``{ "ScoreID" : {"Error" : "incorrect score ID."} }``

**Response code**
	* 200 ("Success")
	* 400 ("Bad Request") in case of incorrect parameter.
	* 404 ("Not Found") in case of incorrect score ID.

Example
^^^^^^^^^^^

Using jQuery::

	$.get('http://guido.server.org/XXX/midi');
