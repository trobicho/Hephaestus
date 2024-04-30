#pragma once

#include <memory>
#include <vector>
#include <list>
#include <vulkan/vulkan_core.h>
		
class	HephInstanceExtensionInterface {
	public:
		inline virtual std::vector<const char*>							instanceExtensions()
			{return (std::vector<const char*>());}
		inline virtual std::vector<const char*>							instanceValidationLayers()
			{return (std::vector<const char*>());}
		inline virtual void																	instanceFunctionLoader(VkInstance device) {};
};

class	HephDeviceExtensionInterface {
	public:
		inline virtual std::vector<const char*>							deviceExtensions() = 0;
		inline virtual std::list<std::unique_ptr<void*>>		deviceFeatures() = 0;
		inline virtual std::vector<const char*>							deviceValidationLayers()
			{return (std::vector<const char*>());}
		inline virtual void																	deviceFunctionLoader(VkDevice	device) {};
		inline virtual void																	printDeviceProperties(VkDevice	device) {};
};
