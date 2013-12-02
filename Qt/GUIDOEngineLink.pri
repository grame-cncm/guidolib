# Universal binary on MAC
#macx:QMAKE_MAC_SDK=macosx10.7
macx:CONFIG+= i386 x86_64

QMAKE_CXXFLAGS += -mmacosx-version-min=10.7
CONFIG += c++11

win32: {
	contains(QMAKE_HOST.arch, x86): {
	QMAKE_LFLAGS *= /MACHINE:X86
	!DebugBuild { GUIDO = ../../build/win32/release/GUIDOEngine.lib }
	else		{ GUIDO = ../../build/win32/debug/GUIDOEngineD.lib } 
	}
	contains(QMAKE_HOST.arch, x86_64): {
	QMAKE_LFLAGS *= /MACHINE:X64
	!DebugBuild { GUIDO = ../../build/win64/release/GUIDOEngine64.lib }
	else		{ GUIDO = ../../build/win64/debug/GUIDOEngine64D.lib }
	}
}

# GUIDOEngine library link for each platform
!DebugBuild {
	macx:LIBS += -F../../build/MacOS/Release -framework GUIDOEngine
	unix:!macx:LIBS += -L../../cmake -lGUIDOEngine
} else {
	macx:LIBS += -F../../build/MacOS/Debug -framework GUIDOEngine
	unix:!macx:LIBS += -L../../cmake -lGUIDOEngine
}
win32:LIBS += $$GUIDO

INCLUDEPATH += ../../src/include
