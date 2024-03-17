cmake_minimum_required (VERSION 3.9)

project ("hephaestus" VERSION 0.1 DESCRIPTION "a little vulkan wrapper")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES CACHE BOOL "Export all symbols")

add_library (hephaestus
	srcs/core/hephInstance.cpp
	srcs/core/hephInstance.hpp
	srcs/core/hephResult.cpp
	srcs/core/hephResult.hpp

	srcs/extensions/hephExtensionInterface.hpp
	srcs/extensions/hephExtensionDebug.hpp
	srcs/extensions/hephExtensionRayTracing.hpp
)
set_target_properties(hephaestus PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	PUBLIC_HEADER srcs/hephaestus.hpp
)

include(FindVulkan)
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan) 

install(TARGETS hephaestus 
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
