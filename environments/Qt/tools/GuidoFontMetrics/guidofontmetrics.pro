
TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .
win32 {
	TEMPLATE = vcapp
}
CONFIG += console
macx:CONFIG -= app_bundle
macx:QMAKE_MAC_SDK = macosx10.9

SOURCES += main.cpp

QT += widgets 
