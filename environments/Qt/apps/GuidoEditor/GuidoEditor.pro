
RESOURCES_DIR = $$PWD/rsc
QTROOT = $$PWD/../..
ROOT = $$QTROOT/../..

TARGET = GuidoEditor	 

win32 { TEMPLATE = vcapp }
else  { TEMPLATE = app }
win32:DebugBuild { CONFIG += console }

unix:TARGET = GuidoEditor
macx:TARGET = GuidoEditor

MOC_DIR = ./tmpSrc
RCC_DIR = ./tmpSrc
UI_DIR = ./tmpSrc
macx:DESTDIR = $$PWD
win32:DESTDIR = $$PWD/..

macx {
	# In new version of ios, we can't use Contents folder for anything other than code
	RSRC.path =  Contents/Resources
}
macx|ios {
	CONFIG += c++11
	RC_FILE+=$$RESOURCES_DIR/English.lproj/InfoPlist.strings 
	QMAKE_INFO_PLIST = $$PWD/rsc/GuidoEditorInfo.plist
	RSRC.files =  $$RESOURCES_DIR/guido.icns

	FONT.files  = $$ROOT/src/guido2.svg
	FONT.files += $$ROOT/src/guido2.ttf
	FONT.path  = MyContents/Fonts

	DOC.files  = $$ROOT/doc/refcard/latex/RefCardsParams.pdf
	DOC.files += $$ROOT/doc/refcard/latex/RefCardsTags.pdf
	DOC.path  = MyContents/Doc

	QMAKE_BUNDLE_DATA += FONT
	QMAKE_BUNDLE_DATA += DOC
	QMAKE_BUNDLE_DATA += RSRC
}
ios {
	ios_icon.files = $$files($$PWD/rsc/iosIcons/guidoIcon*.png)
	QMAKE_BUNDLE_DATA += ios_icon
	# In new version of ios, we can't use Contents folder for anything other than code
	RSRC.path =  MyContents/Resources
}

win32 { RC_FILE = $$RESOURCES_DIR/GuidoEditor.rc }

# GuidoQt library link for each platform
win32:LIBS += $$QTROOT/libs/GuidoQt.lib
unix:!ios:LIBS += -L$$QTROOT/libs -lGuidoQt
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
