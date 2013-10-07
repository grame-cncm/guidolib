Overview
===============================================

The HTTP GUIDO Engine is a web server that provides music score layout services, based on the
the `GUIDO Engine library <http://guidolib.sourceforge.net/>`_ and on the :ref:`gmn` format. These services are availble using HTTP requests. The corresponding API is inspired by a RESTful design and is close to the `C/C++ API <http://guidolib.sourceforge.net/doc/guidolib/>`_.

The semantic of the operations is carried by the HTTP method (POST, GET) and their scope is indicated by the URI:

• POST is used to create a new score, 
• GET is used to get a score or score attributes

Typical examples of transactions with the server could be as follows:

• a client POST GMN code to a server (e.g. located at ``http://guido.server.org``). When the code is correct, it gets a unique identifier back from the server (let's say 'XXX'), otherwise the server returns an error code.
• in case of success, the client can access the score and its properties with the 'GET' method and simple paths (the part of the URI to the right of the hostname) that start with the identifier previously returned by the server ('XXX' in the following examples): 

	• ``http://guido.server.org/XXX/`` 			to get an image of the score
	• ``http://guido.server.org/XXX/pagecount``	to get the count of pages
	• ``http://guido.server.org/XXX/gmn``		to get the gmn code
	• etc.

• options can be used and combined in conjunction to the GET method e.g.:

	• ``http://guido.server.org/XXX?page=2`` 	to get an image of the second page of the score
	• ``http://guido.server.org/XXX?page=2&format=png`` to get a png image of the second page
	• etc.


.. _gmn:

GUIDO Music Notation
--------------------

The `GUIDO Music Notation format <http://guidolib.sourceforge.net/doc/GUIDO-Music%20Notation%20Format.html>`_ is a formal language for score level music representation. It is a plain-text, i.e. readable and platform independent format capable of representing all information contained in conventional musical scores. The basic GUIDO Format is very flexible and can be easily extended and adapted to capture a wide variety of musical features beyond conventional musical notation (CMN). The GUIDO design is strongly influenced by objective oriented programming to facilitate an adequate representation of musical material, from tiny motives up to complex symphonic scores.

