cmake_minimum_required (VERSION 3.9)

project ("hephaestus" VERSION 0.1 DESCRIPTION "a little vulkan wrapper")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES CACHE BOOL "Export all symbols")
set(CMAKE_CXX_FLAGS_DEBUG  "-g")

add_library (hephaestus
	srcs/core/hephaestus_core.hpp
	srcs/core/hephInstance.cpp
	srcs/core/hephInstance.hpp
	srcs/core/hephResult.cpp
	srcs/core/hephResult.hpp
	srcs/core/hephHelper.hpp
	srcs/core/hephQueueReserveInterface.hpp
	srcs/core/hephQueueReserveBasic.cpp
	srcs/core/hephQueueReserveBasic.hpp

	srcs/physicalDevices/hephPhysicalDevicesSelector.hpp

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
	PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}include/${PROJECT_NAME}"
)
install(FILES "srcs/core/hephaestus_core.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}include/${PROJECT_NAME}/core/")
install(FILES "srcs/core/hephInstance.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}include/${PROJECT_NAME}/core/")
install(FILES "srcs/core/hephResult.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}include/${PROJECT_NAME}/core/")
install(FILES "srcs/core/hephHelper.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}include/${PROJECT_NAME}/core/")
install(FILES "srcs/core/hephQueueReserveInterface.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}include/${PROJECT_NAME}/core/")
install(FILES "srcs/core/hephQueueReserveBasic.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}include/${PROJECT_NAME}/core/")

install(FILES "srcs/physicalDevices/hephPhysicalDevicesSelector.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}include/${PROJECT_NAME}/physicalDevices/")

install(FILES "srcs/extensions/hephExtensionInterface.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}include/${PROJECT_NAME}/extensions/")
install(FILES "srcs/extensions/hephExtensionDebug.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}include/${PROJECT_NAME}/extensions/")
install(FILES "srcs/extensions/hephExtensionRayTracing.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}include/${PROJECT_NAME}/extensions/")
