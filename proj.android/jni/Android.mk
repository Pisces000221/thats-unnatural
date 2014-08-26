LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := unnatural/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/Global.cpp \
                   ../../Classes/Bricks.cpp \
                   ../../Classes/algo/indirect_touch.cpp \
                   ../../Classes/data/level_reader.cpp \
                   ../../Classes/widgets/GravityPicker.cpp \
                   ../../Classes/widgets/TickButton.cpp \
                   ../../Classes/widgets/Slider.cpp \
                   ../../Classes/widgets/Dashboard.cpp \
                   ../../Classes/widgets/Timepiece.cpp \
                   ../../Classes/SplashScene.cpp \
                   ../../Classes/StartupScene.cpp \
                   ../../Classes/LevelListScene.cpp \
                   ../../Classes/FreePhysicsScene.cpp \
                   ../../Classes/LevelPlayScene.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
                    $(LOCAL_PATH)/../../cocos2d/external

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static

# LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static
# LOCAL_WHOLE_STATIC_LIBRARIES += spine_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)

# $(call import-module,Box2D)
# $(call import-module,editor-support/cocosbuilder)
# $(call import-module,editor-support/spine)
# $(call import-module,editor-support/cocostudio)
# $(call import-module,network)
# $(call import-module,extensions)
