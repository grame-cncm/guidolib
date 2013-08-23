SOURCES += ../../server/main.cpp ../../server/server.cpp guido2img.cpp ../../server/guidosession.cpp ../../server/utilities.cpp engine.cpp
HEADERS += ../../server/server.h guido2img.h ../../server/guidosession.h ../../server/utilities.h engine.h

# json
SOURCES += ../../server/json_array.cpp ../../server/json_object.cpp ../../server/json_stream.cpp ../../server/json_value.cpp
HEADERS += ../../server/json_array.h ../../server/json_element.h ../../server/json_object.h ../../server/json_stream.h ../../server/json_value.h

# json parse
#SOURCES += ../../server/jsonparse.cpp ../../server/jsonlex.cpp ../../server/json_parser.cpp
#HEADERS += ../../server/json_parser.h ../../server/jsonparse.hpp

include(flex.pri)
include(bison.pri)
FLEXSOURCES = ../../server/json.l
BISONSOURCES = ../../server/json.y


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
#unix:LIBS += -ljson2osc
INCLUDEPATH += ../GuidoQt/include
INCLUDEPATH += ../../server


#macx:QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.6.sdk
#macx:CONFIG+=x86_64

include( ../GUIDOEngineLink.pri )

# GUIDOEngine library link for each platform
#macx:LIBS += -F../../build/MacOS/Release -framework GUIDOEngine
#win32:LIBS += ../../cmake/release/GUIDOEngine.lib
#unix:!macx:LIBS += -L../../cmake -lGUIDOEngine
#INCLUDEPATH += ../../src/include
