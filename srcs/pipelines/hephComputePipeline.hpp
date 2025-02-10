#pragma once

#include "../core/hephaestus_core.hpp"

class	HephComputePipeline {
	public:
		HephComputePipeline(){};
		~HephComputePipeline(){};

	private:
		VkPipeline				m_pipeline;
		VkPipelineLayout	m_pipelineLayout;
};
