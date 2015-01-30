Pianoroll drawing and page formatting
=================================

.. index::
  single: pianoroll

| Drawing a score and getting the corresponding image depends on optionnal layout and formatting parameters. 

.. toctree::
   :maxdepth: 1

   params/layoutpianoroll
   params/formatpianoroll

**Path**
	``ScoreID/pianoroll``: for drawing a simple pianoroll. ScoreID is an identifier previously retrived using the POST method.
	``ScoreID/trajectorypianoroll``: for drawing a trajectory pianoroll. ScoreID is an identifier previously retrived using the POST method.

**Response body**
	* in case of success: 
		| an image of the pianoroll which format and mime/type depends on the optional formatting parameters.
	* in case of error:
		| An error message formatted as JSON with 'Error' as key.
	  	| Example: ``{ "ScoreID" : {"Error" : "incorrect score ID."} }``

..
  **Response code**
          * 200 ("Success")	
          * 400 ("Bad Request") in case of incorrect parameter.
          * 404 ("Not Found") in case of incorrect score ID.

**Response code**
	* 200 ("Success")	
	* 404 ("Not Found") in case of incorrect score ID.

Example
^^^^^^^^^^^

Using jQuery::

	$.get('http://guido.server.org/XXX/pianoroll?page=2');

