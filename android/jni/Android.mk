# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# the purpose of this sample is to demonstrate how one can
# generate two distinct shared libraries and have them both
# uploaded in
#

#LOCAL_PATH:= $(call my-dir)
#
## first lib, which will be built statically
##
#include $(CLEAR_VARS)
#
#LOCAL_MODULE    := libtwolib-first
#LOCAL_SRC_FILES := first.c
#
#include $(BUILD_STATIC_LIBRARY)
#
## second lib, which will depend on and include the first one
##
#include $(CLEAR_VARS)
#
#LOCAL_MODULE    := libtwolib-second
#LOCAL_SRC_FILES := second.c
#
#LOCAL_STATIC_LIBRARIES := libtwolib-first
#
#include $(BUILD_SHARED_LIBRARY)
#

LOCAL_PATH := $(call my-dir)

# prebuilt GUIDO from cmake
include $(CLEAR_VARS)
LOCAL_MODULE := GUIDOEngine-prebuilt
LOCAL_SRC_FILES := libs/armeabi-v7a/libSGUIDOEngine.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/libs/armeabi-v7a/Headers
include $(PREBUILT_STATIC_LIBRARY)

# prebuilt pixman
include $(CLEAR_VARS)
LOCAL_MODULE := pixman-prebuilt
LOCAL_SRC_FILES := ../../../android-cairo/obj/local/armeabi-v7a/libpixman.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../../android-cairo/jni/pixman/pixman
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../../../android-cairo/jni/pixman-extra
include $(PREBUILT_STATIC_LIBRARY)

# prebuilt cairo
include $(CLEAR_VARS)
LOCAL_MODULE := cairo-prebuilt
LOCAL_SRC_FILES := ../../../android-cairo/obj/local/armeabi-v7a/libcairo.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../../android-cairo/jni/cairo/src
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../../../android-cairo/jni/cairo-extra
include $(PREBUILT_STATIC_LIBRARY)

# local library
include $(CLEAR_VARS)
LOCAL_MODULE := GUIDOEngine-android
LOCAL_SRC_FILES := GUIDOEngine-android.cpp
LOCAL_STATIC_LIBRARIES = GUIDOEngine-prebuilt cairo-prebuilt pixman-prebuilt
include $(BUILD_SHARED_LIBRARY)