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

		//Init a Surface from a instance. 
		//If glfw window is used, in first SetGLFWwindow have to be called in. 
		//If win32 is used, in first SetWin32Window have to be called. 
		VkResult init(VkInstance _instance = VK_NULL_HANDLE);	

#ifdef _glfw3_h_
		void SetGLFWwindow(GLFWwindow * window) { windowPtr = window; }
		VkResult initGlfw(VkInstance instance, GLFWwindow * window);
#ifdef _WIN32
		VkResult initWin32Glfw(VkInstance instance, GLFWwindow * window); //Call initWin32
#endif
#endif

#ifdef _WIN32
		void SetWin32Window(HWND hwnd, HINSTANCE hinstance = GetModuleHandle(nullptr));
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
	protected:
		VkInstance instance;
		VkSurfaceKHR surface;
		VkExtent2D extent2D = { 0,0 };
		VkSurfaceFormatKHR surfaceFormat = { VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR };
		VkPresentModeKHR presentMode = VK_PRESENT_MODE_MAILBOX_KHR;

		void * windowPtr = nullptr;

	protected:
		void initInstance(VkInstance instance);

//platform 
#ifdef _WIN32
		VkResult initWin32(HWND hwnd, HINSTANCE hinstance = GetModuleHandle(nullptr));
		typedef struct Win32Window
		{
			HWND hwnd;
			HINSTANCE hinstance;
		}Win32Window;

		Win32Window win32Window;
#else
		#warning "No implemented VK_KHR_surface"
#endif
	};
}//namespace bhd
#endif //VulkanSurface