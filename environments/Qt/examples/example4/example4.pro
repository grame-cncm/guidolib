TEMPLATE = app
SOURCES = main.cpp
#CONFIG += console
macx:CONFIG += app_bundle
DESTDIR = ./bin

# GuidoQt library link for each platform
win32:LIBS += ../GuidoQt/GuidoQt.lib
unix:LIBS += -L.. -lGuidoQt
INCLUDEPATH += ../../libs/GuidoQt/include ../../../../src/engine/include

QT += widgets printsupport


include( ../../GuidoServer/GUIDOEngineLink.pri )
