LOCAL_PATH := $(call my-dir)

# first, we generate guido2.h from the svg file
# $(shell (xxd -i ../../src/guido2.svg > $(LOCAL_PATH)/guido2.h))

# then, we link to the premade lib GUIDO-engine
# make sure to make this via nkd-build in the guido-engine-android folder
# also, make sure to change x86 to armeabi if you are using an armeabi architecture or vice versa
include $(CLEAR_VARS)
LOCAL_MODULE := GUIDOEngine-prebuilt
LOCAL_SRC_FILES := ../../guido-engine-android/obj/local/armeabi/libGUIDOEngine.a
LOCAL_EXPORT_C_INCLUDES := $(addprefix $(LOCAL_PATH)/../../../src/engine/, include devices)
include $(PREBUILT_STATIC_LIBRARY)

# local library
include $(CLEAR_VARS)
LOCAL_MODULE := GUIDOEngine-android

LOCAL_SRC_FILES 	:= $(addprefix ../, $(wildcard ../../java/jni/*.cpp))
LOCAL_SRC_FILES 	+= $(addprefix ../, $(wildcard ../../java/jni/svg/*.cpp))
LOCAL_C_INCLUDES 	:= $(wildcard ../../java/jni/*)
LOCAL_C_INCLUDES 	+= $(wildcard ../../java/jni/svg/*)
LOCAL_CPPFLAGS 		:= -Dandroid

LOCAL_STATIC_LIBRARIES = GUIDOEngine-prebuilt
include $(BUILD_SHARED_LIBRARY)