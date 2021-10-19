#
# a makefile to build the guido engine android library
#

J        ?= 6    # parallel jobs
PLATFORM ?= 22

ROOT := ../..

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
	@echo "### Android platform is android-$(PLATFORM)"
	@echo "### Use PLATFORM=xx to change"
	ndk-build -C guido-engine-android -j $(J) APP_PLATFORM=android-$(PLATFORM)
	cp guido-engine-android/libs/armeabi-v7a/libGUIDOEngine.so $(ROOT)/build/lib/libGUIDOEngine_armeabi-v7a.so
	cp guido-engine-android/libs/x86/libGUIDOEngine.so $(ROOT)/build/lib/libGUIDOEngine_x86.so
	cp guido-engine-android/libs/arm64-v8a/libGUIDOEngine.so $(ROOT)/build/lib/libGUIDOEngine_arm64-v8a.so
	cp guido-engine-android/libs/x86_64/libGUIDOEngine.so $(ROOT)/build/lib/libGUIDOEngine_x86_64.so

clean:
	cd guido-engine-android && rm -rf obj libs

test :
	$(foreach abi, $(ABI), echo $(abi))
#	-[ $(ABI) != all ] && echo libGUIDOEngine.$(ABI).so || true

