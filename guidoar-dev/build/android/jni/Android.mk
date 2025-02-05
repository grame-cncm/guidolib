#### Shared library for Guido engine ####

LOCAL_PATH := $(call my-dir)
SRC := $(LOCAL_PATH)/../../../src

include $(CLEAR_VARS)

LOCAL_MODULE := guidoar

# SRC var is a trick to use wildcard in android.mk
# if no wildcard, you have better to use relative path, conforming to android doc 
LOCAL_SRC_FILES         := $(subst $(LOCAL_PATH)/,,$(wildcard $(SRC)/guido/*.cpp) $(wildcard $(SRC)/guido/abstract/*.cpp) $(wildcard $(SRC)/lib/*.cpp) $(wildcard $(SRC)/operations/*.cpp) $(wildcard $(SRC)/visitors/*.cpp) $(wildcard $(SRC)/parser/*.cpp) $(wildcard $(SRC)/interface/*.cpp) )
LOCAL_EXPORT_C_INCLUDES := $(addprefix $(SRC)/, interface)
LOCAL_C_INCLUDES        := $(subst $(LOCAL_PATH)/../,,$(wildcard $(SRC)/*/) $(SRC)/guido/abstract)
LOCAL_CPPFLAGS          := -DGUIDOAR_EXPORTS -frtti

include $(BUILD_SHARED_LIBRARY)

