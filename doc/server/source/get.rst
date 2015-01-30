The GET method
===============

| The GET method is used to query an existing score or the GUIDO engine itself. 
| The HTTP request entity-body is ignored and should be empty.
| The scope of the request is indicated by the path of the URI.
| Replies to GET queries are formatted as JSON and presented as a values of an object which key is the score identifier.

Whenever the request involves a conversion to a graphic instance of the score, options may be used to convey the layout settings. Similarly, options may be used to convert the score to a MIDI file. 

.. toctree::
   :maxdepth: 1

   browsing
   drawing
   pianoroll
   midi
   mapping
   misc

