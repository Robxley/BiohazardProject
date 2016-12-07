
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

		//Instance stuffs
		VkInstance instance = VK_NULL_HANDLE;;

		//Surface stuffs
		VkSurfaceKHR surface = VK_NULL_HANDLE;
		VkExtent2D extent2D = { 0,0 };
		VkSurfaceFormatKHR surfaceFormat = { VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR };
		VkPresentModeKHR presentMode = VK_PRESENT_MODE_MAILBOX_KHR;

		//Device stuffs
		VkDevice device = VK_NULL_HANDLE;
		std::vector<VkQueue> queues;

		//Swapchain stuffs
		VkSwapchainCreateInfoKHR swapchainCreateInfo;
		VkSwapchainKHR swapChain = VK_NULL_HANDLE;
		std::vector<VkImage> swapChainImages;

	};


	//Virtual Vulkan Factory
	template <typename VKObject>
	class VulkanFactory : public VKObject
	{
	public:
		virtual VkResult fillContext(VulkanContext & vkContext) = 0;

		template <typename VKItem, VKItem resetValue = VK_NULL_HANDLE >
		VKItem Move(VKItem & item, VKItem reset = resetValue) {
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

	inline void TestContext(
		void * ptr, 
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
}

#endif

