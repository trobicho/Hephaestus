#pragma once

#include "hephResult.hpp"
#include <vulkan/vulkan_core.h>
#include <vector>
#include <cstdint>

struct	HephQueue {
	VkQueue				queue;
	VkQueueFlags	flags;
	bool					inUse = false;
	uint32_t			familyIndex;
	uint32_t			queueIndex;
};

struct	HephQueueRetrieveInfo {
	uint32_t	familyIndex;
	uint32_t	queueIndex;
};

struct	HephQueueReserveInfo {
	VkQueueFlags												flags;
	float																priority;
	uint32_t														count;
	std::vector<HephQueueRetrieveInfo>	retrieveInfos;
};

struct	HephQueueFamilyProperties {
	HephQueueFamilyProperties() {
		props = (VkQueueFamilyProperties2) {
			.sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2,
			.pNext = &globalPriorityProps,
		};
		globalPriorityProps = (VkQueueFamilyGlobalPriorityPropertiesKHR) {
			.sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_GLOBAL_PRIORITY_PROPERTIES_KHR
		};
	}
	VkQueueFamilyProperties2									props;
	VkQueueFamilyGlobalPriorityPropertiesKHR	globalPriorityProps;
};

class	HephQueueReserveInterface {
	public:
		virtual HephResult	reserve(VkPhysicalDevice physicalDevice) = 0;
		void								addReserveInfo(HephQueueReserveInfo	reserveInfo) {
			m_queueReserveInfos.push_back(reserveInfo);
		}
		void								addReserveInfo(std::vector<HephQueueReserveInfo>	reserveInfos) {
			for (auto reserveInfo: reserveInfos) {
				m_queueReserveInfos.push_back(reserveInfo);
			}
		}
		void								addReserveInfo(HephQueueReserveInfo*	reserveInfos, uint32_t reserveCount) {
			for (uint32_t i = 0; i < reserveCount; i++) {
				m_queueReserveInfos.push_back(reserveInfos[i]);
			}
		}
		const VkDeviceQueueCreateInfo*	getQueueCreateInfos(uint32_t* queueCreateInfoCount = nullptr) {
			if (queueCreateInfoCount != nullptr)
				*queueCreateInfoCount = m_queueCreateInfos.size();
			return (m_queueCreateInfos.data());
		}
		const std::vector<HephQueueReserveInfo>&			getReserveInfo() {return (m_queueReserveInfos);}

	protected:
		HephResult	getFamilyProps(VkPhysicalDevice	physicalDevice) {
			std::vector<VkQueueFamilyProperties2>	queueFamilyProps;
			uint32_t															queueCount = 0;

			vkGetPhysicalDeviceQueueFamilyProperties2(physicalDevice, &queueCount, nullptr);
			if (queueCount == 0)
				return (HephResult("No Queue Found !"));
			queueFamilyProps.resize(queueCount);
			m_queueFamilyProps.resize(queueCount);
			m_queueFamilyCurrentIndex.resize(queueCount);
			for (int i = 0; i < queueCount; i++) {
				queueFamilyProps[i] = m_queueFamilyProps[i].props;
			}
			vkGetPhysicalDeviceQueueFamilyProperties2(physicalDevice, &queueCount, queueFamilyProps.data());
			for (int i = 0; i < queueCount; i++) {
				m_queueFamilyProps[i].props = queueFamilyProps[i];
				m_queueFamilyCurrentIndex[i] = 0;
			}
			return (HephResult());
		}

		std::vector<HephQueueReserveInfo>				m_queueReserveInfos;
		std::vector<HephQueueFamilyProperties>	m_queueFamilyProps;
		std::vector<uint32_t>										m_queueFamilyCurrentIndex;
		std::vector<VkDeviceQueueCreateInfo>		m_queueCreateInfos;

		std::vector<HephQueue*>	queues;
};
