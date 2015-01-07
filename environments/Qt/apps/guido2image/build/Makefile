

QMAKEIOS ?= $(HOME)/Qt5.4.0/5.4/ios/bin/qmake
QMAKE    ?= qmake
PROJECT := guido2image

ROOT    := ..
SIM     := iphoneos

.PHONY : macos 

all:
	make macos

macos: macos/$(PROJECT).xcodeproj  ../guido2image 
	@echo ===================================
	@echo Build $(PROJECT) for mac os
	@echo ===================================

###################
# MacOS part
###################
macos/$(PROJECT).xcodeproj : $(ROOT)/$(PROJECT).pro $(ROOT)/../GUIDOEngine.shared
	[ -d macos ] || mkdir macos
	cd macos && $(QMAKE) $(SPEC) $(ROOT)/..

../guido2image:
	xcodebuild -jobs 8 -project  macos/$(PROJECT).xcodeproj -target $(PROJECT) -configuration Release
	cp macos/Release/guido2image ../guido2image 

clean:
	rm -rf ../guido2image macos
