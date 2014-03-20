Server Options
===============================================

The following options can be passed to the server on startup:

	- ``--port``
		portnum : sets the communication port number
		
		*default value*: 8000

	- ``--initfile``
		the location of the (optional) server initiation file. See the sample init file guidohttpdserver.ini in the GUIDO source for inspiration.
		
		*default value*: A file called guidohttpdserver.ini in the directory of the executable.

	- ``--daemon``
		used with nohup to make this run as a daemon. Daemon-like behavior can be achieved with the options below:

		- ``--root``: run the server in the root directory
		- ``--closestdin``: close standard in
		- ``--closestdout``: close standard out
		- ``--closestderr``: close standard error

	- ``--logfile``
		logfile mode. Use 0 for Apache-like or 1 for XML.

		*default value*: 0 (Apache-like)

	- ``--cachedir``
		name of the cache directory
		
		*default value*: A directory named ``cache`` in the directory of the current executable
	
	- ``--svgfontfile``
		name of the svg font file.
		
		*default value*: A file called guido2.svg in the directory of the current executable

	- ``--version``
		version of the server and of guido

	- ``--help``
		display all of these options in a help message


Here's an example of how to make the cachedir in the file ``/home/guidofriend/mychachedir``

	./guidohttpserver --cachedir /home/guidofriend/mycachedir


Here's an example of how to run the server in daemon mode

	sudo nohup ./guidohttpserver --daemon &
