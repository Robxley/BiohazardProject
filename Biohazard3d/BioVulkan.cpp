#include "BioVulkan.hpp"
#include "VulkanTools.hpp"

using namespace bhd;

VkResult BioVulkan::init(const std::vector<std::string> & extensions, const std::vector<std::string> & layers)
{
	BHD_LOG("Vulkan initialisaion:");
	BHD_LOG_PUSH;
	VkResult result;

	instance.extensions = extensions;
	instance.layers = layers;

	auto resultTest = [&](const char * msg) {
		if (result != VK_SUCCESS) {
			BHD_LOG(msg << ": FAIL"); }
		else {
			BHD_LOG(msg << ": OK"); }
	};

	result = instance.init();
	resultTest("Vulkan Create Instance");

#if _DEBUG
	result = debugVulkanLayer.initDebugReportCallback(instance);
	resultTest("Debug Vulkan Layer");
#endif

	BHD_LOG_POP;
	return VK_SUCCESS;
}

void BioVulkan::release()
{
#ifdef _DEBUG
	debugVulkanLayer.release();
#endif

	instance.release();
}


#ifdef _glfw3_h_
 std::vector<std::string> BioVulkan::getGlfwRequiredInstanceExtensions()
{
	uint32_t n = 0;
	std::vector<std::string> exts;
	if (glfwVulkanSupported() != GLFW_TRUE)
	{
		BHD_THROW_WITH_LOG("Vuklan is not available with GLFW!");
	}
	else
	{
		BHD_LOG("Vuklan is available with GLFW");
	}
	BHD_LOG_PUSH;
	const char ** glwfexts = glfwGetRequiredInstanceExtensions(&n);
	exts.reserve(n + 1);
	BHD_LOG("GLFW required instance extensions : ");
	BHD_LOG_PUSH;

	for (uint32_t i = 0; i < n; i++)
	{
		exts.push_back(glwfexts[i]);
		BHD_LOG(glwfexts[i]);
	}

	BHD_LOG("Debug extension : ");
	BHD_LOG(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	exts.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	BHD_LOG(std::endl);
	BHD_LOG_POP;
	BHD_LOG_POP;
	return std::move(exts);
}
#endif
