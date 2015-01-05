SOURCES += main.cpp

QTROOT = $$PWD/../..

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

DESTDIR = $$PWD
QT += widgets printsupport

# GuidoQt library link for each platform
win32 {
	LIBS += $$QTROOT/libs/GuidoQt.lib
}
unix:LIBS += -L$$QTROOT/libs -lGuidoQt
INCLUDEPATH += $$QTROOT/libs/GuidoQt/include

include( ../GUIDOEngine.shared )
