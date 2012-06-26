SOURCES += main.cpp HTTPDServer.cpp guido2img.cpp
HEADERS += HTTPDServer.h guido2img.h

TEMPLATE = app
win32 {
	TEMPLATE = vcapp
}
CONFIG += console
macx:CONFIG -= app_bundle
DESTDIR = .

# GuidoQt library link for each platform
win32:LIBS += ../GuidoQt/GuidoQt.lib
unix:LIBS += -L../GuidoQt -lGuidoQt
unix:LIBS += -lmicrohttpd 
INCLUDEPATH += ../GuidoQt/include


macx:QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.6.sdk
macx:CONFIG+=x86_64

# GUIDOEngine library link for each platform
macx:LIBS += -F../../cmake/Release -framework GUIDOEngine
win32:LIBS += ../../cmake/release/GUIDOEngine.lib
unix:!macx:LIBS += -L../../cmake -lGUIDOEngine
INCLUDEPATH += ../../src/include
