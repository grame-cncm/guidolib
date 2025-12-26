start:
	@ echo "Target list : "
	@ echo "     macos : bundles GuidoCalculus"
	@ echo "     win32 : TODO"
	@ echo "     linux : TODO"

#include makeBundle

APP_PATH := ../bin
APP		 := GuidoCalculus
QT_PLUGIN_PATH 		:= /Developer/Applications/Qt/plugins/
QT_FRAMEWORK_PATH 	:= /Library/Frameworks
GUIDOLIB = ../../../guidolib/cmake/Release

striplist = $(shell find $(APP).app -name "*_debug*")

macos:
	-[ -d $(APP).app ] && rm -rf  $(APP).app
	cp -r ../bin/$(APP).app .
	macdeployqt GuidoCalculus.app
	

strip:
	rm -rf $(striplist)
