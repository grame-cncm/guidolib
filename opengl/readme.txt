-----------------------------------
  OpenGL support in Guido
-----------------------------------

This folder contains :
	- device : the code for an OpenGl device.
	- applications : sample applications using the OpenGL device

Dependencies for the OpenGL device :
-----------------------------------
	- Freetype: a free library to render fonts
	  see at http://www.freetype.org/

	- FTGL: a library that uses Freetype2 to simplify rendering fonts in OpenGL applications.
	  see at https://sourceforge.net/projects/ftgl/

For the moment, the 'device' folder contains a makefile. It assumes that: 
	- a folder named 'freetype' is available at the same level than the device folder
	- a folder named 'ftgl' is available at the same level than the device folder
These folders should point to the freetype and ftgl distributions. A simple way to do that is 
put your distribution anywhere you want and to make symbolic links in the opengl folder.
The last distributions used are 
	- freetype-2.5.3
	- ftgl-2.1.3~rc5

Note that the OpenGL device has been developped years ago but not maintained. 
It has to be tested again.


TODO :
implement the new VGDevice methods
		virtual	void			SelectPenColor( const VGColor & inColor)	{}
		virtual	void			SelectPenWidth( float witdh)				{}
		virtual	void			PushPenColor( const VGColor & inColor)		{}
		virtual	void			PopPenColor()								{}
		virtual	void			PushPenWidth( float width)					{}
		virtual	void			PopPenWidth()								{}
