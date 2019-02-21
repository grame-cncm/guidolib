
QTROOT = $$PWD/../..
ROOT = $$QTROOT/../..

SOURCES_DIR = $$PWD/src
RESOURCES_DIR = $$PWD/rsc

unix!macx { TARGET = guidoscenecomposer }
else { TARGET = GuidoSceneComposer }

win32 { TEMPLATE = vcapp }
else  { TEMPLATE = app }

win32:DebugBuild { CONFIG += console }


APP_NAME = GuidoSceneComposer
DEFINES += GUIDO_SCENE_COMPOSER_APP

QT += xml widgets printsupport

MOC_DIR = tmpSrc
RCC_DIR = tmpSrc
UI_DIR  = tmpSrc
unix:DESTDIR = $$PWD
win32:DESTDIR = $$PWD/..

macx {
	CONFIG += c++11
	RC_FILE += $$RESOURCES_DIR/English.lproj/InfoPlist.strings 
	QMAKE_INFO_PLIST = $$RESOURCES_DIR/$${APP_NAME}Info.plist
}

win32 {
	RC_FILE = $$RESOURCES_DIR/$${APP_NAME}.rc
}

####### SOURCES ######
SOURCES += 	$$SOURCES_DIR/*.cpp \
			$$SOURCES_DIR/GUI/*.cpp \
			$$SOURCES_DIR/GuidoItemContainer/*.cpp \
			$$SOURCES_DIR/GuidoHighlighter/*.cpp 

####### HEADERS ######
HEADERS += $$SOURCES_DIR/GUI/*.h $$SOURCES_DIR/GuidoItemContainer/*.h $$SOURCES_DIR/GuidoHighlighter/*.h
INCLUDEPATH += $$SOURCES_DIR/GUI $$SOURCES_DIR/GuidoItemContainer $$SOURCES_DIR/GuidoHighlighter

###### RESOURCES ######
RESOURCES += $$RESOURCES_DIR/guido.qrc

####### External libraries link #######
# GraphicsSceneMainWindow link for each platform
win32:LIBS += $$QTROOT/libs/GraphicsSceneMainWindow.lib  $$QTROOT/libs/GuidoQt.lib
unix:LIBS  += -L$$QTROOT/libs -lGraphicsSceneMainWindow -lGuidoQt
ios:LIBS   += -L$$QTROOT/libs -lGraphicsSceneMainWindow-iOS -lGuidoQt-iOS

INCLUDEPATH += $$QTROOT/libs/GraphicsSceneMainWindow/include
macx:ICON =  $$RESOURCES_DIR/guido.icns

# GuidoQt library link for each platform
INCLUDEPATH += $$QTROOT/libs/GuidoQt/include

include( ../GUIDOEngine.shared )

