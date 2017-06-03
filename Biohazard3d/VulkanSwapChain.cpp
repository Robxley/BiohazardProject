#include "VulkanSwapChain.h"
#include "Assertion.hpp"


using namespace bhd;


VkResult VulkanSwapChain::create(const VkDevice & _device, const VkSwapchainCreateInfoKHR & _swapChainInfo) 
{

	//Init
	device = _device;
	swapChainInfo = _swapChainInfo;
	
	//Create swap chain
	VkResult result = vkCreateSwapchainKHR(device, &swapChainInfo, nullptr, &swapChain);
	if (result != VK_SUCCESS) return result;
	
	//Get the swapchain images
	uint32_t imageCount = 0;
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

	//Link some image viewes to the swapchain images
	swapChainImageViews.resize(imageCount);
	for (std::size_t i = 0; i < swapChainImageViews.size(); i++)
	{
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = swapChainImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = swapChainInfo.imageFormat;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
			BHD_THROW_WITH_LOG("failed to create image views!")
		}
	}

	return result;
}


void VulkanSwapChain::release() 
{
	for (auto & imageView : swapChainImageViews)
		vkDestroyImageView(device, imageView, nullptr);
	swapChainImageViews.clear();

	if (swapChain != VK_NULL_HANDLE)
	{
		vkDestroySwapchainKHR(device, swapChain, nullptr);
		swapChain = VK_NULL_HANDLE;
	}

	device = VK_NULL_HANDLE;
}





VkSwapchainCreateInfoKHR SwapChainFeatures::pickSwapChain(
	VkSurfaceKHR surface,
	VkExtent2D extent2D,
	VkSurfaceFormatKHR surfaceFormat,
	VkPresentModeKHR presentMode,
	const std::vector<uint32_t> & queueFamilyIndices
) const
{

	BHD_ASSERT_MSG(IsValid(), "Swap Chain Features aren't Valids");

	surfaceFormat = pickSurfaceFormat(surfaceFormat);
	presentMode = pickPresentMode(presentMode);
	extent2D = pickExtent2D(extent2D);

	uint32_t imageCount = surfaceCapabilities.minImageCount + 1;
	if (surfaceCapabilities.maxImageCount > 0 && imageCount > surfaceCapabilities.maxImageCount) {
		imageCount = surfaceCapabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent2D;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	if (queueFamilyIndices.size() > 1 && queueFamilyIndices[0] != queueFamilyIndices[1]) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = (uint32_t)queueFamilyIndices.size();
		createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; //
		createInfo.pQueueFamilyIndices = nullptr; //
	}

	createInfo.preTransform = surfaceCapabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	return createInfo;
}





bool SwapChainFeatures::IsAvailable(VkSurfaceFormatKHR surfaceFormat) const
{
	BHD_ASSERT(IsValid());
	if (surfaceFormats.size() == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED) {
		return true;
	}
	for (const auto& one : surfaceFormats) {
		if (one.format == surfaceFormat.format && one.colorSpace == surfaceFormat.format) {
			return true;
		}
	}
	return false;
}


bool SwapChainFeatures::IsAvailable(VkPresentModeKHR surfaceFormat) const
{
	BHD_ASSERT(IsValid());
	for (const auto& one : presentModes) {
		if (one == surfaceFormat) {
			return true;
		}
	}
	return false;
}

#ifdef _MINWINDEF_
#undef max
#undef min 
#endif
VkExtent2D	SwapChainFeatures::pickExtent2D(VkExtent2D extent2D) const {
	return{
		std::max(std::min(extent2D.width,	surfaceCapabilities.maxImageExtent.width),	surfaceCapabilities.minImageExtent.width),
		std::max(std::min(extent2D.height,	surfaceCapabilities.maxImageExtent.height),	surfaceCapabilities.minImageExtent.height)
	};
}

