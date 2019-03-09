LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libvrpmb
LOCAL_MODULE_TAGS := optional

LOCAL_MODULE_CLASS := STATIC_LIBRARIES
LOCAL_MODULE_SUFFIX := .a

LOCAL_MULTILIB := both

LOCAL_SRC_FILES_arm := ../../../client/libs/armeabi-v7a/libvrpmb.a
LOCAL_SRC_FILES_arm64 := ../../../client/libs/arm64-v8a/libvrpmb.a

LOCAL_EXPORT_C_INCLUDE_DIRS := \
	$(LOCAL_PATH)/include

include $(BUILD_PREBUILT)
