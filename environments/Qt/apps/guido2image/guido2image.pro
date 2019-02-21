SOURCES += main.cpp

QTROOT = $$PWD/../..
macx:DESTDIR = $$PWD
win32:DESTDIR = $$PWD/..

win32 {
	SOURCES += XGetopt.cpp
	HEADERS += XGetopt.h
}
else {
	DEFINES += USE_UNISTD
	CONFIG += c++11
}

TEMPLATE = app
win32 {
	TEMPLATE = vcapp
}
CONFIG += console
macx:CONFIG -= app_bundle

QT += widgets printsupport

# GuidoQt library link for each platform
win32 {
	LIBS += $$QTROOT/libs/GuidoQt.lib
}
unix:LIBS += -L$$QTROOT/libs -lGuidoQt
INCLUDEPATH += $$QTROOT/libs/GuidoQt/include

include( ../GUIDOEngine.shared )
