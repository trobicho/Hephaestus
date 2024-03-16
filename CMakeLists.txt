cmake_minimum_required (VERSION 3.9)

project ("hephaestus" VERSION 0.1 DESCRIPTION "a little vulkan wrapper")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES CACHE BOOL "Export all symbols")

add_library (hephaestus
						srcs/core/hephInstance.cpp)
