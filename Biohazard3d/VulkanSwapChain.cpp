#include "VulkanSwapChain.h"
#include "Assertion.hpp"


using namespace bhd;


VkResult VulkanSwapChain::create(const VkDevice & _device, const VkSwapchainCreateInfoKHR & swapChainInfo) {
	device = _device;
	VkResult result = vkCreateSwapchainKHR(device, &swapChainInfo, nullptr, &swapChain);
	if (result != VK_SUCCESS) return result;
	uint32_t imageCount = 0;
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

}

void VulkanSwapChain::release() {
	if (swapChain != VK_NULL_HANDLE)
	{
		vkDestroySwapchainKHR(device, swapChain, nullptr);
		swapChain = VK_NULL_HANDLE;
	}
}





VkSwapchainCreateInfoKHR SwapChainFeatures::pickSwapChain(
	VkSurfaceKHR surface,
	VkExtent2D extent2D,
	VkSurfaceFormatKHR surfaceFormat,
	VkPresentModeKHR presentMode,
	const std::vector<uint32_t> & queueFamilyIndices
) const
{

	BHD_ASSERT_LOG(IsValid(), "Swap Chain Features aren't Valids");

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

