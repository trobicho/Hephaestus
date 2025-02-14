#include "hephCompiler.hpp"
#include <glslang/Public/ShaderLang.h>
#include <iostream>
#include <stdexcept>
#include <cstdint>

HephCompiler::HephCompiler() {
  if (!glslang::InitializeProcess())
    throw std::runtime_error("Error with InitializeProcess");
}

HephCompiler::~HephCompiler() {
  glslang::FinalizeProcess();
}
  
HephResult HephCompiler::setShader(const std::string &filename) {
  m_shaderIfs = std::ifstream(filename, std::ifstream::in);
  if (!m_shaderIfs.is_open())
    return (HephResult("Cannot open file" + filename));

  std::string filenameWithoutPath = filename;
  auto posBegin = filename.find_last_of('/');
  if (posBegin != std::string::npos)
    filenameWithoutPath = filename.substr(posBegin + 1);

  auto pos = filenameWithoutPath.find_last_of('.');
  if (pos == std::string::npos)
    return (HephResult("cannot deduce shader stage from filename: " + filenameWithoutPath));
  std::string stage = filenameWithoutPath.substr(pos + 1);

	int stageV = hephGetShaderStageFromFileExtension(stage);
	if (stageV != -1)
		m_stage = static_cast<EShLanguage>(stageV);
	else
		return (HephResult("invalid stage " + stage));
	return (HephResult());
}

HephResult	HephCompiler::shaderValidate() {
  HEPH_CHECK_RESULT(HephResult(m_shaderIfs.is_open()).errorFormat("shader as not been set {{}}"));

  glslang::TShader  shader = glslang::TShader(m_stage);

  shader.setEnvInput(glslang::EShSourceGlsl, m_stage, glslang::EShClientVulkan, 450);
  shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_3);
  shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_4);

  std::vector<std::string> lines;
  for (std::string line; std::getline(m_shaderIfs, line); ) {
    lines.push_back(line);
  }
  std::vector<const char *> lines_cstr;
  lines_cstr.reserve(lines.size());
  for (auto& line : lines) {
    line.append("\n");
    lines_cstr.push_back(line.c_str());
    std::cout << line;
  }
  shader.setStrings(lines_cstr.data(), lines.size());

  EShMessages       messages = EShMsgAST;
  TBuiltInResource  builtInResources;
  shader.setEntryPoint("main");
  shader.setEnhancedMsgs();

  bool  parseRet = shader.parse(&builtInResources, 0, true, messages);
  if (!parseRet) {
    std::cout << "parsing failed" << std::endl;
		std::cout << std::endl;
		std::cout << "INFO LOG:" << std::endl;
		std::cout << shader.getInfoLog() << std::endl;
		std::cout << std::endl;

		std::cout << "INFO DEBUG LOG:" << std::endl;
		std::cout << shader.getInfoDebugLog() << std::endl;
		std::cout << std::endl;
		return HephResult("Failed to parse shader!");
  }
  m_intermediate = shader.getIntermediate();
  
  if (!parseRet) {
    return (parseRet);
  }

  {
    glslang::TProgram program;
    program.addShader(&shader);
    EShMessages       compileMessages = EShMsgDebugInfo;
    bool compileRet = program.link(compileMessages);
    compileRet &= program.buildReflection(EShReflectionDefault);

    if (!compileRet) {
      std::cout << "compilation failed" << std::endl;
			std::cout << std::endl;
			std::cout << "INFO LOG:" << std::endl;
			std::cout << program.getInfoLog() << std::endl;
			std::cout << std::endl;

			std::cout << "INFO DEBUG LOG:" << std::endl;
			std::cout << program.getInfoDebugLog() << std::endl;
			std::cout << std::endl;

			std::cout << "REFLECTION:" << std::endl;
			program.dumpReflection();
			return HephResult("Failed to compile shader!");
		}

		std::vector<uint32_t> outSPV;

		glslang::SpvOptions spvOptions;

		spvOptions.generateDebugInfo = true;
		spvOptions.disableOptimizer = false;
		spvOptions.optimizeSize = false;
		glslang::GlslangToSpv(*program.getIntermediate(m_stage), outSPV, &spvOptions);
		std::cout << "SPV data size:" << static_cast<uint64_t>(outSPV.size()) << std::endl;
		return (HephResult());
	}
}
