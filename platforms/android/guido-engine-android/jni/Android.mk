#### Prebuilt static library ####

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := GUIDOEngine-prebuilt

LOCAL_SRC_FILES 	:= $(addprefix ../, $(wildcard ../../../src/engine/*/*.cpp))
LOCAL_EXPORT_C_INCLUDES := $(addprefix $(LOCAL_PATH)/../../../../src/engine/, include devices)
LOCAL_C_INCLUDES 	:= $(wildcard ../../../src/engine/*)
LOCAL_CPPFLAGS		:= -Dandroid -frtti -DINDEPENDENTSVG -DJSON_ONLY

include $(BUILD_STATIC_LIBRARY)



#### Shared library ####

include $(CLEAR_VARS)
LOCAL_MODULE := GUIDOEngine-android

LOCAL_SRC_FILES 	:= $(addprefix ../, $(wildcard ../../../lang/java/jni/*.cpp))
LOCAL_SRC_FILES 	+= $(addprefix ../, $(wildcard ../../../lang/java/jni/svg/*.cpp))
LOCAL_C_INCLUDES 	:= $(wildcard ../../../lang/java/jni/*)
LOCAL_C_INCLUDES 	+= $(wildcard ../../../lang/java/jni/svg/*)
LOCAL_CPPFLAGS 		:= -Dandroid

LOCAL_STATIC_LIBRARIES = GUIDOEngine-prebuilt

include $(BUILD_SHARED_LIBRARY)