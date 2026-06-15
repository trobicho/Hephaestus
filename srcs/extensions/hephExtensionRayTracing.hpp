#pragma once

#include "hephExtensionInterface.hpp"
#include <cstdint>
#include <cstring>
#include <memory>
#include <vulkan/vulkan_core.h>

class	HephExtensionRayTracing: public HephDeviceExtensionInterface {
	public:
		inline std::vector<const char*>						deviceExtensions() override {
			return (std::vector<const char*> {
				VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
				VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
				VK_KHR_RAY_QUERY_EXTENSION_NAME,
				VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME,
				VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
				VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
				VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
				VK_KHR_SPIRV_1_4_EXTENSION_NAME,
				VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME,
				VK_EXT_SCALAR_BLOCK_LAYOUT_EXTENSION_NAME,
			});
		}
		inline uint32_t                 deviceFeatures(void* data = nullptr, void* link = nullptr) override {
      if (data != nullptr) {
        VkPhysicalDeviceScalarBlockLayoutFeatures         scalarBlockFeatures = {
          .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES,
          .pNext = (char*)data + sizeof(scalarBlockFeatures),
          .scalarBlockLayout = VK_TRUE,
        };
        memcpy(data, &scalarBlockFeatures, sizeof(scalarBlockFeatures));
        data = (char*)data + sizeof(scalarBlockFeatures);

        VkPhysicalDeviceBufferDeviceAddressFeatures       deviceAddressFeatures = {
          .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES,
          .pNext = (char*)data + sizeof(deviceAddressFeatures),
          .bufferDeviceAddress = VK_TRUE,
        };
        memcpy(data, &deviceAddressFeatures, sizeof(deviceAddressFeatures));
        data = (char*)data + sizeof(deviceAddressFeatures);

        VkPhysicalDeviceRayTracingPipelineFeaturesKHR     rtpFeatures = {
          .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR,
          .pNext = (char*)data + sizeof(rtpFeatures),
          .rayTracingPipeline = VK_TRUE,
        };
        memcpy(data, &rtpFeatures, sizeof(rtpFeatures));
        data = (char*)data + sizeof(rtpFeatures);

        VkPhysicalDeviceAccelerationStructureFeaturesKHR  asFeatures = {
          .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR,
          .pNext = link,
          .accelerationStructure = VK_TRUE,
        };
        memcpy(data, &asFeatures, sizeof(asFeatures));

        /*
           VkPhysicalDeviceFeatures2KHR  features = {
           .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR,
           .pNext = data,
           .features = {
           .shaderInt64 = VK_TRUE,
           },
           };
           */
      }
      return (
        sizeof(VkPhysicalDeviceScalarBlockLayoutFeatures)
        + sizeof(VkPhysicalDeviceBufferDeviceAddressFeatures)
        + sizeof(VkPhysicalDeviceRayTracingPipelineFeaturesKHR)
        + sizeof(VkPhysicalDeviceAccelerationStructureFeaturesKHR)
      );
		}
		void	deviceFunctionLoader(VkDevice device) override;
};
