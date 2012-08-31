SOURCES += ../../server/main.cpp ../../server/HTTPDServer.cpp guido2img.cpp ../../server/guidosession.cpp ../../server/guidouser.cpp ../../server/json.c ../../server/jsonhelper.cpp ../../server/utilities.cpp engine.cpp
HEADERS += ../../server/HTTPDServer.h guido2img.h ../../server/guidosession.h ../../server/guidouser.h ../../server/json.h ../../server/jsonhelper.h ../../server/utilities.h engine.h

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
