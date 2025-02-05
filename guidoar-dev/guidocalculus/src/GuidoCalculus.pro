APP_NAME = GuidoCalculus
DEFINES += GUIDO_CALCULUS_APP
include( shared.pri )

RESOURCES_DIR=rsc
isEmpty( GUIDOLIB ) { GUIDOLIB = ../../../guidolib }
GUIDOLIBS = $$GUIDOLIB/environments/Qt/libs
GUIDOQT	 = $$GUIDOLIB/environments/Qt/libs/GuidoQt
GSMW	 = $$GUIDOLIB/environments/Qt/libs/GraphicsSceneMainWindow

#######
CONFIG += no_keywords
QT += widgets printsupport

!exists( $$GUIDOLIB ) {
	error( "Can't find Guido Library at \"$$GUIDOLIB\". Call qmake with option: \"GUIDOLIB=./path/to/guidolib\" " )
} 

####### External libraries link #######

# GraphicsSceneMainWindow link for each platform
win32:!win32-g++ {
	LIBS += $$GSMW/GraphicsSceneMainWindow.lib
}
win32-g++ {
	LIBS += -L$$GSMW  -lGraphicsSceneMainWindow
}
unix {
	LIBS += -L$$GUIDOLIBS -lGraphicsSceneMainWindow
}
INCLUDEPATH += $$GSMW/include

# GuidoQt library link for each platform
win32:!win32-g++:LIBS 	+=   $$GUIDOQT/GuidoQt.lib
win32-g++:LIBS 	+= -L$$GUIDOQT/ -lGuidoQt
unix:LIBS 		+= -L$$GUIDOLIBS -lGuidoQt
INCLUDEPATH 	+=  $$GUIDOQT/include

# GUIDOEngine library link for each platform
macx:LIBS 	+= -F$$GUIDOLIB/build/MacOS/Release -framework GUIDOEngine
win32:!win32-g++:LIBS 	+=   $$GUIDOLIB/cmake/release/GUIDOEngine.lib
win32-g++:LIBS 	+= -L$$GUIDOLIB/cmake -lguidoengine
unix:!macx:LIBS += -lGUIDOEngine
INCLUDEPATH 	+= $$GUIDOLIB/src/engine/include

###### GUIDO AR ######
DEFINES += USES_GUIDO_AR
GUIDO_AR_PATH = ../..

macx {
	ICON =  $$RESOURCES_DIR/guido.icns
	FONT.files  = $$RESOURCES_DIR/guido2.svg
	FONT.files += $$RESOURCES_DIR/guido2.ttf
	FONT.path  = Contents/Resources
	QMAKE_BUNDLE_DATA += FONT
}

#### guidoar library link
macx:LIBS += -F$$GUIDO_AR_PATH/build/macos/Release -framework guidoar
win32:!win32-g++:LIBS += $$GUIDO_AR_PATH/cmake/release/guidoar.lib
win32-g++:LIBS += -L$$GUIDO_AR_PATH/cmake -lguidoar
unix:!macx:LIBS += -L$$GUIDO_AR_PATH/cmake -lguidoar
INCLUDEPATH += $$GUIDO_AR_PATH/src/lib $$GUIDO_AR_PATH/src/interface $$GUIDO_AR_PATH/src/midi $$GUIDO_AR_PATH/src/visitors $$GUIDO_AR_PATH/src/parser $$GUIDO_AR_PATH/src/guido $$GUIDO_AR_PATH/src/guido/abstract

SOURCES_DIR = .

###### SOURCES ######
SOURCES 	+=  main.cpp GUI/*.cpp GuidoAR/*.cpp GuidoHighlighter/*.cpp GuidoItemContainer/*.cpp 

###### HEADERS ######
HEADERS 	+= GUI/*.h GuidoAR/*.h GuidoHighlighter/*.h GuidoItemContainer/*.h
INCLUDEPATH += GUI GuidoAR GuidoHighlighter GuidoItemContainer

###### RESOURCES ######
RESOURCES += rsc/guidoar.qrc rsc/guido.qrc
