cmake_minimum_required(VERSION 3.8)

project(360vr VERSION 0.1 LANGUAGES CXX)

if (NOT WIN32)
  message(FATAL_ERROR "This project currently supports only windows")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(THIRDPARTY_LIB_DIR ${CMAKE_SOURCE_DIR}/extlibs/lib/x64)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
  set(THIRDPARTY_LIB_DIR ${CMAKE_SOURCE_DIR}/extlibs/lib/Win32)
endif()

set(THIRDPARTY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/extlibs/include)

add_subdirectory("engine")
add_subdirectory("vrtour")