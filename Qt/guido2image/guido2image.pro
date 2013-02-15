SOURCES += main.cpp

win32 {
	SOURCES += XGetopt.cpp
	HEADERS += XGetopt.h
}
else {
	DEFINES += USE_UNISTD
}

TEMPLATE = app
win32 {
	TEMPLATE = vcapp
}
CONFIG += console
macx:CONFIG -= app_bundle
DESTDIR = ../bin

QT += widgets printsupport

# GuidoQt library link for each platform
win32 {
	DebugBuild{
		LIBS += ../GuidoQt/Debug/GuidoQt.lib
	}
	else {
		LIBS += ../GuidoQt/Release/GuidoQt.lib
	}
}
unix:LIBS += -L../GuidoQt -lGuidoQt
INCLUDEPATH += ../GuidoQt/include

include( ../GUIDOEngineLink.pri )
