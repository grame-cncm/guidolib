
RESOURCES_DIR = $$PWD/rsc
QTROOT = $$PWD/../..
ROOT = $$QTROOT/../..

TARGET = GuidoEditor	 

win32 { TEMPLATE = vcapp }
else  { TEMPLATE = app }
win32:DebugBuild { CONFIG += console }

macx:TARGET = GuidoEditor

MOC_DIR = ./tmpSrc
RCC_DIR = ./tmpSrc
UI_DIR = ./tmpSrc

macx {
	RC_FILE+=$$RESOURCES_DIR/English.lproj/InfoPlist.strings 
	QMAKE_INFO_PLIST = $$PWD/rsc/GuidoEditorInfo.plist
	RSRC.files =  $$RESOURCES_DIR/guido.icns
	RSRC.path =  Contents/Resources

	FONT.files  = $$ROOT/src/guido2.svg
	FONT.files += $$ROOT/src/guido2.ttf
	FONT.path  = Contents/Fonts

	DOC.files  = $$ROOT/doc/refcard/latex/RefCardsParams.pdf
	DOC.files += $$ROOT/doc/refcard/latex/RefCardsTags.pdf
	DOC.path  = Contents/Doc

	QMAKE_BUNDLE_DATA += FONT
	QMAKE_BUNDLE_DATA += DOC
	QMAKE_BUNDLE_DATA += RSRC
}
ios {
	ICON =  rsc/guido.icns
	FONT.files  = $$ROOT/src/guido2.svg
	FONT.files += $$ROOT/src/guido2.ttf
	FONT.path  = Contents/Fonts
	QMAKE_BUNDLE_DATA += FONT
}

win32 { RC_FILE = $$RESOURCES_DIR/GuidoEditor.rc }

# GuidoQt library link for each platform
win32:LIBS += $$QTROOT/libs/GuidoQt.lib
unix:LIBS += -L$$QTROOT/libs -lGuidoQt
ios:LIBS += -L$$QTROOT/libs -lGuidoQt-iOS
macx:LIBS += -L$$QTROOT/libs -lGuidoQt

macx:LIBS += -framework CoreFoundation
INCLUDEPATH += $$QTROOT/libs/GuidoQt/include
QT += widgets printsupport

include( ../GUIDOEngine.shared )

# Input
HEADERS += *.h
SOURCES += *.cpp
RESOURCES += $$RESOURCES_DIR/application.qrc 
FORMS +=  $$RESOURCES_DIR/*.ui
