#pragma once
#include <memory>
#include <vector>
#include <list>
		
class	HephInstanceExtensionInterface {
	public:
		inline virtual std::vector<const char*>							instanceExtensions() = 0;
		inline virtual std::vector<const char*>							instanceValidationLayers()
			{return (std::vector<const char*>());}
};

class	HephDeviceExtensionInterface {
	public:
		inline virtual std::vector<const char*>							deviceExtensions() = 0;
		inline virtual std::list<std::unique_ptr<void*>>		deviceFeatures() = 0;
		inline virtual std::vector<const char*>							deviceValidationLayers()
			{return (std::vector<const char*>());}
};
