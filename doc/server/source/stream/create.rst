Creating score with stream API
------------------------------

.. index::
  single: stream, create score

| This service is used to create a new score using the stream API. 

**url**
	/stream

	The score data are in a request attribute called ``data``.

**Response body**
	* in case of success: 
		| a unique identifier, formatted as JSON with 'ID' as key.
	  	| Example: ``{ "ID" : "a_unique_score_id" }``
	* in case of error: 
		| An error message formatted as JSON with 'Error' as key.
	  	| Example: ``{ "Error" : "Parse error line 9." }``

**Response code**
	* 201 ("Created") in case of success.
	* 400 ("Bad request") in case of non valid GMN code.

**Valid GMN code**
	is GUIDO Music Notation code that is successfully parsed.

Examples
^^^^^^^^^^^
	
The following HTML script passes in Guido Music Notation code to a server::

  <html>
    <body>
      <form action="http://guido.server.org/stream" method="post">
        <input type="hidden" name="data" value="[ g e c " />
        <input type="submit" value="Submit" />
      </form>
    </body>
  </html>
	
Here is an equivalent call using ajax::

   $.ajax({
        url: 'http://guido.server.org/stream',
        type: 'POST',
        data: "[ g e c",
    });

	
or using jQuery::

	$.post('http://guido.server.org/stream', "[ g e c");

or using curl::

  curl -d "data=[ g e c" http://guido.server.org/stream


POSTing files
^^^^^^^^^^^^^

Files can be sent to the server via POST.  For example, you can upload the file ``foo.gmn`` with::

  curl --data-urlencode "data@foo.gmn" http://guido.grame.fr:8000/stream

Internals
^^^^^^^^^^^

You may think of the returned ID as a reference to a `GUIDO stream <http://guidolib.sourceforge.net/doc/guidolib/group__Parser.html>`_, that is build using ``GuidoOpenStream`` and ``GuidoWriteStream`` C/C++ API. After that, the Guido Abstract representation is created.
