
TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .
win32 {
	TEMPLATE = vcapp
}
CONFIG += console
macx:CONFIG -= app_bundle
macx:QMAKE_CXXFLAGS += -mmacosx-version-min=10.6

SOURCES += main.cpp

QT += widgets 
