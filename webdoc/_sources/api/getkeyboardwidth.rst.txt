Getting the Keyboard width
--------------------------

.. index::
  single: getkeyboardwidth

Get the keyboard width of a piano roll. The keyboard has to be drawn to calculate the width. The parameter *keyboard=true* is necessary to have a non null result.

**Path**
	*ScoreID*/pianoroll/``getkeyboardwidth``

	*ScoreID*/trajectorypianoroll/``getkeyboardwidth``

**Response body**
	* in case of success: 
		| Gives the keyboard width of the pianoroll identified by ``ScoreID`` formatted as JSON with 'keyboardwidth' as key.
	  	| Example: ``{ "ScoreID" : {"keyboardwidth" : 256} }``
	* in case of error:
		| An error message formatted as JSON with 'Error' as key.
	  	| Example: ``{ "ScoreID" : {"Error" : "incorrect score ID."} }``

**Response code**
	* 200 ("Success")
	* 404 ("Not Found") in case of incorrect score ID.


Example
^^^^^^^^^^^

Using jQuery::

	$.get('http://guido.server.org/XXX/pianoroll/getkeyboardwidth?keyboard=true');


Internals
^^^^^^^^^^^

The ``getkeyboardwidth`` request corresponds to the ``GuidoPianoRollGetKeyboardWidth`` C/C++ API.
