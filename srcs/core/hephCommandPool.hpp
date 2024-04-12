#pragma once

#include "hephResult.hpp"

class		HephCommandPool {
	public:
		HephCommandPool() {};
		~HephCommandPool() {destroy();}

		HephResult	create(HephDevice	device);
		HephResult	destroy();
};
