#ifndef _BHD_VULKANSWAPCHAIN_H
#define _BHD_VULKANSWAPCHAIN_H
#pragma once


#include <vector>
#include <algorithm>
#include "vulkan/vulkan.h"
#include "VulkanSurface.hpp"
#include "VulkanFramebuffer.h"

namespace bhd
{
	class VulkanSwapChain
	{

	public:

		~VulkanSwapChain() {
			release();
		}

		//Create the swap chain object and link image view objects to the swap chain images
		VkResult create(const VkDevice & _device, const VkSwapchainCreateInfoKHR & _swapChainInfo);
		
		void release();

		operator VkSwapchainKHR() const { return swapChain; }
		operator VkSwapchainCreateInfoKHR() const { return swapChainInfo; }
		operator VkExtent2D () const { return swapChainInfo.imageExtent; }
		operator VkFormat () const { return swapChainInfo.imageFormat; }

		//operator const std::vector<VkImage>&() const { return swapChainImages; }
		//operator const std::vector<VkImageView>&() const { return swapChainImageViews; }

		const std::vector<VkImageView>& getImageViews() const {	return swapChainImageViews;	};
		
	protected:

		VkSwapchainKHR swapChain = VK_NULL_HANDLE;			//vulkan swap chain object
		VkSwapchainCreateInfoKHR swapChainInfo;				//swap chain info

		VkDevice device;									//linked device
		std::vector<VkImage> swapChainImages;				//list of the joined swapchain images
		std::vector<VkImageView> swapChainImageViews;		//Image views working on the swapchain images

	};

	//Useful class used to get intel on the swap chain
	class SwapChainFeatures
	{
	public:
		SwapChainFeatures() {}
		
		VkSurfaceCapabilitiesKHR  surfaceCapabilities;
		std::vector<VkSurfaceFormatKHR>		surfaceFormats;
		std::vector<VkPresentModeKHR>		presentModes;
		bool IsValid() const{
			return !surfaceFormats.empty() && !presentModes.empty();
		}

		//Check if a surface format is available
		bool IsAvailable(VkSurfaceFormatKHR surfaceFormat /*= { VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR }*/) const;
		
		//Check if a present mode is available
		bool IsAvailable(VkPresentModeKHR presentMode /*= VK_PRESENT_MODE_MAILBOX_KHR*/) const;

		//Check if a extent 2D is available
		bool IsAvailable(VkExtent2D extent2D /*= { 800,600 }*/) const {
			return	surfaceCapabilities.maxImageExtent.width >= extent2D.width && surfaceCapabilities.maxImageExtent.height >= extent2D.height
				&&	surfaceCapabilities.minImageExtent.width <= extent2D.width && surfaceCapabilities.minImageExtent.height <= extent2D.height;
		}
	
		//try to pick a specific surface format else return the first one
		VkSurfaceFormatKHR pickSurfaceFormat(VkSurfaceFormatKHR surfaceFormat = { VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR }) const {
			return IsAvailable(surfaceFormat) ? surfaceFormat : surfaceFormats[0];
		}
		//try to pick a specific present mode else return VK_PRESENT_MODE_FIFO_KHR (garanteed by the Vulkan API)
		VkPresentModeKHR pickPresentMode(VkPresentModeKHR presentMode = VK_PRESENT_MODE_MAILBOX_KHR) const {
			return IsAvailable(presentMode) ? presentMode : VK_PRESENT_MODE_FIFO_KHR/*presentModes[0]*/;
		}
	
		//try to pick a specific extent 2D (resolution) else return the closest
		VkExtent2D	pickExtent2D(VkExtent2D extent2D) const;


		VkSwapchainCreateInfoKHR pickSwapChain(
			VkSurfaceKHR surface,
			VkExtent2D extent2D,
			VkSurfaceFormatKHR surfaceFormat = { VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR },
			VkPresentModeKHR presentMode = VK_PRESENT_MODE_MAILBOX_KHR,
			const std::vector<uint32_t> & queueFamilyIndices = {}
		) const;
	
		VkSwapchainCreateInfoKHR pickSwapChain(const VulkanSurface & surface) const 
		{
			return pickSwapChain(surface.GetSurfaceKHR(), surface.GetExtent2d(),surface.GetSurfaceFormat(),surface.GetPresentMode());
		}

	};


}
#endif //_BHD_VULKANSWAPCHAIN_H