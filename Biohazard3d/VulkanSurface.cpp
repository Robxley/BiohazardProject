#include "VulkanSurface.hpp"

#include <GLFW/glfw3native.h>
using namespace bhd;

VkResult VulkanSurface::init(VkInstance _instance = VK_NULL_HANDLE)
{
	if (_instance != VK_NULL_HANDLE)
	{
		release();
		instance = _instance;
	}
	BHD_ASSERT_LOG(instance != VK_NULL_HANDLE, "Instance is VK_NULL_HANDLE");



	return VK_SUCCESS;
}


VkResult VulkanSurface::initWin32(HWND hwnd, HINSTANCE hinstance)
{

	VkWin32SurfaceCreateInfoKHR   createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hwnd = hwnd;
	createInfo.hinstance = hinstance;

	return VK_SUCCESS;
}