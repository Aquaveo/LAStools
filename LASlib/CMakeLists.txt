cmake_minimum_required(VERSION 3.1.2 FATAL_ERROR)
set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_CXX_STANDARD 17)
project("LASlib")

if (MSVC AND USE_TYPEDEF_WCHAR_T)
	add_definitions(/Zc:wchar_t-)
endif()

if (!MSVC)
	add_compile_options(-O3 -Wall -Wno-strict-aliasing)
else()
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_subdirectory(LASlib/src)
add_subdirectory(LASlib/example)
