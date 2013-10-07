The POST method
===============

| The POST method is used to create a new score. 
| The HTTP request entity-body is expected to contain **valid GMN code**.

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
      <form action="http://guido.server.org" method="post">
        <input type="hidden" name="data" value="[ g e c ]" }' />
        <input type="submit" value="Submit" />
      </form>
    </body>
  </html>
	
Here is an equivalent call using ajax::

   $.ajax({
        url: 'http://guido.server.org',
        type: 'POST',
        data: "[ g e c ]",
    });

	
or using jQuery::

	$.post('http://guido.server.org', "[ g e c ]");

or using curl::

  curl -d "data={ \"[ g e c ]\" }" http://guido.server.org



Internals
^^^^^^^^^^^

You may think of the returned ID as a reference to a `GUIDO abstract representation <http://guidolib.sourceforge.net/doc/guidolib/group__Engine.html>`_ [GAR], that is build using ``GuidoParseFile`` C/C++ API. Further reference to this ID may be viewed as a reference to this GAR.