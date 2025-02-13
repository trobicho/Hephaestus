#include "hephCompiler.hpp"
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
    throw std::invalid_argument("Cannot open file: " + filename);

  std::string filenameWithoutPath = filename;
  auto posBegin = filename.find_last_of('/');
  if (posBegin != std::string::npos)
    filenameWithoutPath = filename.substr(posBegin + 1);

  auto pos = filenameWithoutPath.find_last_of('.');
  if (pos == std::string::npos)
    throw std::invalid_argument("cannot deduce shader stage from filename: " + filenameWithoutPath);
  std::string stage = filenameWithoutPath.substr(pos + 1);

  if (stage == "vert") //for a vertex shader
    m_stage = EShLanguage::EShLangVertex;
  else if (stage == "frag") //for a fragment shader
    m_stage = EShLanguage::EShLangFragment;
  else if (stage == "comp") //for a compute shader
    m_stage = EShLanguage::EShLangCompute;
  else if (stage == "rgen ") //for a ray generation shader
    m_stage = EShLanguage::EShLangRayGen;
  else if (stage == "rint ") //for a ray intersection shader
    m_stage = EShLanguage::EShLangIntersect;
  else if (stage == "rahit") //for a ray any hit shader
    m_stage = EShLanguage::EShLangAnyHit;
  else if (stage == "rchit") //for a ray closest hit shader
    m_stage = EShLanguage::EShLangClosestHit;
  else if (stage == "rmiss") //for a ray miss shader
    m_stage = EShLanguage::EShLangMiss;
  else if (stage == "rcall") //for a ray callable shader
    m_stage = EShLanguage::EShLangCallable;
  else if (stage == "tesc") //for a tessellation control shader
    m_stage = EShLanguage::EShLangTessControl;
  else if (stage == "tese") //for a tessellation evaluation shader
    m_stage = EShLanguage::EShLangTessEvaluation;
  else if (stage == "geom") //for a geometry shader
    m_stage = EShLanguage::EShLangGeometry;
  else if (stage == "mesh") //for a mesh shader
    m_stage = EShLanguage::EShLangMeshNV;
  else if (stage == "task") //for a task shader
    m_stage = EShLanguage::EShLangTaskNV;
  else
		return (HephResult("invalid stage"));
	return (HephResult());
}

HephResult	HephCompiler::shaderValidate() {
  if (!m_shaderIfs.is_open()) {
    throw std::invalid_argument("shader as not been set");
  }

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
