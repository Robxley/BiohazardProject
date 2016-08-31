#ifndef _BHD_VULKANSWAPCHAIN_H
#define _BHD_VULKANSWAPCHAIN_H
#pragma once


#include <vector>
#include <algorithm>
#include "vulkan/vulkan.h"

namespace bhd
{
	typedef struct SwapChainFeatures
	{
		VkSurfaceCapabilitiesKHR  surfaceCapabilities;
		std::vector<VkSurfaceFormatKHR>		surfaceFormats;
		std::vector<VkPresentModeKHR>		presentModes;
		bool IsValid(){
			return !surfaceFormats.empty() && !presentModes.empty();
		}

		//Check if a surface format is available
		bool IsAvailable(VkSurfaceFormatKHR surfaceFormat /*= { VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR }*/);
		
		//Check if a present mode is available
		bool IsAvailable(VkPresentModeKHR presentMode /*= VK_PRESENT_MODE_MAILBOX_KHR*/);

		//Check if a extent 2D is available
		bool IsAvailable(VkExtent2D extent2D /*= { 800,600 }*/) { 
			return	surfaceCapabilities.maxImageExtent.width >= extent2D.width && surfaceCapabilities.maxImageExtent.height >= extent2D.height
				&&	surfaceCapabilities.minImageExtent.width <= extent2D.width && surfaceCapabilities.minImageExtent.height <= extent2D.height;
		}
	
		//try to pick a specific surface format else return the first one
		VkSurfaceFormatKHR pickSurfaceFormat(VkSurfaceFormatKHR surfaceFormat = { VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR }) {
			return IsAvailable(surfaceFormat) ? surfaceFormat : surfaceFormats[0];
		}
		//try to pick a specific present mode else return VK_PRESENT_MODE_FIFO_KHR (garanteed by the Vulkan API)
		VkPresentModeKHR pickPresentMode(VkPresentModeKHR presentMode = VK_PRESENT_MODE_MAILBOX_KHR) {
			return IsAvailable(presentMode) ? presentMode : VK_PRESENT_MODE_FIFO_KHR/*presentModes[0]*/;
		}
	
		//try to pick a specific extent 2D (resolution) else return the closest
		VkExtent2D	pickExtent2D(VkExtent2D extent2D);


		VkSwapchainCreateInfoKHR pickSwapChain(
			VkSurfaceKHR surface = VK_NULL_HANDLE,
			VkExtent2D extent2D = { 800,600 },
			VkSurfaceFormatKHR surfaceFormat = { VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR },
			VkPresentModeKHR presentMode = VK_PRESENT_MODE_MAILBOX_KHR,
			const std::vector<uint32_t> & queueFamilyIndices = {}
		);
	

	}SwapChainFeatures;

}
#endif //_BHD_VULKANSWAPCHAIN_H