#include "BioVulkan.hpp"
#include "VulkanTools.hpp"
#include "VulkanDevice.hpp"
#include "VulkanContext.h"

using namespace bhd;

VkResult BioVulkan::init(const std::vector<std::string> & extensions, const std::vector<std::string> & layers)
{
	BHD_LOG_NEW_SECTION;
	BHD_LOG("Vulkan initialisaion:");
	BHD_LOG_PUSH;
	VkResult result;

	TestContext(glfwWindow, extensions, layers);



	instance.extensions = extensions;
	instance.layers = layers;

	//result log tool
	//--------------------------------------
	auto resultTest = [&](const char * msg) {
		if (result != VK_SUCCESS) {
			BHD_THROW_WITH_LOG(msg << ": FAIL"); }
		else {
			BHD_LOG(msg << ": OK"); }
	};

	//Vulkan instance
	//--------------------------------------
	result = instance.init();
	resultTest("Vulkan Create Instance");

	//Vulkan Debug report layer
	//--------------------------------------
#if _DEBUG
	result = debugVulkanLayer.initDebugReportCallback(instance);
	resultTest("Debug Vulkan Layer");
#endif

	//Vulkan surface
	//--------------------------------------
#ifdef _glfw3_h_
	result = surface.initGlfw(instance, glfwWindow);
	resultTest("GLFW Surface linked to vulkan");
#endif 


	//Vulkan devices
	//--------------------------------------

	//Get the physical device features
	const std::vector<std::string> & names = device.getPhysicalDeviceStuffs(instance, surface);
	BHD_LOG_LIST("Available Physical Devices :", names);

	//Pick the "best physical" device
	auto pickedPhysicalDevice = device.getBestPhysicalDevice(instance, surface);
	BHD_LOG("Picked Physical Device : "<<*pickedPhysicalDevice.name);
	pickedPhysicalDevice.extensionNames->info("Supported Extensions:");

	//Create a logical device from the best physical device
	result = device.createLogicalDevice(pickedPhysicalDevice, surface, VK_QUEUE_GRAPHICS_BIT);
	resultTest("Vulkan Create logical device");

	//Swap chain maker
	//--------------------------------------

	auto swapChainInfo = pickedPhysicalDevice.swapChainFeatures->pickSwapChain(surface);
	swapChain.create(device, swapChainInfo);
	resultTest("Vulkan Create Swap chain");

	return VK_SUCCESS;
}

void BioVulkan::release()
{
	/*swapChain.release();
	device.release();
	surface.release();
#ifdef _DEBUG
	debugVulkanLayer.release();
#endif
	instance.release();*/
}


//GLFW stuffs
//------------------------------------------------------------------------
#ifdef _glfw3_h_

VkResult BioVulkan::initWithGlfw(GLFWwindow * window)
{
	this->glfwWindow = window;
#ifdef _DEBUG
	return init(getGlfwRequiredInstanceExtensions(), { "VK_LAYER_LUNARG_standard_validation" });
#else
	return init(getGlfwRequiredInstanceExtensions());
#endif

}

 std::vector<std::string> BioVulkan::getGlfwRequiredInstanceExtensions()
{
	 BHD_LOG_NEW_SECTION;
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
#else

#define BHD_WDYE_GLFW {BHD_ASSERT_LOG(0, "what did you expect ? GLFW not available")}

VkResult BioVulkan::initWithGlfw()
{
	BHD_WDYE_GLFW;
}
std::vector<std::string> BioVulkan::getGlfwRequiredInstanceExtensions()
{
	BHD_WDYE_GLFW;
}
#endif
