#pragma once

#include "hephExtensionInterface.hpp"
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
		inline std::list<std::unique_ptr<void*>>	deviceFeatures() override {
			std::list<std::unique_ptr<void*>>	featuresBuffer;
			//RAYTRACING FEATURES

			VkPhysicalDeviceScalarBlockLayoutFeatures     scalarBlockFeatures = {
				.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES,
				.pNext = VK_NULL_HANDLE,
				.scalarBlockLayout = VK_TRUE,
			};
			featuresBuffer.push_front(std::make_unique<void*>(&scalarBlockFeatures));
			VkPhysicalDeviceBufferDeviceAddressFeatures   deviceAddressFeatures = {
				.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES,
				.pNext = *featuresBuffer.front(),
				.bufferDeviceAddress = VK_TRUE,
			};
			featuresBuffer.push_front(std::make_unique<void*>(&deviceAddressFeatures));
			VkPhysicalDeviceRayTracingPipelineFeaturesKHR rtpFeatures = {
				.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR,
				.pNext = *featuresBuffer.front(),
				.rayTracingPipeline = VK_TRUE,
			};
			featuresBuffer.push_front(std::make_unique<void*>(&rtpFeatures));
			VkPhysicalDeviceAccelerationStructureFeaturesKHR  asFeatures = {
				.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR,
				.pNext = *featuresBuffer.front(),
				.accelerationStructure = VK_TRUE,
			};
			featuresBuffer.push_front(std::make_unique<void*>(&asFeatures));
			VkPhysicalDeviceFeatures2KHR  features = {
				.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR,
				.pNext = *featuresBuffer.front(),
				.features = {
					.shaderInt64 = VK_TRUE,
				},
			};
			featuresBuffer.push_front(std::make_unique<void*>(&features));

			return (featuresBuffer);
		}
		void	deviceFunctionLoader(VkDevice device) override;
};
