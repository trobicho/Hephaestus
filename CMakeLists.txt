cmake_minimum_required (VERSION 4.0)

project ("hephaestus" VERSION 0.1 DESCRIPTION "a little vulkan wrapper")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES CACHE BOOL "Export all symbols")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_compile_options("-g")

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
	srcs/core/hephShaderModule.cpp
	srcs/core/hephShaderModule.hpp
	srcs/core/hephDescriptorSet.cpp
	srcs/core/hephDescriptorSet.hpp

	srcs/command/hephCommandPool.cpp
	srcs/command/hephCommandPool.hpp

	srcs/memory/hephMemoryAllocator.cpp
	srcs/memory/hephMemoryAllocator.hpp

	srcs/texture/hephTexture.hpp

	srcs/rendering/hephSwapchain.cpp
	srcs/rendering/hephSwapchain.hpp

	srcs/physicalDevices/hephPhysicalDevicesSelector.hpp

	srcs/pipelines/hephPipeline.hpp
	srcs/pipelines/hephComputePipeline.cpp
	srcs/pipelines/hephComputePipeline.hpp
	
	srcs/compiler/hephCompiler.cpp
	srcs/compiler/hephCompiler.hpp

	srcs/extensions/hephExtensionInterface.hpp
	srcs/extensions/hephExtensionDebug.hpp
	srcs/extensions/hephExtensionRayTracing.cpp
	srcs/extensions/hephExtensionRayTracing.hpp
	srcs/extensions/hephExtensionVideo.cpp
	srcs/extensions/hephExtensionVideo.hpp
	srcs/extensions/hephExtensionScreenRendering.cpp
	srcs/extensions/hephExtensionScreenRendering.hpp

  srcs/plugins/font/hephPluginFont.hpp
  srcs/plugins/font/hephPluginFont.cpp
  srcs/plugins/font/hephFont.hpp
  srcs/plugins/font/hephFont.cpp
)

set_target_properties(hephaestus PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	PUBLIC_HEADER srcs/hephaestus.hpp
)

include(FindVulkan)
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan) 
find_package(Freetype)
target_link_libraries(${PROJECT_NAME} PRIVATE Freetype::Freetype)

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
install(FILES "srcs/core/hephShaderModule.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}include/${PROJECT_NAME}/core/")
install(FILES "srcs/core/hephDescriptorSet.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}include/${PROJECT_NAME}/core/")

install(FILES "srcs/command/hephCommandPool.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}include/${PROJECT_NAME}/command/")
install(FILES "srcs/memory/hephMemoryAllocator.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}include/${PROJECT_NAME}/memory/")

install(FILES "srcs/texture/hephTexture.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}include/${PROJECT_NAME}/texture/")

install(FILES "srcs/physicalDevices/hephPhysicalDevicesSelector.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}include/${PROJECT_NAME}/physicalDevices/")

install(FILES "srcs/rendering/hephSwapchain.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}include/${PROJECT_NAME}/rendering/")

install(FILES "srcs/pipelines/hephPipeline.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}include/${PROJECT_NAME}/pipelines/")
install(FILES "srcs/pipelines/hephComputePipeline.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}include/${PROJECT_NAME}/pipelines/")

install(FILES "srcs/compiler/hephCompiler.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}include/${PROJECT_NAME}/compiler/")

install(FILES "srcs/extensions/hephExtensionInterface.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}include/${PROJECT_NAME}/extensions/")
install(FILES "srcs/extensions/hephExtensionDebug.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}include/${PROJECT_NAME}/extensions/")
install(FILES "srcs/extensions/hephExtensionRayTracing.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}include/${PROJECT_NAME}/extensions/")
install(FILES "srcs/extensions/hephExtensionVideo.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}include/${PROJECT_NAME}/extensions/")
install(FILES "srcs/extensions/hephExtensionScreenRendering.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}include/${PROJECT_NAME}/extensions/")

install(FILES "srcs/plugins/font/hephPluginFont.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}include/${PROJECT_NAME}/plugins/font/")
install(FILES "srcs/plugins/font/hephFont.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}include/${PROJECT_NAME}/plugins/font/")
