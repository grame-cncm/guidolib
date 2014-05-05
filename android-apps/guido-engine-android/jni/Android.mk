LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := GUIDOEngine

LOCAL_SRC_FILES 	:= $(addprefix ../, $(wildcard ../../src/engine/[^d][a-z]*/*.cpp))
LOCAL_SRC_FILES 	:= $(LOCAL_SRC_FILES) $(addprefix ../, $(wildcard ../../src/engine/devices/SVG*.cpp))
LOCAL_C_INCLUDES 	:= $(wildcard ../../src/engine/*)
LOCAL_CPPFLAGS		:= -Dandroid -frtti -DINDEPENDENTSVG

include $(BUILD_STATIC_LIBRARY)
