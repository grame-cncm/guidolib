TEMPLATE = app
DESTDIR = ../bin
DEPENDPATH += .
INCLUDEPATH += .
win32 {
	TEMPLATE = vcapp
}
CONFIG += console
macx:CONFIG -= app_bundle

SOURCES += main.cpp

QT += widgets 
