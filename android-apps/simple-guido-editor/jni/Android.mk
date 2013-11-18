LOCAL_PATH := $(call my-dir)

# first, we generate guido2.h from the svg file
$(shell (xxd -i ../../src/guido2.svg > $(LOCAL_PATH)/guido2.h))

# prebuilt GUIDO from cmake
include $(CLEAR_VARS)
LOCAL_MODULE := GUIDOEngine-prebuilt
LOCAL_SRC_FILES := libs/armeabi-v7a/libSGUIDOEngine.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/libs/armeabi-v7a/Headers
include $(PREBUILT_STATIC_LIBRARY)

# prebuilt pixman
include $(CLEAR_VARS)
LOCAL_MODULE := pixman-prebuilt
LOCAL_SRC_FILES := ../../../../android-cairo/obj/local/armeabi-v7a/libpixman.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../../../android-cairo/jni/pixman/pixman
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../../../../android-cairo/jni/pixman-extra
include $(PREBUILT_STATIC_LIBRARY)

# prebuilt cairo
include $(CLEAR_VARS)
LOCAL_MODULE := cairo-prebuilt
LOCAL_SRC_FILES := ../../../../android-cairo/obj/local/armeabi-v7a/libcairo.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../../../android-cairo/jni/cairo/src
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../../../../android-cairo/jni/cairo-extra
include $(PREBUILT_STATIC_LIBRARY)

# local library
include $(CLEAR_VARS)
LOCAL_MODULE := GUIDOEngine-android
LOCAL_SRC_FILES := GUIDOEngine-android.cpp
LOCAL_STATIC_LIBRARIES = GUIDOEngine-prebuilt cairo-prebuilt pixman-prebuilt
include $(BUILD_SHARED_LIBRARY)