#include "hephQueueReserveBasic.hpp"

static void	printQueueFamilyInfo(HephQueueFamilyProperties) {

}

HephResult	HephQueueReserveBasic::reserve(VkPhysicalDevice physicalDevice) {
	if (m_queueReserveInfos.empty())
		return (HephResult("in HephQueueReserveBasic QueueReserveInfo shouldn't be empty"));
	m_queueFamilyProps.clear();
	HEPH_CHECK_RESULT(getFamilyProps(physicalDevice));

	for (auto& reserveInfo: m_queueReserveInfos) {
		for (uint32_t i = 0; i < m_queueFamilyProps.size(); i++) {
			printQueueFamilyInfo(m_queueFamilyProps[i]);
			if (m_queueFamilyProps[i].props.queueFamilyProperties.queueFlags == reserveInfo.flags
					&& m_queueFamilyProps[i].props.queueFamilyProperties.queueCount > m_queueFamilyCurrentIndex[i]) {
				std::cout << "TEST" << i << std::endl;
				uint32_t	queueCount = std::min(m_queueFamilyProps[i].props.queueFamilyProperties.queueCount - m_queueFamilyCurrentIndex[i], reserveInfo.count);
				if (queueCount == 0)
					queueCount = m_queueFamilyProps[i].props.queueFamilyProperties.queueCount;
				std::vector<float>	priorities;
				priorities.resize(queueCount, reserveInfo.priority);
				m_queueCreateInfos.push_back((VkDeviceQueueCreateInfo){
					.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
					.queueFamilyIndex = i,
					.queueCount = queueCount,
					.pQueuePriorities = priorities.data(),
				});
				for (int j = 0; j < queueCount; j++) {
					m_queueReserveInfos[i].retrieveInfo[j].familyIndex = i;
					m_queueReserveInfos[i].retrieveInfo[j].queueIndex = j + m_queueFamilyCurrentIndex[i];
				}
				m_queueFamilyCurrentIndex[i] += queueCount;
			}
		}
	}
	return (HephResult());
}
