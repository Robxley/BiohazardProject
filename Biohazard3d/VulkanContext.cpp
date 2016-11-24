#include "VulkanContext.h"

using namespace bhd;

VkResult VulkanInstanceFactory::fillContext(VulkanContext & vkContext)
{
	VkResult result = VulkanInstance::init();
	vkContext.instance = Move(instance);		//Move the instance in the context
	return result;
}

VkResult VulkanSurfaceFactory::fillContext(VulkanContext & vkContext)
{
	VkResult result = VulkanSurface::init(vkContext.instance);
	//Move the surface stuffs in the context
	vkContext.surface = Move(surface);		
	vkContext.extent2D = extent2D;
	vkContext.surfaceFormat = surfaceFormat;
	vkContext.presentMode = presentMode;
	return result;
}


VkResult VulkanDeviceFactory::fillContext(VulkanContext & vkContext)
{
	BHD_ASSERT(vkContext.instance != VK_NULL_HANDLE);
	BHD_ASSERT(vkContext.surface != VK_NULL_HANDLE);

	auto instance = vkContext.instance;
	auto surface = vkContext.surface;
	auto extent2D = vkContext.extent2D;
	auto surfaceFormat = vkContext.surfaceFormat;
	auto presentMode = vkContext.presentMode;


	//Get the physical device features
	const std::vector<std::string> & names = VulkanDevice::getPhysicalDeviceStuffs(instance, surface);
	BHD_LOG_LIST("Available Physical Devices :", names);

	//Pick the "best physical" device
	auto pickedPhysicalDevice = VulkanDevice::getBestPhysicalDevice(instance, surface);
	BHD_LOG("Picked Physical Device : " << *pickedPhysicalDevice.name);
	pickedPhysicalDevice.extensionNames->info("Supported Extensions:");

	//Create a logical device from the best physical device
	VkResult result = VulkanDevice::createLogicalDevice(pickedPhysicalDevice, surface, VK_QUEUE_GRAPHICS_BIT);
	vkContext.device = Move(device);	//Move the device in the context 
	vkContext.swapchainCreateInfo = pickedPhysicalDevice.swapChainFeatures->pickSwapChain(surface,extent2D,surfaceFormat,presentMode);
	return result;
}

VkResult VulkanSwapChainFactory::fillContext(VulkanContext & vkContext)
{
	BHD_ASSERT(vkContext.instance != VK_NULL_HANDLE);
	BHD_ASSERT(vkContext.surface != VK_NULL_HANDLE);

	VkResult result = VulkanSwapChain::create(vkContext.device, vkContext.swapchainCreateInfo);
	vkContext.swapChain = Move(swapChain);
	vkContext.swapChainImages = std::move(swapChainImages);
	return result;
}