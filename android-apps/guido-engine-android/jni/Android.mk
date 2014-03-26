LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := GUIDOEngine

LOCAL_SRC_FILES 	:= $(addprefix ../, $(wildcard ../../src/engine/*/*.cpp))
LOCAL_C_INCLUDES 	:= $(wildcard ../../src/engine/*)
LOCAL_CPPFLAGS		:= -Dandroid -frtti -DINDEPENDENTSVG

#$(warning Value of LOCAL_SRC_FILES is '$(LOCAL_SRC_FILES)')
#$(warning Value of LOCAL_PATH is '$(LOCAL_PATH)')
include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_STATIC_LIBRARY)
