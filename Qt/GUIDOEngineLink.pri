# Universal binary on MAC
#macx:QMAKE_MAC_SDK=macosx10.7
#macx:CONFIG+= x86_64

# GUIDOEngine library link for each platform
!DebugBuild {
	macx:LIBS += -F../../build/MacOS/Release -framework GUIDOEngine
	win32:LIBS += ../../build/win32/release/GUIDOEngine.lib
	unix:!macx:LIBS += -L../../cmake -lGUIDOEngine
} else {
	macx:LIBS += -F../../build/MacOS/Debug -framework GUIDOEngine
	win32:LIBS += ../../build/win32/Debug/GUIDOEngine.lib
	unix:!macx:LIBS += -L../../cmake -lGUIDOEngine
}
INCLUDEPATH += ../../src/include
