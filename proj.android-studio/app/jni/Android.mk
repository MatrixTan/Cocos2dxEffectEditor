LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

MY_SRC_FILES := $(wildcard $(LOCAL_PATH)/../../../Classes/*.c*)
MY_SRC_FILES += $(wildcard $(LOCAL_PATH)/../../../Classes/**/*.c*)
MY_SRC_FILES := $(MY_SRC_FILES:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES := hellocpp/main.cpp
LOCAL_SRC_FILES += $(MY_SRC_FILES)


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes \
                    $(LOCAL_PATH)/../../../Classes/Common \
                    $(LOCAL_PATH)/../../../Classes/Audio \
                    $(LOCAL_PATH)/../../../Classes/Project \
                    $(LOCAL_PATH)/../../../Classes/Message \
                    $(LOCAL_PATH)/../../../Classes/UI


# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
