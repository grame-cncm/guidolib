TEMPLATE = app
SOURCES = main.cpp
QT = widgets

# GuidoQt library link for each platform
win32:LIBS += ../GuidoQt/GuidoQt.lib
unix:LIBS += -L../GuidoQt -lGuidoQt
INCLUDEPATH += ../../libs/GuidoQt/include ../../../../src/engine/include

include( ../../GuidoServer/GUIDOEngineLink.pri )
