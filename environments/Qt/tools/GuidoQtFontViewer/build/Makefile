

QMAKEIOS ?= $(HOME)/Qt5.4.0/5.4/ios/bin/qmake
QMAKE    ?= qmake
PROJECT := GuidoQtFontViewer

ROOT    := ..

.PHONY : macos 

all:
	make macos

macos: macos/$(PROJECT).xcodeproj  ../$(PROJECT).app
	@echo ===================================
	@echo Build $(PROJECT) for mac os done
	@echo ===================================

###################
# MacOS part
###################
macos/$(PROJECT).xcodeproj : $(ROOT)/$(PROJECT).pro
	[ -d macos ] || mkdir macos
	cd macos && $(QMAKE) $(SPEC) $(ROOT)/..

../$(PROJECT).app:
	xcodebuild -jobs 8 -project  macos/$(PROJECT).xcodeproj -target $(PROJECT) -configuration Release
	cp -r macos/Release/$(PROJECT).app .. 

clean:
	rm -rf ../$(PROJECT).app macos