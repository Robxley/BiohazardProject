
#ifndef _BHD_VULKANCONTEXT_H
#define _BHD_VULKANCONTEXT_H
#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "VulkanInstance.hpp"
#include "VulkanSurface.hpp"
#include "VulkanDevice.hpp"
#include "VulkanSwapChain.h"

namespace bhd
{
	//Class used to create a functional vulkan context
	//Use VulkanFactory classes to fill it
	class VulkanContext
	{
	public:
		VulkanContext() {}
		~VulkanContext() { release(); }
		void release();

		//Instance stuffs (filled by VulkanInstanceFactory)
		VkInstance instance = VK_NULL_HANDLE;					//Vulkan instance

		//Surface stuffs (filled by VulkanSurfaceFactory)
		VkSurfaceKHR surface = VK_NULL_HANDLE;					//Vulkan surface
		VkExtent2D extent2D = { 0,0 };							//related extent2d (generally windows size (w,h))
		VkSurfaceFormatKHR surfaceFormat = { VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR };
		VkPresentModeKHR presentMode = VK_PRESENT_MODE_MAILBOX_KHR;

		//Device stuffs (filled by VulkanDeviceFactory)
		VkDevice device = VK_NULL_HANDLE;						//Vulkan device (your gpu)
		std::vector<VkQueue> queues;							//associeted queue (generally graphic queues)

		//Swapchain stuffs (filled by VulkanSwapChainFactory)
		VkSwapchainCreateInfoKHR swapchainCreateInfo;			//Vulkan swapchain info used to create the vulkan swapchain
		VkSwapchainKHR swapChain = VK_NULL_HANDLE;				//Vulkan swapchain
		std::vector<VkImage> swapChainImages;					//related images
		std::vector<VkImageView> swapChainImageViews;			//Image views linked to swapchain images (same extend, same format ...)

	};


	//Virtual Vulkan Factory
	template <typename VKObject>
	class VulkanFactory : public VKObject
	{
	public:
		virtual VkResult fillContext(VulkanContext & vkContext) = 0;

		template <typename VKItem, VKItem resetValue = VK_NULL_HANDLE >
		VKItem move(VKItem & item, VKItem reset = resetValue) {
			VKItem tmp = item; 
			item = reset;
			return tmp;
		}

	};

	//Factory to fill a VulkanContext with a instance
	class VulkanInstanceFactory : public VulkanFactory<VulkanInstance>
	{
		public:
			VkResult fillContext(VulkanContext & vkContext);
	};

	//Factory to fill a VulkanContext with a surface
	class VulkanSurfaceFactory : public VulkanFactory<VulkanSurface>
	{
		public:
			VkResult fillContext(VulkanContext & vkContext);
	};

	//Factory to fill a VulkanContext with a logical device
	class VulkanDeviceFactory : public VulkanFactory<VulkanDevice>
	{
		public:
			VkResult fillContext(VulkanContext & vkContext);
	};

	//Factory to fill a VulkanContext with a swapchain
	class VulkanSwapChainFactory : public VulkanFactory<VulkanSwapChain>
	{
		public:
			VkResult fillContext(VulkanContext & vkContext);
	};

	//Generic operator to fill a vulkan context
	template <typename VFactory>
	VulkanContext& operator<<(VulkanContext &context, VFactory & vulkanFactory)
	{
		VkResult result = vulkanFactory.fillContext(context);
		BHD_ASSERT(result == VK_SUCCESS);
		return context;
	}

#ifdef _glfw3_h_
	//GLFW vulkan context exemple
	inline void GLFWVulkanContext(
		GLFWwindow * ptr,
		std::vector<std::string> extensions = { "VK_KHR_surface" , "VK_KHR_win32_surface", "VK_EXT_debug_report" },
		std::vector<std::string> layers = { "VK_LAYER_LUNARG_standard_validation" }
	)
	{
		VulkanContext context;
		VulkanInstanceFactory instanceFactory;
		VulkanSurfaceFactory surfaceFactory;
		VulkanDeviceFactory	deviceFactory;
		VulkanSwapChainFactory	swapChainFactory;

		instanceFactory.SetExtensions(extensions);
		instanceFactory.SetLayers(layers);
		surfaceFactory.SetGLFWwindow((GLFWwindow*)ptr);

		context << instanceFactory;
		context << surfaceFactory;
		context << deviceFactory;
		context << swapChainFactory;
	}
#endif

}

#endif

