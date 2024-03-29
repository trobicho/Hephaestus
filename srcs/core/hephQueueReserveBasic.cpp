#include "hephQueueReserveBasic.hpp"
#include "hephHelper.hpp"
#include <algorithm>
#include <bitset>
#include <cstdint>

void	HephQueueReserveBasic::rateQueueFamily() {
	for (auto& reserveInfo: m_queueReserveInfos) {
		std::vector<QueueFamilyRating>	familyRatings;
		for (int i = 0; i < m_queueFamilyProps.size(); i++) {
			auto& props = m_queueFamilyProps[i]; 
			VkQueueFlags propFlags = props.props.queueFamilyProperties.queueFlags;
			if (reserveInfo.flags & propFlags) {
				QueueFamilyRating	rating;
				rating.family = i;
				rating.rating = 1.0;
				propFlags -= reserveInfo.flags;
				if (propFlags != 0) {
					if (propFlags & VK_QUEUE_GRAPHICS_BIT) {
						rating.rating -= 0.5;
					}
					std::bitset<32> flagsBitSet(propFlags);
					rating.rating /= flagsBitSet.count() + 1;
				}
				familyRatings.push_back(rating);
			}
		}
		std::sort(familyRatings.begin(), familyRatings.end());
		m_queueFamilyRatings.push_back(familyRatings);
	}
}

HephResult	HephQueueReserveBasic::reserve(VkPhysicalDevice physicalDevice) {
	if (m_queueReserveInfos.empty())
		return (HephResult("in HephQueueReserveBasic QueueReserveInfo shouldn't be empty"));
	m_queueFamilyProps.clear();
	HEPH_CHECK_RESULT(getFamilyProps(physicalDevice));
	for (uint32_t i = 0; i < m_queueFamilyProps.size(); i++) {
		HephHelper::printQueueFamilyInfo(m_queueFamilyProps[i]);
	}
	rateQueueFamily();

	for (int r = 0; r < m_queueReserveInfos.size(); r++) {
		auto& reserve = m_queueReserveInfos[r];
		auto& ratings = m_queueFamilyRatings[r];
		uint32_t	queueTotal = 0;

		for (auto& rating : ratings) {
			if (reserve.count == 0 && queueTotal > 0)
				break;
			uint32_t	queueCount = std::min(m_queueFamilyProps[rating.family].props.queueFamilyProperties.queueCount
					- m_queueFamilyCurrentIndex[rating.family], reserve.count - queueTotal);
			if (queueCount == 0) {
				if (queueTotal == 0)
					queueCount = m_queueFamilyProps[rating.family].props.queueFamilyProperties.queueCount;
				else
					break;
			}
			std::vector<float>	priorities;
				priorities.resize(queueCount, reserve.priority);
				m_queueCreateInfos.push_back((VkDeviceQueueCreateInfo){
					.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
					.queueFamilyIndex = rating.family,
					.queueCount = queueCount,
					.pQueuePriorities = priorities.data(),
				});
				queueTotal += queueCount;
				for (int i = 0; i < queueCount; i++) {
					HephQueueRetrieveInfo retrieveInfo;
					retrieveInfo.familyIndex = rating.family;
					retrieveInfo.queueIndex = i + m_queueFamilyCurrentIndex[rating.family];
					reserve.retrieveInfo.push_back(retrieveInfo);
				}
				m_queueFamilyCurrentIndex[rating.family] += queueCount;
		}
		reserve.count -= queueTotal;
	}
	return (HephResult());
}
