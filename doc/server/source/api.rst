API
===

| The GUIDO Engine Web API makes only use of the HTTP POST, GET and HEAD methods.
| The POST method is used to create a new score and the GET method to get the corresponding score or the score attributes. 
 Unless otherwise specified, the HTTP request entity-body is empty for the GET method or contains GMN code for the POST method.
 The HEAD method will return the header of a server response.
 Responses from the server make use of the HTTP header response code to indicate the request status, and the entity-body contains `JSON <http://www.json.org/>`_ code.


.. toctree::
   :maxdepth: 2

   post
   get
