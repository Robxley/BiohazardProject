
#ifndef _BHD_VULKANTOOLS_H
#define _BHD_VULKANTOOLS_H
#pragma once

#include <vector>
#include <string>
#include <functional>

#include <vulkan/vulkan.h>

#include "Assertion.hpp"
#include "Logger.hpp"

namespace bhd
{
	inline std::string vkResultToString(VkResult result)
	{
		std::string strResult;
		switch (result)
		{
		case VK_SUCCESS: return{ "VK_SUCCESS" };
		case VK_NOT_READY: return{ "VK_NOT_READY" };
		case VK_TIMEOUT: return{ "VK_TIMEOUT" };
		case VK_EVENT_SET: return{ "VK_EVENT_SET" };
		case VK_EVENT_RESET: return{ "VK_EVENT_RESET" };
		case VK_INCOMPLETE: return{ "VK_INCOMPLETE" };
		case VK_ERROR_OUT_OF_HOST_MEMORY: return{ "VK_ERROR_OUT_OF_HOST_MEMORY" };
		case VK_ERROR_OUT_OF_DEVICE_MEMORY: return{ "VK_ERROR_OUT_OF_DEVICE_MEMORY" };
		case VK_ERROR_INITIALIZATION_FAILED: return{ "VK_ERROR_INITIALIZATION_FAILED" };
		case VK_ERROR_DEVICE_LOST: return{ "VK_ERROR_DEVICE_LOST" };
		case VK_ERROR_MEMORY_MAP_FAILED:return{ "VK_ERROR_MEMORY_MAP_FAILED" };
		case VK_ERROR_LAYER_NOT_PRESENT: return{ "VK_ERROR_LAYER_NOT_PRESENT" };
		case VK_ERROR_EXTENSION_NOT_PRESENT: return{ "VK_ERROR_EXTENSION_NOT_PRESENT" };
		case VK_ERROR_FEATURE_NOT_PRESENT: return{ "VK_ERROR_FEATURE_NOT_PRESENT" };
		case VK_ERROR_INCOMPATIBLE_DRIVER: return{ "VK_ERROR_INCOMPATIBLE_DRIVER" };
		case VK_ERROR_TOO_MANY_OBJECTS: return{ "VK_ERROR_TOO_MANY_OBJECTS" };
		case VK_ERROR_FORMAT_NOT_SUPPORTED:return{ "VK_ERROR_FORMAT_NOT_SUPPORTED" };
		case VK_ERROR_SURFACE_LOST_KHR: return{ "VK_ERROR_SURFACE_LOST_KHR" };
		case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return{ "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR" };
		case VK_SUBOPTIMAL_KHR:return{ "VK_SUBOPTIMAL_KHR" };
		case VK_ERROR_OUT_OF_DATE_KHR: return{ "VK_ERROR_OUT_OF_DATE_KHR" };
		case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return{ "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR" };
		case VK_ERROR_VALIDATION_FAILED_EXT: return{ "VK_ERROR_VALIDATION_FAILED_EXT" };
		case VK_ERROR_INVALID_SHADER_NV: return{ "VK_ERROR_INVALID_SHADER_NV" };
		case VK_RESULT_RANGE_SIZE: return{ "VK_RESULT_RANGE_SIZE" };
		case VK_RESULT_MAX_ENUM: return{ "VK_RESULT_MAX_ENUM" };
		}

		return{"UNKNOW RESULT"};
	}

	//Useful struct to get a valid const char ** pointer from a string vector
	class VulkanPPMaker
	{
	public:
		VulkanPPMaker(const std::vector<std::string> & names) : strNames(&names) {}
		VulkanPPMaker(const std::vector<std::string> * names) : strNames(names) {}
		
