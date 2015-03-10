#### Prebuilt static library ####

LOCAL_PATH := $(call my-dir)
SRC := $(LOCAL_PATH)/../../../../src/engine

include $(CLEAR_VARS)

LOCAL_MODULE := GUIDOEngine-prebuilt

# SRC var is a trick to use wildcard in android.mk
# if no wildcard, you have better to use relative path, conforming to android doc 
LOCAL_SRC_FILES 	:= $(subst $(LOCAL_PATH)/,,$(wildcard $(SRC)/*/*.cpp))
LOCAL_EXPORT_C_INCLUDES := $(addprefix $(SRC)/, include devices lib)
LOCAL_C_INCLUDES 	:= $(subst $(LOCAL_PATH)/../,,$(wildcard $(SRC)/*/))
LOCAL_CPPFLAGS		:= -Dandroid -frtti -DINDEPENDENTSVG

include $(BUILD_STATIC_LIBRARY)

#### Shared library ####

include $(CLEAR_VARS)
RELPATH := ../../../../lang/java/jni
SRC := $(LOCAL_PATH)/$(RELPATH)

LOCAL_MODULE := GUIDOEngine-android

FILES			:= $(notdir $(wildcard $(SRC)/*.cpp))
LOCAL_SRC_FILES 	:= $(addprefix $(RELPATH)/, $(FILES))
FILES			:= $(notdir $(wildcard $(SRC)/svg/*.cpp))
LOCAL_SRC_FILES 	+= $(addprefix $(RELPATH)/svg/, $(FILES))
LOCAL_C_INCLUDES 	:= $(SRC)
LOCAL_C_INCLUDES 	+= $(SRC)/svg
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
LOCAL_CPPFLAGS 		:= -Dandroid


LOCAL_STATIC_LIBRARIES = GUIDOEngine-prebuilt

include $(BUILD_SHARED_LIBRARY)
