TEMPLATE = app
SOURCES = main.cpp
CONFIG += console
macx:CONFIG -= app_bundle

# GuidoQt library link for each platform
win32:LIBS += ../GuidoQt/GuidoQt.lib
unix:LIBS += -L../GuidoQt -lGuidoQt
INCLUDEPATH += ../GuidoQt/include

include( ../GUIDOEngineLink.pri )