SOURCES += ../../server/main.cpp ../../server/server.cpp guido2img.cpp ../../server/guidosession.cpp ../../server/utilities.cpp ../../server/profport.c engine.cpp 
HEADERS += ../../server/server.h guido2img.h ../../server/guidosession.h ../../server/utilities.h engine.h ../../server/profport.h ../../server/tinydir.h
DEFINES += "JSON_ONLY"

TEMPLATE = app
win32 {
	TEMPLATE = vcapp
}
QT += widgets printsupport
CONFIG += console
macx:CONFIG -= app_bundle
#DESTDIR = ../bin

# GuidoQt library link for each platform
win32:LIBS += ../GuidoQt.lib
unix:LIBS += -L.. -lGuidoQt -L/usr/local/lib
unix:LIBS += -lmicrohttpd -ljson -lcrypto -lcurl
INCLUDEPATH += ../GuidoQt/include
INCLUDEPATH += ../../server
INCLUDEPATH += /usr/local/include

include( ../GUIDOEngineLink.pri )
