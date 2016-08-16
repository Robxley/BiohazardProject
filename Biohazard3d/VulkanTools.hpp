
#ifndef _BHD_VULKANTOOLS_H
#define _BHD_VULKANTOOLS_H
#pragma once

#include "Assertion.hpp"
#include "Logger.hpp"
#include <vulkan\vulkan.h>

#include <vector>
#include <string>
#include <functional>

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


	//Vulkan basic object
	template<typename T, typename C = std::function<VkResult(T&)>, typename D = std::function<void(T)>>
	class VulkanObject
	{
	protected:
		T object = { VK_NULL_HANDLE };
		C create{ nullptr };
		D destroy{ nullptr };
	public:
		VulkanObject(){}
		~VulkanObject() { release(); }

		VkResult init()
		{
			return create(object);
		}

		void release()
		{
			if(object != VK_NULL_HANDLE)
				destroy(object);

			object = VK_NULL_HANDLE;
		}

		operator T()
		{
			return object;
		}

		//To check if the given list of extensions is include in the other
		static bool checkAvailability(const std::vector<std::string> & exts, const std::vector<std::string> & vulkanExts)
		{
			bool availability = true;
			for (const auto & ext : exts)
			{
				bool isHere = false;
				for (const auto & vext : vulkanExts)
				{
					if (std::string(ext).compare(std::string(vext)) == 0)
					{
						BHD_LOG(ext << ": OK");
						isHere = true;
						continue;
					}
				}
				if (!isHere)
				{
					BHD_LOG_WARNING(ext << ": Not available");
					availability = false;
				}
			}
			return availability;
		}
	};



//Debug vulkan tools
//----------------------------------------------

#ifdef _DEBUG
	class DebugVulkanLayer
	{

	public:
		DebugVulkanLayer(VkInstance _instance = VK_NULL_HANDLE): instance(_instance) {}
		~DebugVulkanLayer()
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

		void release()
		{
			if (debugReportCallback != VK_NULL_HANDLE)
			{
				auto vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
				if (vkDestroyDebugReportCallbackEXT != nullptr)
					vkDestroyDebugReportCallbackEXT(instance, debugReportCallback, nullptr);
				else
				{
					BHD_THROW_WITH_LOG("vkDestroyDebugReportCallbackEXT == nullptr")
				}
			}
			debugReportCallback = VK_NULL_HANDLE;
		}


		VkResult initDebugReportCallback(VkInstance _instance = VK_NULL_HANDLE)
		{
			release();
			if (_instance != VK_NULL_HANDLE) instance = _instance;
			BHD_ASSERT_LOG(_instance != VK_NULL_HANDLE, "Instance is VK_NULL_HANDLE");
			auto vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
			if (vkCreateDebugReportCallbackEXT != nullptr)
				return vkCreateDebugReportCallbackEXT(instance, &createInfo, nullptr, &debugReportCallback);
			else
				return VK_ERROR_EXTENSION_NOT_PRESENT;
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