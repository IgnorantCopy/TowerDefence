#/****************************************************************************
# Copyright (c) 2013-2014 cocos2d-x.org
# Copyright (c) 2015-2017 Chukong Technologies Inc.
#
# http://www.cocos2d-x.org
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/

cmake_minimum_required(VERSION 3.6)

set(APP_NAME TowerDefence)

project(${APP_NAME})

if (XCODE)
    if (NOT DEFINED CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET)
        SET(CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET 8.0)
    endif ()
endif ()

if (TOWERDEFENCE_SANITIZER)
    if (WIN32)
        message("disable annotation on windows")
        add_compile_definitions("_DISABLE_VECTOR_ANNOTATION" "_DISABLE_STRING_ANNOTATION")
    endif ()
endif ()

if (NOT DEFINED BUILD_ENGINE_DONE) # to test TowerDefence into root project
    set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cocos2d)
    set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

    include(CocosBuildSet)
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)
endif ()

if (TOWERDEFENCE_SANITIZER)
    if (WINDOWS)
        message("enabling sanitizer")
        add_compile_options("/fsanitize=address")
        add_link_options("/fsanitize=address")
        add_compile_definitions("TOWERDEFENCE_SANITIZER")
    else ()
        message("enabling sanitizer")
        add_compile_options(-fsanitize=undefined,address)
        add_link_options(-fsanitize=undefined,address)
    endif ()
endif ()

add_library(core
        Classes/core/map.cpp
        Classes/core/map.h
        Classes/core/entity/entity.h
        Classes/core/entity/entity.cpp
        Classes/core/timer.h
        Classes/core/id.h
        Classes/core/entity/route.cpp
        Classes/core/entity/enemy/Attack-down.cpp
        Classes/core/entity/enemy/Attack-down.h
        Classes/core/entity/enemy/Boss-1.cpp
        Classes/core/entity/enemy/Boss-1.h
        Classes/core/entity/enemy/Boss-2.cpp
        Classes/core/entity/enemy/Boss-2.h
        Classes/core/entity/enemy/Crab.h
        Classes/core/entity/enemy/Destroyer.h
        Classes/core/entity/enemy/Life-up.cpp
        Classes/core/entity/enemy/Life-up.h
        Classes/core/entity/enemy/Not-attacked.cpp
        Classes/core/entity/enemy/Not-attacked.h
        Classes/core/entity/enemy/Soldier.h
        Classes/core/entity/enemy/Speed-up.cpp
        Classes/core/entity/enemy/Speed-up.h
        Classes/core/entity/enemy/Tank.h
        Classes/core/entity/enemy/Warlock.h
        Classes/core/entity/enemy/dog.h
        Classes/core/entity/enemy/worm.cpp
        Classes/core/entity/enemy/worm.h
        Classes/core/entity/tower/aggressive_magician.cpp
        Classes/core/entity/tower/aggressive_magician.h
        Classes/core/entity/tower/aggressive_magician_plus.cpp
        Classes/core/entity/tower/aggressive_magician_plus.h
        Classes/core/entity/tower/archer.cpp
        Classes/core/entity/tower/archer.h
        Classes/core/entity/tower/archer_base.cpp
        Classes/core/entity/tower/archer_base.h
        Classes/core/entity/tower/archer_plus.cpp
        Classes/core/entity/tower/archer_plus.h
        Classes/core/entity/tower/bomber.cpp
        Classes/core/entity/tower/bomber.h
        Classes/core/entity/tower/bomber_plus.cpp
        Classes/core/entity/tower/bomber_plus.h
        Classes/core/entity/tower/core_magician.cpp
        Classes/core/entity/tower/core_magician.h
        Classes/core/entity/tower/core_magician_plus.cpp
        Classes/core/entity/tower/core_magician_plus.h
        Classes/core/entity/tower/decelerate_magician.cpp
        Classes/core/entity/tower/decelerate_magician.h
        Classes/core/entity/tower/decelerate_magician_plus.cpp
        Classes/core/entity/tower/decelerate_magician_plus.h
        Classes/core/entity/tower/diffusive_magician.cpp
        Classes/core/entity/tower/diffusive_magician.h
        Classes/core/entity/tower/diffusive_magician_plus.cpp
        Classes/core/entity/tower/diffusive_magician_plus.h
        Classes/core/entity/tower/helper_base.cpp
        Classes/core/entity/tower/helper_base.h
        Classes/core/entity/tower/highspeed_archer.cpp
        Classes/core/entity/tower/highspeed_archer.h
        Classes/core/entity/tower/highspeed_archer_plus.cpp
        Classes/core/entity/tower/highspeed_archer_plus.h
        Classes/core/entity/tower/magician_base.cpp
        Classes/core/entity/tower/magician_base.h
        Classes/core/entity/tower/special_magician.cpp
        Classes/core/entity/tower/special_magician.h
        Classes/core/entity/tower/special_magician_plus.cpp
        Classes/core/entity/tower/special_magician_plus.h
        Classes/core/entity/tower/weaken_magician.cpp
        Classes/core/entity/tower/weaken_magician.h
        Classes/core/entity/tower/weaken_magician_plus.cpp
        Classes/core/entity/tower/weaken_magician_plus.h)

set(TOWERDEFENCE_CXX_STANDARD 23)

