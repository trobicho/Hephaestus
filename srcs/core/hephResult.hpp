#pragma once

#include <iostream>
#include <cstdint>
#include <string>
#include <vulkan/vulkan_core.h>

#define	HEPH_CHECK_RESULT(Result)	{HephResult r = Result; if (!r.valid()){return (r);}}
#define	HEPH_PRINT_RESULT(Result)	{HephResult r = Result; if (!r.valid()){std::cout << r.error << std::endl;}}

class	HephResult {
	public:
		HephResult(bool ok = true): m_isOk(ok) {};
		HephResult(VkResult result) {m_isOk = true; if (result != VK_SUCCESS) {m_isOk = false; error = vkResultToString(result);}}
		HephResult(std::string err, bool ok = false): m_isOk(ok), error(err) {};
		HephResult(const char* err, bool ok = false): m_isOk(ok), error(std::string(err)) {};

		bool	valid(){return (m_isOk);}

		std::string		error = "";

		static std::string	vkResultToString(VkResult result);

  private:
		bool					m_isOk = true;
};
