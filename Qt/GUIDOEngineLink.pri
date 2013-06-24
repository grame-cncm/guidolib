# Universal binary on MAC
#macx:QMAKE_MAC_SDK=macosx10.7
macx:CONFIG+= x86_64

# GUIDOEngine library link for each platform
!DebugBuild {
	macx:LIBS += -F../../cmake/Release -framework GUIDOEngine
	win32:LIBS += ../../cmake/release/GUIDOEngine.lib
	unix:!macx:LIBS += -L../../cmake -lGUIDOEngine
} else {
	macx:LIBS += -F../../cmake/Debug -framework GUIDOEngine
	win32:LIBS += ../../cmake/Debug/GUIDOEngine.lib
	unix:!macx:LIBS += -L../../cmake -lGUIDOEngine
}
INCLUDEPATH += ../../src/include
