#pragma once

#include "hephQueueReserveInterface.hpp"

class	HephQueueReserveBasic: public HephQueueReserveInterface {
	public:
		HephResult	reserve(VkPhysicalDevice physicalDevice);
};
