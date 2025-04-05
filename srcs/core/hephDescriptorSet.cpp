#include "hephDescriptorSet.hpp"
#include "hephResult.hpp"
#include <cstdint>
#include <vulkan/vulkan_core.h>

HephResult	HephPipelineDescriptor::build(HephDevice& device) {
  std::vector<VkDescriptorPoolSize> descPoolSize;

  for (auto& descriptor: descriptors) {
    for (auto& binding : descriptor.layoutBinds) {
      descPoolSize.push_back((VkDescriptorPoolSize){
        .type = binding.descriptorType,
        .descriptorCount = 1,
      });
    }
  }
  VkDescriptorPoolCreateInfo  descPoolInfo = {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
    .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT | VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT,
    .maxSets = static_cast<uint32_t>(descriptors.size()),
    .poolSizeCount = static_cast<uint32_t>(descPoolSize.size()),
    .pPoolSizes = descPoolSize.data(),
  };
  HEPH_CHECK_RESULT(HephResult(vkCreateDescriptorPool(device.device, &descPoolInfo, device.pAllocationCallbacks, &descriptorPool)
				, "Failed to create RT descriptor pool !"));

  for (auto& descriptor: descriptors) {
    VkDescriptorSetLayoutCreateInfo descSetLayoutInfo = {
      .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
      .flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT,
      .bindingCount = static_cast<uint32_t>(descriptor.layoutBinds.size()),
      .pBindings = descriptor.layoutBinds.data(),
    };
    HEPH_CHECK_RESULT(HephResult(vkCreateDescriptorSetLayout(device.device, &descSetLayoutInfo, device.pAllocationCallbacks, &descriptor.layout)
				, "Failed to create RT descriptor set layout !"));
    VkDescriptorSetAllocateInfo descSetAllocInfo = {
      .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
      .descriptorPool = descriptorPool,
      .descriptorSetCount = 1,
      .pSetLayouts = &descriptor.layout,
    };
    HEPH_CHECK_RESULT(HephResult(vkAllocateDescriptorSets(device.device, &descSetAllocInfo, &descriptor.set)
      , "Failed to Allocate RT descriptor set !"));
  }
	return (HephResult());
}

void  			HephPipelineDescriptor::update(HephDevice& device, uint32_t id, const HephDescriptorUpdateInfo* info, uint32_t count) {
	if (id >= descriptors.size())
		return ;

	std::vector<VkWriteDescriptorSet>  writes;
  writes.reserve(count);
  for (uint32_t idx = 0; idx < count; idx++) {
    for (auto& layout : descriptors[id].layoutBinds) {
      if (layout.binding == info[idx].binding) {
        if (layout.descriptorType == info[idx].type) {
          VkWriteDescriptorSet  write = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = descriptors[id].set,
            .dstBinding = layout.binding,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = info[idx].type,
          };
          if (info[idx].type == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER
              || info[idx].type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
            write.pBufferInfo = static_cast<VkDescriptorBufferInfo*>(info[idx].pInfo);
          else if (info[idx].type == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE
              || info[idx].type == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
            write.pImageInfo = static_cast<VkDescriptorImageInfo*>(info[idx].pInfo);
          else
            write.pNext = info[idx].pInfo;
          writes.push_back(write);
        }
        break ;
      }
    }
  }
  vkUpdateDescriptorSets(device.device, writes.size(), writes.data(), 0, nullptr);
}

