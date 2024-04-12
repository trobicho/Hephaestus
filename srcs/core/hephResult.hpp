#pragma once

#include <algorithm>
#include <iostream>
#include <cstdint>
#include <string>
#include <vulkan/vulkan_core.h>

#define	HEPH_CHECK_RESULT(Result)	{HephResult r = Result; if (!r.valid()){return (r);}}
#define	HEPH_PRINT_RESULT(Result)	{HephResult r = Result; if (!r.valid()){std::cout << r.error << std::endl;}}

class	HephResult {
	public:
		HephResult(bool ok = true): m_isOk(ok) {};
		HephResult(VkResult result, std::string err = "") {
			m_isOk = true; 
			if (result != VK_SUCCESS) {
				vkResult = result;
				m_isOk = false; 
				if (err.empty())
					error = vkResultToString(result);
				else {
					error = err;
					size_t formatPos = error.find("{}");
					if (formatPos != -1) {
						error.erase(formatPos, 2);
						error.insert(formatPos, vkResultToString(result));
					}
				}
			}
		}
		HephResult(std::string err, bool ok = false): m_isOk(ok) {error = (ok)? "" : err;}
		HephResult(const char* err, bool ok = false): m_isOk(ok) {error = (ok)? "" : std::string(err);}

		bool	valid(){return (m_isOk);}

		std::string		error = "";
		VkResult			vkResult = VK_SUCCESS;

		static std::string	vkResultToString(VkResult result);

  private:
		bool					m_isOk = true;
};