set_property(TARGET core PROPERTY CXX_STANDARD ${TOWERDEFENCE_CXX_STANDARD})

set(GAME_RES_FOLDER
        "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
)
if (APPLE OR WINDOWS)
    cocos_mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
endif ()

# add cross-platforms source files and header files 
list(APPEND GAME_SOURCE
        Classes/AppDelegate.cpp
        Classes/HelloWorldScene.cpp
        Classes/SelectLevelScene.cpp
        Classes/LevelScene.cpp
        Classes/Level1Scene.cpp
        Classes/Level2Scene.cpp
        Classes/Level3Scene.cpp
        Classes/animation/TowerAnimation.cpp
        Classes/animation/EnemyAnimation.cpp
        Classes/manual/manual.cpp
)
list(APPEND GAME_HEADER
        Classes/AppDelegate.h
        Classes/HelloWorldScene.h
        Classes/SelectLevelScene.h
        Classes/LevelScene.h
        Classes/Level1Scene.h
        Classes/Level2Scene.h
        Classes/Level3Scene.h
        Classes/animation/TowerAnimation.h
        Classes/animation/EnemyAnimation.h
        Classes/manual/manual.h
)

if (ANDROID)
    # change APP_NAME to the share library name for Android, it's value depend on AndroidManifest.xml
    set(APP_NAME MyGame)
    list(APPEND GAME_SOURCE
            proj.android/app/jni/hellocpp/main.cpp
    )
elseif (LINUX)
    list(APPEND GAME_SOURCE
            proj.linux/main.cpp
    )
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")
elseif (WINDOWS)
    list(APPEND GAME_HEADER
            proj.win32/main.h
            proj.win32/resource.h
    )
    list(APPEND GAME_SOURCE
            proj.win32/main.cpp
            proj.win32/game.rc
            ${common_res_files}
    )
elseif (APPLE)
    if (IOS)
        list(APPEND GAME_HEADER
                proj.ios_mac/ios/AppController.h
                proj.ios_mac/ios/RootViewController.h
        )
        set(APP_UI_RES
                proj.ios_mac/ios/LaunchScreen.storyboard
                proj.ios_mac/ios/LaunchScreenBackground.png
                proj.ios_mac/ios/Images.xcassets
        )
        list(APPEND GAME_SOURCE
                proj.ios_mac/ios/main.m
                proj.ios_mac/ios/AppController.mm
                proj.ios_mac/ios/RootViewController.mm
                proj.ios_mac/ios/Prefix.pch
                ${APP_UI_RES}
        )
    elseif (MACOSX)
        set(APP_UI_RES
                proj.ios_mac/mac/Icon.icns
                proj.ios_mac/mac/Info.plist
        )
        list(APPEND GAME_SOURCE
                proj.ios_mac/mac/main.cpp
                proj.ios_mac/mac/Prefix.pch
                ${APP_UI_RES}
        )
    endif ()
    list(APPEND GAME_SOURCE ${common_res_files})
endif ()

# mark app complie info and libs info
set(all_code_files
        ${GAME_HEADER}
        ${GAME_SOURCE}
        Classes/plot/plot_1.cpp
        Classes/plot/plot_1.h
        Classes/plot/plot_2.cpp
        Classes/plot/plot_2.h
        Classes/plot/plot_3.cpp
        Classes/plot/plot_3.h
        Classes/plot/plot_4.cpp
        Classes/plot/plot_4.h
        Classes/plot/plot.cpp
        Classes/plot/plot.h

)
if (NOT ANDROID)
    add_executable(${APP_NAME} ${all_code_files})
else ()
    add_library(${APP_NAME} SHARED ${all_code_files})
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos/platform/android ${ENGINE_BINARY_PATH}/cocos/platform)
    target_link_libraries(${APP_NAME} -Wl,--whole-archive cpp_android_spec -Wl,--no-whole-archive)
endif ()

if (TOWERDEFENCE_SANITIZER)
    add_compile_options(-fsanitize=address,undefined)
    add_link_options(-fsanitize=address,undefined)
endif ()

set_property(TARGET ${APP_NAME} PROPERTY CXX_STANDARD ${TOWERDEFENCE_CXX_STANDARD})
target_link_libraries(${APP_NAME} cocos2d core)
target_include_directories(${APP_NAME}
        PRIVATE Classes
        PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include/
)

# mark app resources
setup_cocos_app_config(${APP_NAME})
if (APPLE)
    set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")
    set_xcode_property(${APP_NAME} INSTALL_PATH "\$(LOCAL_APPS_DIR)")

    if (MACOSX)
        set_xcode_property(${APP_NAME} INFOPLIST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/Info.plist")
    elseif (IOS)
        set_xcode_property(${APP_NAME} INFOPLIST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/ios/Info.plist")
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
    endif ()

    # For code-signing, set the DEVELOPMENT_TEAM:
    #set_xcode_property(${APP_NAME} DEVELOPMENT_TEAM "GRLXXXX2K9")
elseif (WINDOWS)
    cocos_copy_target_dll(${APP_NAME})
endif ()

if (LINUX OR WINDOWS)
    cocos_get_resource_path(APP_RES_DIR ${APP_NAME})
    cocos_copy_target_res(${APP_NAME} LINK_TO ${APP_RES_DIR} FOLDERS ${GAME_RES_FOLDER})
endif ()
