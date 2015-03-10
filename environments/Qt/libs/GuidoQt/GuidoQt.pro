
# root of the Guido project
ROOT = ../../../..

TARGET = GuidoQt	 

macx {
	CONFIG+= i386 x86_64
	QMAKE_CXXFLAGS += -mmacosx-version-min=10.6
}

ios {
	TARGET = GuidoQt-iOS
	QMAKE_CXXFLAGS += -DIOS
	CONFIG += c++11
	CONFIG+= arm64 armv7 armv7s
}

android {
        TARGET = GuidoQt-android
        LIBS += -L$$PWD/$$ROOT/platforms/android/guido-engine-android/libs/armeabi -lGUIDOEngine
}

INCLUDEPATH += $$ROOT/src/engine/include 
INCLUDEPATH += $$PWD/include


# Input
HEADERS += $$PWD/include/*.h
SOURCES += *.cpp

# Library setting
TEMPLATE = lib
!android:CONFIG += staticlib

DESTDIR = $$PWD/..
MOC_DIR = tmpSrc

win32:TEMPLATE = vclib

QT += widgets printsupport
