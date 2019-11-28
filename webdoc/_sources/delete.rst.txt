The DELETE method
=================
.. toctree::
   :maxdepth: 1

| The DELETE method is used to delete an existing score on the server. 
| There is only one request with the delete method. Thise request require a valid score ID.
| Replies to the DELETE query are formatted as JSON.

**Path**
	*ScoreID*

**Response body**
	* in case of success: 
		|  An message formatted as JSON with 'Success' as key where ``ScoreID`` is the ID of the score.
	  	| Example: ``{ "Success": "Successfully removed the score with ID ScoreID." }``
	* in case of error:
		| An error message formatted as JSON with 'Error' as key where ``ScoreID`` is the ID of the score.
	  	| Example: ``{ "Error": "Cannot delete the score with ID ScoreID because it does not exist." }``

**Response code**
	* 200 ("Success")
	* 404 ("Not Found") in case of incorrect score ID.

Example
^^^^^^^^^^^

Using XMLHttpRequest::

	var xhr = new XMLHttpRequest();
	xhr.open('DELETE','http://localhost:8000/da8e1434e155e8a20f328de7d7ea5874d149f5ee', true);
	xhr.onload = function(e) {
		console.log(this.status);
		console.log(this.response);
		if (this.status == 201) {
			success = true;
	        }
	}
	xhr.send();


