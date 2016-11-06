#ifndef _BHD_VULKANSURFACE_H
#define _BHD_VULKANSURFACE_H
#pragma once

#include <vulkan\vulkan.h>
#include "GLFW\glfw3.h"

#include "VulkanTools.hpp"

namespace bhd
{
	class VulkanSurface
	{
	public:
		VulkanSurface(VkInstance _instance = VK_NULL_HANDLE) : instance(_instance), surface(VK_NULL_HANDLE){};
		~VulkanSurface() { release(); }

#ifdef _glfw3_h_
		VkResult initGlfw(VkInstance instance, GLFWwindow * window);
		VkResult initWin32Glfw(VkInstance instance, GLFWwindow * window);
#endif

#ifdef _WIN32
		VkResult initWin32(VkInstance instance, HWND hwnd, HINSTANCE hinstance = GetModuleHandle(nullptr));
#endif

		operator VkSurfaceKHR() {
			return surface;
		}

		const VkSurfaceKHR & GetSurfaceKHR() const { return surface; }
		const VkExtent2D & GetExtent2d() const { return extent2D; }
		const VkSurfaceFormatKHR & GetSurfaceFormat() const { return surfaceFormat; }
		const VkPresentModeKHR & GetPresentMode() const { return presentMode; }

		void release() {
			if(surface!=VK_NULL_HANDLE)
				vkDestroySurfaceKHR(instance, surface, nullptr);
			surface = VK_NULL_HANDLE;
		}
	private:
		VkInstance instance;
		VkSurfaceKHR surface;
		VkExtent2D extent2D = { 0,0 };
		VkSurfaceFormatKHR surfaceFormat = { VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR };
		VkPresentModeKHR presentMode = VK_PRESENT_MODE_MAILBOX_KHR;

	private:
		VkResult initInstance(VkInstance instance);
//platform 
#ifdef _WIN32
		VkResult initWin32(HWND hwnd, HINSTANCE hinstance = GetModuleHandle(nullptr));
#else
		#warning "No implemented VK_KHR_surface"
#endif
	};
}//namespace bhd
#endif //VulkanSurface