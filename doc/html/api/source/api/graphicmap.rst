Graphic to time mappings
----------------------------

.. index::
  single: pagemap
  single: staffmap
  single: voicemap
  single: systemmap

Gives the mapping between the graphic and the time time. The mapping is given as a set of pairs of graphic rectangles and time intervals. 

Note that the relations between the graphic and time space depends on :ref:`layout` and :ref:`format`. Thus, and to make sense, layout and formatting parameters could be used with the mapping requests and should be the same than those used to get the graphic score.


**Path**

| ``pagemap``	: gives the time to graphic mapping at page level
| ``staffmap``: gives a staff time to graphic mapping
| ``voicemap`` : gives a voice time to graphic mapping
| ``systemmap`` : gives the time to graphic mapping at system level

**Response body**
	* in case of success: 
		| a set of pairs indicating the relation between the graphic and the time space, formatted as JSON with the path as key.
	  	
	  	Example::

	  		{ "staffmap" : [
	  				["graph": {"left": 80, "top": 0, "right": 122, "bottom": 451 }, 
	  				 "time": { "start": "0/1", "end": "1/1"} ],
	  				["graph": {"left": 122, "top": 0, "right": 243, "bottom": 451 }, 
	  				 "time": { "start": "1/1", "end": "2/1"} ],
	  				["graph": {"left": 243, "top": 0, "right": 312, "bottom": 451 }, 
	  				 "time": { "start": "2/1", "end": "3/1"} ]
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

	$.get('http://guido.server.org/XXX/staffmap?staff=1');

