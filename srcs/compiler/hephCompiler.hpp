#pragma once

#include "../core/hephaestus_core.hpp"
#include <glslang/Public/ShaderLang.h>
#include <glslang/SPIRV/GlslangToSpv.h>
#include <fstream>

class HephCompiler {
  public:
    HephCompiler();
    ~HephCompiler();

    HephResult	setShader(const std::string &filename);

    HephResult	shaderValidate();

		std::vector<uint32_t>	outSPV;

  private:
    std::ifstream     m_shaderIfs;
    EShLanguage       m_stage = EShLanguage::EShLangVertex;

    glslang::TIntermediate* m_intermediate;
};
