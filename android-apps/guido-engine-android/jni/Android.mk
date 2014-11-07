LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := GUIDOEngine

LOCAL_SRC_FILES 	:= $(addprefix ../, $(wildcard ../../src/engine/*/*.cpp))
LOCAL_C_INCLUDES 	:= $(wildcard ../../src/engine/*)
LOCAL_CPPFLAGS		:= -Dandroid -frtti -DINDEPENDENTSVG -DJSON_ONLY

include $(BUILD_STATIC_LIBRARY)
