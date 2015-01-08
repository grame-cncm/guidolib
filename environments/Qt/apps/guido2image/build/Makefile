
system	:= $(shell uname -s)
ifeq ($(system), Darwin)
	TARGET = macos
	DEST := ..
else
ifeq ($(system), MINGW32_NT-6.1)
	TARGET = windows
	QMAKEWIN ?= $(shell ../../../qt-win32)
	MSVC 	 ?= $(shell ../../../vs-win32)
	DEST := ../..
else

endif
endif

QMAKEIOS ?= $(HOME)/Qt5.4.0/5.4/ios/bin/qmake
QMAKE    ?= qmake
PROJECT := guido2image
ROOT    := ..

.PHONY : macos win32 

all:
	make $(TARGET)

macos: macos/$(PROJECT).xcodeproj  $(PROJECT) 
	@echo ===================================
	@echo Build $(PROJECT) for mac os
	@echo ===================================

windows: win32/$(PROJECT).vcxproj  $(PROJECT).exe 
	@echo ===================================
	@echo Build $(PROJECT) for win32
	@echo ===================================

###################
# MacOS part
###################
macos/$(PROJECT).xcodeproj : $(ROOT)/$(PROJECT).pro $(ROOT)/../GUIDOEngine.shared
	[ -d macos ] || mkdir macos
	cd macos && $(QMAKE) $(SPEC) $(ROOT)/..

$(PROJECT):
	xcodebuild -jobs 8 -project  macos/$(PROJECT).xcodeproj -target $(PROJECT) -configuration Release
	cp macos/Release/guido2image $(DEST)

###################
# windows part
###################
win32/$(PROJECT).vcxproj : $(ROOT)/$(PROJECT).pro $(ROOT)/../GUIDOEngine.shared
	[ -d win32 ] || mkdir win32
	cd win32 && $(QMAKEWIN) $(SPEC) $(ROOT)/..

$(PROJECT).exe:
	cmd /c "\"$(MSVC)\VC\vcvarsall.bat\" && msbuild win32/$(PROJECT).vcxproj /maxcpucount:4 /p:Configuration=Release"
	cp -R win32/Release/$(PROJECT).exe $(DEST)

clean:
	rm -rf ../$(PROJECT) ../$(PROJECT).exe macos win32
