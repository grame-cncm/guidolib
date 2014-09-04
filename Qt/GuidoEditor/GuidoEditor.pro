RESOURCES_DIR=rsc

win32 {
	TEMPLATE = vcapp
}
else {
	TEMPLATE = app
}

 win32:DebugBuild {
 	TARGET = GuidoEditor_debug
 	CONFIG += console	 
 }
 else {	 
	TARGET = GuidoEditor	 
}

unix:DebugBuild {
	TARGET = guidoeditord
} else {
	TARGET = guidoeditor
}

DESTDIR = ../bin
MOC_DIR = ./tmpSrc
RCC_DIR = ./tmpSrc
UI_DIR = ./tmpSrc

macx {
	message("generates project for MacOS")
	RC_FILE+=$$RESOURCES_DIR/English.lproj/InfoPlist.strings 
	QMAKE_INFO_PLIST = $$RESOURCES_DIR/GuidoEditorInfo.plist
	ICON =  $$RESOURCES_DIR/guido.icns
	FONT.files  = ../../src/guido2.svg
	FONT.files += ../../src/guido2.ttf
	FONT.path  = Contents/Fonts
	QMAKE_BUNDLE_DATA += FONT
}
ios {
	message("generates project for iOS")
#	RC_FILE+=$$RESOURCES_DIR/English.lproj/InfoPlist.strings 
#	QMAKE_INFO_PLIST = $$RESOURCES_DIR/GuidoEditorInfo.plist
	ICON =  $$RESOURCES_DIR/guido.icns
	FONT.files  = ../../src/guido2.svg
	FONT.files += ../../src/guido2.ttf
	FONT.path  = Contents/Fonts
	QMAKE_BUNDLE_DATA += FONT
}

win32 {
	RC_FILE = $$RESOURCES_DIR/GuidoEditor.rc
}

# GuidoQt library link for each platform
win32 {
	DebugBuild{
		LIBS += ../GuidoQt.lib
	}
	else {
		LIBS += ../GuidoQt.lib
	}
}

macx:LIBS += -framework CoreFoundation
unix:LIBS += -L.. -lGuidoQt
INCLUDEPATH += ../GuidoQt/include
QT += widgets printsupport

include( ../GUIDOEngineLink.pri )

# Input
HEADERS += *.h
SOURCES += *.cpp
RESOURCES += $$RESOURCES_DIR/application.qrc 
FORMS +=  $$RESOURCES_DIR/*.ui
