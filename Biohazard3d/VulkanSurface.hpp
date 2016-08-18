#ifndef _BHD_VULKANSURFACE_H
#define _BHD_VULKANSURFACE_H
#pragma once

#include "VulkanTools.hpp"

#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW\glfw3.h"
#include <GLFW\glfw3native.h>

namespace bhd
{
	class VulkanSurface
	{
	public:
		VulkanSurface(VkInstance _instance = VK_NULL_HANDLE) : instance(_instance) {};
		~VulkanSurface() { release(); }

		VkResult init(VkInstance instance);

		void release() {
			vkDestroySurfaceKHR(instance, surface, nullptr);
		}
	private:
		VkSurfaceKHR surface;
		VkInstance instance;

	private:
		//platform 
#ifdef _WIN32
		VkResult initWin32(HWND hwnd, HINSTANCE hinstance = GetModuleHandle(nullptr));
		VkResult initWin32Glfw(GLFWwindow * win32) {
			initWin32(glfwGetWin32Window(win32));
		}
#else
		#warning "No implemented VK_KHR_surface"
#endif
	};
}//namespace bhd
#endif //VulkanSurface