		const char** data() {
			if (strNames == nullptr) return nullptr;
			if (strNames->empty()) return nullptr;
			ccNames.clear();
			ccNames.reserve(strNames->size());
			for (const auto & name : *strNames) ccNames.push_back(name.c_str());
			return ccNames.data();
		}
		operator const char**() {
			return data();
		}
		uint32_t count() { return (strNames == nullptr)? 0 :(uint32_t)strNames->size(); }
	private:
		std::vector<const char*> ccNames;
		const std::vector<std::string> * strNames;
	};

	template <typename T>
	class Printer
	{
	public:
		void info(const char * title) const
		{
			const auto & list = static_cast<const T&>(*this);
			BHD_LOG_LIST(title, list);
		}
	};

	class InstanceExtensionNames : public std::vector<std::string>, public Printer<InstanceExtensionNames> {};
	class InstanceLayerNames : public std::vector<std::string>, public Printer<InstanceLayerNames> {};
	typedef std::vector<VkQueueFamilyProperties> QueueFamilyProperties;		//for one device
	typedef std::vector<VkExtensionProperties> ExtensionProperties;			//for one device
	class ExtensionNames : public std::vector<std::string>, public Printer<ExtensionNames> {
	public:
		ExtensionNames() {}
		ExtensionNames(const std::vector<std::string> & extensionNames) :std::vector<std::string>(extensionNames) {}
		ExtensionNames(const char*  extensionName) : std::vector<std::string>({ extensionName }) {}
	};				
	typedef std::vector<VkQueue>	DeviceQueues;
	
	typedef struct BioVulkanContext
	{
		//Instance stuffs
		VkInstance				instance = VK_NULL_HANDLE;
		InstanceExtensionNames	instanceExtensionNames;
		InstanceLayerNames		instanceLayerNames;

		//Surface stuff
		VkSurfaceKHR			surface = VK_NULL_HANDLE;

		//Device stuff
		VkDevice				device = VK_NULL_HANDLE;
		ExtensionNames			extensionNames;
		QueueFamilyProperties	queueFamilyProperties;
		DeviceQueues			deviceQueues;

		//Some operators
		operator VkInstance() const { return instance; }
		operator const InstanceExtensionNames&() const { return instanceExtensionNames; }
		operator const InstanceLayerNames&() const { return instanceLayerNames; }

		operator VkSurfaceKHR() const { return surface; }

		operator VkDevice() const { return device; }
		operator const ExtensionNames&() const{ return extensionNames; };
		operator const QueueFamilyProperties&() const { return queueFamilyProperties; };
		operator DeviceQueues() const { return deviceQueues; };

		void release()
		{
			//Container
			deviceQueues.clear();
			queueFamilyProperties.clear();
			extensionNames.clear();
			instanceLayerNames.clear();
			instanceExtensionNames.clear();

			//Vulkan objects (The order is important.)
			if (device != VK_NULL_HANDLE) {
				vkDestroyDevice(device, nullptr);
				device = VK_NULL_HANDLE;
			}
			if (surface != VK_NULL_HANDLE) {
				vkDestroySurfaceKHR(instance, surface, nullptr);
				surface = VK_NULL_HANDLE;
			}
			if (instance != VK_NULL_HANDLE) {
				vkDestroyInstance(instance, nullptr);
				instance = VK_NULL_HANDLE;
			}
		}

	} BioVulkanContext;
	
		
	//To check if the given list of extensions is include in the other
	static bool checkAvailability(const std::vector<std::string> & exts, const std::vector<std::string> & vulkanExts, int verbose = 2)
	{
		bool availability = true;
		for (const auto & ext : exts)
		{
			bool isHere = false;
			for (const auto & vext : vulkanExts)
			{
				if (std::string(ext).compare(std::string(vext)) == 0)
				{
					if(verbose>0) BHD_LOG(ext << ": OK");
					isHere = true;
					continue;
				}
			}
			if (!isHere)
			{
				if (verbose>1) BHD_LOG_WARNING(ext << ": Not available");
				availability = false;
			}
		}
		return availability;
	}

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
			std::cerr << msg << std::endl;
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
}  //namespace
#endif //_BHD_VULKANTOOLS_H