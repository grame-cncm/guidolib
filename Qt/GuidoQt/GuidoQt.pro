# Input
HEADERS += include/*.h
SOURCES += *.cpp
INCLUDEPATH += include

# Library setting
TEMPLATE = lib
CONFIG += staticlib
DESTDIR = .
MOC_DIR = tmpSrc

win32 {
	TEMPLATE = vclib
}

# Support SVG export
#guidoSupportSVG {
#	QT += svg
#	DEFINES += GUIDO_2_IMAGE_SVG_SUPPORT
#	!build_pass:message("SVG support ok")
#}
#else {
#	!build_pass:message( "To support the Guido export to SVG format, call qmake with option \"CONFIG+=guidoSupportSVG\"" )
#}

QT += widgets printsupport
include( ../GUIDOEngineLink.pri )
