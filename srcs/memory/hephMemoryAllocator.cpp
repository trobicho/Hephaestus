#include "hephMemoryAllocator.hpp"

HephResult	HephMemoryAllocator::create(HephDevice& device) {
	m_device = device;

	return (HephResult());
}
