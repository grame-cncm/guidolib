Score drawing and page formatting
=================================

.. index::
  single: drawing

| Drawing a score and getting the corresponding image depends on optionnal layout and formatting parameters. 

.. toctree::
   :maxdepth: 1

   params/layout
   params/format

**Path**
	``ScoreID``: an identifier previously retrived using the POST method

**Response body**
	* in case of success: 
		| an image of the score which format and mime/type depends on the optional formatting parameters.
	* in case of error:
		| An error message formatted as JSON with 'Error' as key.
	  	| Example: ``{ "Error" : "incorrect score ID." }``

**Response code**
	* 200 ("Success")
	* 400 ("Bad Request") in case of incorrect parameter.
	* 404 ("Not Found") in case of incorrect score ID.

Example
^^^^^^^^^^^

Using jQuery::

	$.get('http://guido.server.org/XXX/?page=2');

