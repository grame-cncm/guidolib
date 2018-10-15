#
# a makefile to build the guido engine android library
#

ABI ?= armeabi-v7a
ifneq ($(ABI),)
	APP_ABI = APP_ABI=$(ABI)
endif

all: android

help:
	@echo "make the guido engine library for android"
	@echo "     you can specify an ABI using ABI=xxx where xxx should be in 'armeabi' 'armeabi-v7a' 'arm64-v8a' 'x86' 'x86_64' 'mips' 'mips64'"
	@echo "     default ABI is armeabi"
	@echo "     use ABI=all to compile for all architectures"

#===============================================================
# building the library for Android
#===============================================================
android :
	ndk-build -C guido-engine-android -j 4 $(APP_ABI)
	-[ $(ABI) ] && cp guido-engine-android/libs/$(ABI)/libGUIDOEngine.so libGUIDOEngine.$(ABI).so

test :
	-[ $(ABI) != all ] && echo libGUIDOEngine.$(ABI).so || true
