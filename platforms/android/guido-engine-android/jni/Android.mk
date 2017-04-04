#### Shared library for Guido engine ####

LOCAL_PATH := $(call my-dir)
SRC := $(LOCAL_PATH)/../../../../src/engine

include $(CLEAR_VARS)

LOCAL_MODULE := GUIDOEngine

# SRC var is a trick to use wildcard in android.mk
# if no wildcard, you have better to use relative path, conforming to android doc 
LOCAL_SRC_FILES         := $(subst $(LOCAL_PATH)/,,$(wildcard $(SRC)/*/*.cpp))
LOCAL_EXPORT_C_INCLUDES := $(addprefix $(SRC)/, include devices lib)
LOCAL_C_INCLUDES        := $(subst $(LOCAL_PATH)/../,,$(wildcard $(SRC)/*/))
LOCAL_CPPFLAGS          := -Dandroid -frtti -DINDEPENDENTSVG
LOCAL_LDLIBS += -latomic

include $(BUILD_SHARED_LIBRARY)

