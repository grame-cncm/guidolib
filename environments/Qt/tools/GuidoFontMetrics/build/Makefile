

QMAKEIOS ?= $(HOME)/Qt5.4.0/5.4/ios/bin/qmake
QMAKE    ?= qmake
PROJECT := guidofontmetrics

ROOT    := ..

.PHONY : macos 

all:
	make macos

macos: macos/$(PROJECT).xcodeproj  ../$(PROJECT)
	@echo ===================================
	@echo Build $(PROJECT) for mac os done
	@echo ===================================

###################
# MacOS part
###################
macos/$(PROJECT).xcodeproj : $(ROOT)/$(PROJECT).pro
	[ -d macos ] || mkdir macos
	cd macos && $(QMAKE) $(SPEC) $(ROOT)/..

../$(PROJECT):
	xcodebuild -jobs 8 -project  macos/$(PROJECT).xcodeproj -target $(PROJECT) -configuration Release
	cp macos/Release/$(PROJECT) .. 

clean:
	rm -rf ../$(PROJECT) macos