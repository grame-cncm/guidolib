SOURCES_DIR = src
RESOURCES_DIR = rsc

win32 {
	TEMPLATE = vcapp
}
else {
	TEMPLATE = app
}

win32:DebugBuild {
 	TARGET = GuidoSceneComposer_debug
 	CONFIG += console	 
}
else {	 
	TARGET = GuidoSceneComposer	 
}

APP_NAME = GuidoSceneComposer
DEFINES += GUIDO_SCENE_COMPOSER_APP

QT += xml

DESTDIR = ../bin
MOC_DIR = ./tmpSrc
RCC_DIR = ./tmpSrc
UI_DIR = ./tmpSrc

macx {
	RC_FILE += $$RESOURCES_DIR/English.lproj/InfoPlist.strings 
	QMAKE_INFO_PLIST = $$RESOURCES_DIR/$${APP_NAME}Info.plist
}

win32 {
	RC_FILE = $$RESOURCES_DIR/$${APP_NAME}.rc
}

####### SOURCES ######
SOURCES += $$SOURCES_DIR/*.cpp $$SOURCES_DIR/GUI/*.cpp $$SOURCES_DIR/GuidoItemContainer/*.cpp $$SOURCES_DIR/GuidoHighlighter/*.cpp 

####### HEADERS ######
HEADERS += $$SOURCES_DIR/GUI/*.h $$SOURCES_DIR/GuidoItemContainer/*.h $$SOURCES_DIR/GuidoHighlighter/*.h
INCLUDEPATH += $$SOURCES_DIR/GUI $$SOURCES_DIR/GuidoItemContainer $$SOURCES_DIR/GuidoHighlighter

###### RESOURCES ######
RESOURCES += $$RESOURCES_DIR/guido.qrc

####### External libraries link #######
# GraphicsSceneMainWindow link for each platform
win32:LIBS += ../GraphicsSceneMainWindow/GraphicsSceneMainWindow.lib
unix:LIBS += -L../GraphicsSceneMainWindow -lGraphicsSceneMainWindow

INCLUDEPATH += ../GraphicsSceneMainWindow/include
macx:ICON =  rsc/guido.icns

# GuidoQt library link for each platform
win32:LIBS += ../GuidoQt/GuidoQt.lib
unix:LIBS += -L../GuidoQt -lGuidoQt

INCLUDEPATH += ../GuidoQt/include

include( ../GUIDOEngineLink.pri )

