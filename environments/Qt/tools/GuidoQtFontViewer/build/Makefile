
system	:= $(shell uname -s)
ifeq ($(system), Darwin)
	TARGET = macos
	QMAKEIOS ?= $(HOME)/Qt5.4.0/5.4/ios/bin/qmake
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


QMAKE    ?= qmake
PROJECT := GuidoQtFontViewer

ROOT    := ..

.PHONY : macos win32


all:
	make $(TARGET)

macos: macos/$(PROJECT).xcodeproj  $(PROJECT).app
	@echo ===================================
	@echo Build $(PROJECT) for mac os done
	@echo ===================================

windows: win32/$(PROJECT).vcxproj  $(PROJECT).exe 
	@echo ===================================
	@echo Build $(PROJECT) for win32
	@echo ===================================

###################
# MacOS part
###################
macos/$(PROJECT).xcodeproj : $(ROOT)/$(PROJECT).pro
	[ -d macos ] || mkdir macos
	cd macos && $(QMAKE) $(SPEC) $(ROOT)/..

$(PROJECT).app:
	xcodebuild -jobs 8 -project  macos/$(PROJECT).xcodeproj -target $(PROJECT) -configuration Release
	cp -r macos/Release/$(PROJECT).app $(DEST)

###################
# windows part
###################
win32/$(PROJECT).vcxproj : $(ROOT)/$(PROJECT).pro
	[ -d win32 ] || mkdir win32
	cd win32 && $(QMAKEWIN) $(SPEC) $(ROOT)/..

$(PROJECT).exe:
	cmd /c "\"$(MSVC)\VC\vcvarsall.bat\" && msbuild win32/$(PROJECT).vcxproj /maxcpucount:4 /p:Configuration=Release"
	cp -R win32/Release/$(PROJECT).exe $(DEST)

clean:
	rm -rf $(DEST)/$(PROJECT).* macos win32