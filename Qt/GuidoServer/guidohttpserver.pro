SOURCES += ../../server/main.cpp ../../server/server.cpp guido2img.cpp ../../server/guidosession.cpp ../../server/utilities.cpp engine.cpp
HEADERS += ../../server/server.h guido2img.h ../../server/guidosession.h ../../server/utilities.h engine.h

### uncomment below if we ever need to include bison/flex stuff
#include(flex.pri)
#include(bison.pri)
#FLEXSOURCES = ../../server/json.l
#BISONSOURCES = ../../server/json.y


TEMPLATE = app
win32 {
	TEMPLATE = vcapp
}
QT += widgets printsupport
CONFIG += console
macx:CONFIG -= app_bundle
DESTDIR = ../bin

# GuidoQt library link for each platform
win32:LIBS += ../GuidoQt.lib
unix:LIBS += -L.. -lGuidoQt -L/usr/local/lib
unix:LIBS += -lmicrohttpd
unix:LIBS += -ljson2osc
INCLUDEPATH += ../GuidoQt/include
INCLUDEPATH += ../../server
INCLUDEPATH += /usr/local/include
QMAKE_CXXFLAGS += -DJSON_ONLY

#macx:QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.6.sdk
#macx:CONFIG+=x86_64

include( ../GUIDOEngineLink.pri )

# GUIDOEngine library link for each platform
#macx:LIBS += -F../../build/MacOS/Release -framework GUIDOEngine
#win32:LIBS += ../../cmake/release/GUIDOEngine.lib
#unix:!macx:LIBS += -L../../cmake -lGUIDOEngine
#INCLUDEPATH += ../../src/include
