
#ifndef _BHD_VULKANTOOLS_H
#define _BHD_VULKANTOOLS_H
#pragma once

#include "Assertion.hpp"
#include "Logger.hpp"
#include <vulkan\vulkan.h>

#include <vector>
#include <string>


namespace bhd
{

	//Useful struct to get a valid const char ** pointer from a string vector
	class PPVulkanMaker
	{
	public:
		PPVulkanMaker(const std::vector<std::string> & names) :strNames(names) {}
		const char** data() {
			if (strNames.empty()) return nullptr;
			ccNames.clear();
			ccNames.reserve(strNames.size());
			for (const auto & name : strNames) ccNames.push_back(name.c_str());
			return ccNames.data();
		}
		operator const char**() {
			return data();
		}
		uint32_t count() { return (uint32_t)strNames.size(); }
	private:
		std::vector<const char*> ccNames;
		const std::vector<std::string> & strNames;
	};


	inline std::vector<const char*> getRequiredExtensions(bool enableValidationLayers)
	{
		std::vector<const char*> extensions;

		unsigned int glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		for (unsigned int i = 0; i < glfwExtensionCount; i++) {
			extensions.push_back(glfwExtensions[i]);
		}

		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		}

		return extensions;
	}


//Debug vulkan tools
//----------------------------------------------

#ifdef _DEBUG
	class CDebugVulkanLayer
	{

	public:

		CDebugVulkanLayer(VkInstance _instance): instance(_instance) {}
		~CDebugVulkanLayer()
		{
			release();
		}

		VkDebugReportCallbackCreateInfoEXT createInfo = {
			VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,			//sType;
			nullptr,															//pNext;
			VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT,	//flags;
			(PFN_vkDebugReportCallbackEXT)debugCallback,						//pfnCallback;
			nullptr																//userdata
		};

		VkResult initDebugReportCallback(VkInstance _instance = VK_NULL_HANDLE)
		{
			release();
			if (_instance != VK_NULL_HANDLE) instance = _instance;
			return vkCreateDebugReportCallbackEXT(instance, &createInfo, nullptr, &debugReportCallback);
		}

	private:
		static VkBool32 debugCallback(
			VkDebugReportFlagsEXT flags,
			VkDebugReportObjectTypeEXT objType,
			uint64_t obj,
			size_t location,
			int32_t code,
			const char* layerPrefix,
			const char* msg,
			void* userData)
		{
			BHD_LOG_ERROR("Bug layer: " << msg);
			return VK_FALSE;
		}

		void release()
		{
			if (debugReportCallback != VK_NULL_HANDLE)
				vkDestroyDebugReportCallbackEXT(instance, debugReportCallback, nullptr);

			debugReportCallback = VK_NULL_HANDLE;
		}

		VkDebugReportCallbackEXT debugReportCallback = { VK_NULL_HANDLE };
		VkInstance instance = { VK_NULL_HANDLE };

	};

#endif


/*
#ifdef defined(__ANDROID__) || defined(__IPHONEOS__) || defined(__linux__ )
#define VK_USE_PLATFORM_XCB_KHR
#define VK_USE_PLATFORM_XLIB_KHR
#elif defined(_WIN32)
#include <Windows.h>
#define VK_USE_PLATFORM_WIN32_KHR
#endif


	bool LoadVulkanLibrary()
	{

#if defined(VK_USE_PLATFORM_WIN32_KHR)
		auto VulkanLibrary = LoadLibrary(L"vulkan-1.dll");
#elif defined(VK_USE_PLATFORM_XCB_KHR) || defined(VK_USE_PLATFORM_XLIB_KHR)
		auto VulkanLibrary = dlopen("libvulkan.so", RTLD_NOW);
#else
		auto VulkanLibrary = nullptr;
#endif
			if (VulkanLibrary == nullptr) {
				BHD_LOG_ERROR( "Could not load Vulkan library!" << std::endl);
				return false;
			}
		return true;
	}
*/
}
#endif //_BHD_VULKANTOOLS_H