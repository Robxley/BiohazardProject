#include "VulkanSurface.hpp"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

using namespace bhd;

void VulkanSurface::initInstance(VkInstance _instance = VK_NULL_HANDLE)
{
	if (_instance != VK_NULL_HANDLE)
	{
		release();
		instance = _instance;
	}
	BHD_ASSERT(instance != VK_NULL_HANDLE);
}

#ifdef _WIN32
VkResult VulkanSurface::initWin32(HWND hwnd, HINSTANCE hinstance)
{
	BHD_ASSERT_MSG(hwnd,"Hwnd is null");
	VkWin32SurfaceCreateInfoKHR   createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hwnd = hwnd;
	createInfo.hinstance = hinstance;

	auto vkCreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(instance, "vkCreateWin32SurfaceKHR");
	if(vkCreateWin32SurfaceKHR == nullptr)
		BHD_THROW_WITH_LOG("Can't Get Instance Proc Addr vkCreateWin32SurfaceKHR");
	
	VkResult result = vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface);
	
	if (result != VK_SUCCESS)
	{
		BHD_LOG_ERROR("Can't create a surface with vkCreateWin32SurfaceKHR");
	}
	
	return result;
}

VkResult VulkanSurface::initWin32(VkInstance instance, HWND hwnd, HINSTANCE hinstance)
{
	initInstance(instance);
	return initWin32(hwnd, hinstance);
}

void VulkanSurface::SetWin32Window(HWND hwnd, HINSTANCE hinstance)
{
	win32Window.hwnd = hwnd;
	win32Window.hinstance = hinstance;
}

#endif

#ifdef _glfw3_h_
VkResult VulkanSurface::initGlfw(VkInstance instance, GLFWwindow * window)
{
	BHD_ASSERT(window != nullptr);
	initInstance(instance);
	VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &surface);
	if (result != VK_SUCCESS) {
		BHD_LOG_ERROR("GLFW failed to create window surface!");
	}
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	extent2D.width = w;
	extent2D.height = h;
	return result;
}

VkResult VulkanSurface::initWin32Glfw(VkInstance instance, GLFWwindow * window)
{
	initInstance(instance);
	return initWin32(glfwGetWin32Window(window));
}
#endif


VkResult VulkanSurface::init(VkInstance instance)
{
#ifdef _glfw3_h_
	return initGlfw(instance, (GLFWwindow*) windowPtr);
#endif

#ifdef _WIN32
	return initWin32(instance, win32Window.hwnd, win32Window.hinstance);
#endif
}