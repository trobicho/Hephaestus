#pragma once

#include "../core/hephaestus_core.hpp"

#define	HEPH_PIPELINE_TYPE_UNKNOWN			0
#define	HEPH_PIPELINE_TYPE_COMPUTE			1
#define	HEPH_PIPELINE_TYPE_GRAPHICS			2
#define	HEPH_PIPELINE_TYPE_RAYTRACING		4

class	HephPipeline {
	virtual	int		type(){return (HEPH_PIPELINE_TYPE_UNKNOWN);}
};
