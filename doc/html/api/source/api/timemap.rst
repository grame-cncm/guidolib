Time to time mapping
----------------------------

.. index::
  single: timemap

Gives the mapping between the score and the performance time (i.e. rolled to unrolled time) according to repeat bars and jumps (to coda, da capo, etc.).


**Path**
	*ScoreID*/``timemap``

**Response body**
	* in case of success: 
		| a set of pairs indicating the relation between the score and the performance time, formatted as JSON with ``timemap`` as key.
	  	
	  	Example::

	  		{ "timemap" : [
	  				["score": {"start": "0/1", "end": "1/1"}, 
	  				 "perf":  {"start": "0/1", "end": "1/1"} ],
	  				["score": {"start": "1/1", "end": "2/1"}, 
	  				  "perf": {"start": "1/1", "end": "2/1"} ],
	  				["score": {"start": "2/1", "end": "3/1"}, 
	  				 "perf":  {"start": "0/1", "end": "1/1"} ],
	  				["score": {"start": "3/1", "end": "4/1"}, 
	  				  "perf": {"start": "1/1", "end": "2/1"} ]
	  			] 
	  		}

	* in case of error:
		| An error message formatted as JSON with 'Error' as key.
	  	| Example: ``{ "Error" : "incorrect score ID." }``

**Response code**
	* 200 ("Success")
	* 404 ("Not Found") in case of incorrect score ID.

Example
^^^^^^^^^^^

Using jQuery::

	$.get('http://guido.server.org/XXX/timemap');

