Reset score with stream API
------------------------------

.. index::
  single: stream, reset data

| This service is used to reset a score using the stream API. You can only reset data to a score created with the stream API.

**url**
	/stream/*ScoreID*/reset

**Response body**
	* in case of success: 
		| The score ID, formatted as JSON with 'ID' as key. This is the same score ID in the request and in the response. 
	  	| Example: ``{ "ID" : "a_unique_score_id" }``
	* in case of session error:
		| An error message formatted as JSON with 'Error' as key.
	  	| Example: ``{ "ScoreID" : {"Error" : "incorrect score ID."} }``

**Response code**
	* 201 ("Created") in case of success.
	* 404 ("Not Found") in case of incorrect score ID.

Examples
^^^^^^^^^^^
	
The following HTML script passes in Guido Music Notation code to a server::

  <html>
    <body>
      <form action="http://guido.server.org/stream/*ScoreID*/reset" method="post">
        <input type="submit" value="Submit" />
      </form>
    </body>
  </html>
	
Here is an equivalent call using ajax::

   $.ajax({
        url: 'http://guido.server.org/stream/*ScoreID*/reset',
        type: 'POST',
    });

	
or using jQuery::

	$.post('http://guido.server.org/stream/*ScoreID*/reset');

or using curl::

  curl http://guido.server.org/stream/*ScoreID*/reset

Internals
^^^^^^^^^^^

You may think the GUIDO stream is reset with `GuidoResetStream <http://guidolib.sourceforge.net/doc/guidolib/group__Parser.html>`_. The associated Abstract Respresentation is deleted.
