cmake_minimum_required(VERSION 2.8.12.2)

if (USE_ANDROID_TOOLCHAIN)
  set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/android/android.toolchain.cmake")
  set(ANDROID_NATIVE_API_LEVEL 19)
endif ()

if (WIN32)
  cmake_policy(SET CMP0020 NEW)
endif (WIN32)

if (POLICY CMP0028)
  cmake_policy(SET CMP0028 OLD)
endif ()

if (POLICY CMP0043)
  cmake_policy(SET CMP0043 OLD)
endif () 

if (POLICY CMP0042)
  cmake_policy(SET CMP0042 OLD)
endif ()

project(hifi)
add_definitions(-DGLM_FORCE_RADIANS)

if (WIN32)
  add_definitions(-DNOMINMAX -D_CRT_SECURE_NO_WARNINGS)
  # set path for Microsoft SDKs
  # if get build error about missing 'glu32' this path is likely wrong
  # Uncomment the line with 8.1 if running Windows 8.1
  if (MSVC10)
    set(WINDOW_SDK_PATH "C:\\Program Files\\Microsoft SDKs\\Windows\\v7.1 ")
  elseif (MSVC12)
    set(WINDOW_SDK_PATH "C:\\Program Files (x86)\\Windows Kits\\8.1\\Lib\\winv6.3\\um\\x86 ")
  endif ()
  message (WINDOW_SDK_PATH= ${WINDOW_SDK_PATH})
  set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${WINDOW_SDK_PATH})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
elseif (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
  #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-unknown-pragmas")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fno-strict-aliasing -ggdb")
endif(WIN32)

if (NOT MSVC12)
  include(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
  CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

  if (COMPILER_SUPPORTS_CXX11)
  	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  elseif(COMPILER_SUPPORTS_CXX0X)
  	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
  else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
  endif()
endif ()

if (APPLE) 
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++0x")
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --stdlib=libc++")
endif ()

if (NOT ANDROID_LIB_DIR)
  set(ANDROID_LIB_DIR $ENV{ANDROID_LIB_DIR})
endif ()

if (ANDROID)  
  if (NOT ANDROID_QT_CMAKE_PREFIX_PATH)
    set(QT_CMAKE_PREFIX_PATH ${ANDROID_LIB_DIR}/Qt/5.4/android_armv7/lib/cmake)
  else ()
    set(QT_CMAKE_PREFIX_PATH ${ANDROID_QT_CMAKE_PREFIX_PATH})
  endif ()
  
  set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
  
  if (ANDROID_LIB_DIR)
    list(APPEND CMAKE_FIND_ROOT_PATH ${ANDROID_LIB_DIR})
  endif ()
else ()
  if (NOT QT_CMAKE_PREFIX_PATH)
    set(QT_CMAKE_PREFIX_PATH $ENV{QT_CMAKE_PREFIX_PATH})
  endif ()
endif ()

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QT_CMAKE_PREFIX_PATH})

if (APPLE)
  # set our OS X deployment target
  set(CMAKE_OSX_DEPLOYMENT_TARGET 10.8)
  
  # find the 10.9 SDK path
  execute_process(COMMAND xcodebuild -sdk -version OUTPUT_VARIABLE XCODE_SDK_VERSIONS)
  string(REGEX MATCH \\/.+MacOSX10.9.sdk OSX_SDK_PATH ${XCODE_SDK_VERSIONS})
  
  # set that as the SDK to use
  set(CMAKE_OSX_SYSROOT ${OSX_SDK_PATH})
endif ()

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Instruct CMake to run rcc automatically when needed
set(CMAKE_AUTORCC ON)

set(HIFI_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libraries")

# setup for find modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/")

set(MACRO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake/macros")
set(EXTERNAL_PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake/externals")

file(GLOB HIFI_CUSTOM_MACROS "cmake/macros/*.cmake")
foreach(CUSTOM_MACRO ${HIFI_CUSTOM_MACROS})
  include(${CUSTOM_MACRO})
endforeach()

if (ANDROID)
  file(GLOB ANDROID_CUSTOM_MACROS "cmake/android/*.cmake")
  foreach(CUSTOM_MACRO ${ANDROID_CUSTOM_MACROS})
    include(${CUSTOM_MACRO})
  endforeach()
endif ()

# add subdirectories for all targets
if (NOT ANDROID)
  add_subdirectory(assignment-client)
  add_subdirectory(domain-server)
  add_subdirectory(ice-server)
  add_subdirectory(interface)
  add_subdirectory(tests)
  add_subdirectory(tools)
endif ()

if (ANDROID OR DESKTOP_GVR)
  add_subdirectory(gvr-interface)
endif ()