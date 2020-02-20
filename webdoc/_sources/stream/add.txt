Adding data to a score with stream API
---------------------------------------

.. index::
  single: stream, add data

| This service is used to add data to an existing score with the stream API. You can only add data to a score created with the stream API.

**url**
	/stream/*ScoreID*

	The score data are in a request attribute called ``data``.

**Response body**
	* in case of success: 
		| The score ID, formatted as JSON with 'ID' as key. This is the same score ID in the request and in the response. 
	  	| Example: ``{ "ID" : "ScoreID" }``
	* in case of guido code error: 
		| An error message formatted as JSON with 'Error' as key.
	  	| Example: ``{ "Error" : "Parse error line 9." }``
	* in case of session error:
		| An error message formatted as JSON with 'Error' as key.
	  	| Example: ``{ "ScoreID" : {"Error" : "incorrect score ID."} }``

**Response code**
	* 201 ("Created") in case of success.
	* 400 ("Bad request") in case of non valid GMN code.
	* 404 ("Not Found") in case of incorrect score ID.

**Valid GMN code**
	is GUIDO Music Notation code that is successfully parsed.

Examples
^^^^^^^^^^^
	
The following HTML script passes in Guido Music Notation code to a server::

  <html>
    <body>
      <form action="http://guido.server.org/stream/*ScoreID*" method="post">
        <input type="hidden" name="data" value="g e c " />
        <input type="submit" value="Submit" />
      </form>
    </body>
  </html>
	
Here is an equivalent call using ajax::

   $.ajax({
        url: 'http://guido.server.org/stream/*ScoreID*',
        type: 'POST',
        data: "g e c",
    });

	
or using jQuery::

	$.post('http://guido.server.org/stream/*ScoreID*', "g e c");

or using curl::

  curl -d "data=g e c" http://guido.server.org/stream/*ScoreID*


POSTing files
^^^^^^^^^^^^^

Files can be sent to the server via POST.  For example, you can upload the file ``foo.gmn`` with::

  curl --data-urlencode "data@foo.gmn" http://guido.grame.fr:8000/stream/*ScoreID*

Internals
^^^^^^^^^^^

The new data are added to the referenced stream with `GuidoWriteStream <http://guidolib.sourceforge.net/doc/guidolib/group__Parser.html>`_  C/C++ API. After that, the Guido Abstract representation is created.
