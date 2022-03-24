cmake_minimum_required(VERSION 3.1.2 FATAL_ERROR)
set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_DEBUG_POSTFIX d)
project("LAStools")

if (!MSVC)
	add_compile_options(-O3 -Wall -Wno-strict-aliasing)
else()
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

if (MSVC AND USE_TYPEDEF_WCHAR_T)
	add_definitions(/Zc:wchar_t-)
endif()

option(BUILD_SHARED_LIBS "Build LASlib as DLL" OFF)

if (BUILD_SHARED_LIBS AND UNIX AND NOT APPLE)
	set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib/LASlib")
endif()

add_subdirectory(LASlib/src)
add_subdirectory(src)
if (EXISTS src_full)
	add_subdirectory(src_full)
endif()
