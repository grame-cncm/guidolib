RESOURCES_DIR=rsc

win32:DebugBuild:CONFIG += console
TEMPLATE = app
win32:DebugBuild {
	TARGET = GuidoQtViewer_debug
} else {
	TARGET = GuidoQtViewer
}
DESTDIR = ../bin
MOC_DIR = ./tmpSrc
RCC_DIR = ./tmpSrc
UI_DIR = ./tmpSrc
QT += widgets printsupport

win32 {
	TEMPLATE = vcapp
}

macx {
	RC_FILE+=$$RESOURCES_DIR/English.lproj/InfoPlist.strings 
	QMAKE_INFO_PLIST = $$RESOURCES_DIR/QtViewerInfo.plist
}
win32 {
	RC_FILE = $$RESOURCES_DIR/GuidoQtViewer.rc
}

# GuidoQt library link for each platform
win32:LIBS += ../GuidoQt.lib
macx:LIBS += -framework CoreFoundation
unix:LIBS += -L.. -lGuidoQt
INCLUDEPATH += ../GuidoQt/include

include( ../GUIDOEngineLink.pri )

# Input
HEADERS += MainWindow.h SetupDialog.h
SOURCES += main.cpp MainWindow.cpp SetupDialog.cpp
RESOURCES += $$RESOURCES_DIR/application.qrc 
FORMS +=  $$RESOURCES_DIR/setupDialog.ui
