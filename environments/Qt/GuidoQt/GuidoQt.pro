# Input
HEADERS += include/*.h
SOURCES += *.cpp
INCLUDEPATH += include

# Library setting
TEMPLATE = lib
CONFIG += staticlib

DESTDIR = ..
MOC_DIR = tmpSrc

win32 {
	TEMPLATE = vclib
	TARGET = GuidoQtD
}


QT += widgets printsupport
include( ../GUIDOEngineLink.pri )
