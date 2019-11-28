Graphic to time mappings
----------------------------

.. index::
  single: getpagemap
  single: getstaffmap
  single: getvoicemap
  single: getsystemmap
  single: getmap

Gives the mapping between the graphic and the time time. The mapping is given as a set of pairs of graphic rectangles and time intervals. 

Note that the relations between the graphic and time space depends on :ref:`layout` and :ref:`format`. Thus, and to make sense, layout and formatting parameters could be used with the mapping requests and should be the same than those used to get the graphic score.


**Path**

	| 	*ScoreID*/``getpagemap``	: gives the time to graphic mapping at page level. The array of mapping is called *pagemap*.
	| 	*ScoreID*/``getstaffmap``: gives a staff time to graphic mapping. An optionnal ``staff`` parameter may be used to indicate the staff number (default vaue is 1). The array of mapping is called *staffmap*.
	| 	*ScoreID*/``getvoicemap`` : gives a voice time to graphic mapping. An optionnal ``voice`` parameter may be used to indicate the voice number (default vaue is 1). The array of mapping is called *voicemap*.
	| 	*ScoreID*/``getsystemmap`` : gives the time to graphic mapping at system level. The array of mapping is called *systemmap*.
	|	*ScoreID*/pianoroll/``getmap`` : gives the time to graphic mapping for a piano roll. The array of mapping is called *pianorollmap*.

**Response body**
	* in case of success: 
		| a set of pairs indicating the relation between the graphic and the time space, formatted as JSON with the path as key.
	  	
	  	Example with getstaffmap for the score *ScoreID*::

	  		{ "ScoreID" : {
	  			"staffmap" : [
						{"graph": {"left": 80, "top": 0, "right": 122, "bottom": 451 }, 
						 "time": { "start": "0/1", "end": "1/1"} },
						{"graph": {"left": 122, "top": 0, "right": 243, "bottom": 451 }, 
						 "time": { "start": "1/1", "end": "2/1"} },
						{"graph": {"left": 243, "top": 0, "right": 312, "bottom": 451 }, 
						 "time": { "start": "2/1", "end": "3/1"} }
					] 
				}
	  		}

	* in case of error:
		| An error message formatted as JSON with 'Error' as key.
	  	| Example: ``{ "ScoreID" : {"Error" : "incorrect score ID."} }``

**Response code**
	* 200 ("Success")
	* 404 ("Not Found") in case of incorrect score ID.

Example
^^^^^^^^^^^

Using jQuery::

	$.get('http://guido.server.org/XXX/staffmap?staff=1');

