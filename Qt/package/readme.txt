--------------------------------------------------------------------------
                       GUIDO Engine Library Qt binding
--------------------------------------------------------------------------

Preliminary note: this package doesn't contain the GUIDO Engine Library 
itself. In order to use the GUIDO-Qt binding classes, you must first
download the GUIDOEngine "dev kit" packages for your platform.

GUIDO-Qt binding classes have been developped to use the
Guidolib in Qt applications. This binding basically allows you
to parse Guido Music Notation files or strings and draw the resulting 
GUIDO Score with a QPainter ; "ready-to-use" QGuidoWidget and
QGuidoGraphicsItem are already implemented.

As Qt is a cross-platform toolkit, you can use the Guido Qt binding 
classes on any platform. You need to install the platfrom native Guidolib
before using the Guido-Qt binding. Next you just have to link your Qt 
application against the  corresponding platfrom native Guidolib.

--------------------------------------------------------------------------
			Package contents
--------------------------------------------------------------------------

- doc: 
	- documentation on the GUIDO Music Notation ;
	- documentation on the GUIDOEngine API ;
	- documentation on the GUIDO-Qt binding classes.
- src:
	- GuidoQt: the static library containing the GUIDO-Qt binding 
	classes ;
	- include: headers from the GUIDOEngine ;
	- other folders contains Qt applications using the GUIDO-Qt 
	binding classes. NOTE: YOU NEED THE NATIVE GUIDOENGINE LIBRARY
	TO BUILD THOSE APPLICATIONS. By default, the application will
	look for the GUIDOEngine library in the guidolib-X-devkit-Qt/src
	folder, so you should manually put it there.
	- GraphicsSceneMainWindow is a static library used by the 
	GuidoSceneComposer application. It doesn't use the GUIDOEngine.
	(it is notably used by another GRAME application, 
	FaustGIDE "A Graphic Faust IDE")
- guido2.ttf is the font used by the GUIDO Engine. You'll need to install 
	this font on your system.

--------------------------------------------------------------------------
--------------------------------------------------------------------------

The source code of the GUIDO Qt binding classes are available from 
sourceforge under the LGPL2.1 license (see the LICENSE.LGPL-2.1 file).

For more informations : http://sourceforge.net/projects/guidolib