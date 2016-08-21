
#ifndef _BHD_VULKANINSTANCE_H
#define _BHD_VULKANINSTANCE_H
#pragma once

#include "VulkanTools.hpp"

namespace bhd
{
	class VulkanInstance
	{
	public:
		//Extensions and layers to init with vulkan
		std::vector<std::string> extensions;
		std::vector<std::string> layers;

	public:
		VulkanInstance();
		~VulkanInstance() { release(); };

		//Get available instance extensions and layers.
		static std::vector<std::string> getAvailableExtensions();
		static std::vector<std::string> getAvailableLayers();

		//Log the results of getAvailableExtensions and getAvailableLayers
		static void info();

		//Check if all given extensions and layers are available
		VkResult validation();

		VkResult init();

		void SetApplicationInfo(VkApplicationInfo info) {
			appInfo = info;
		}
		void SetInstanceCreateInfo(VkInstanceCreateInfo info) {
			instanceCreateInfo = info;
		}
		
		void release();

		operator VkInstance() {
			return instance;
		}

	private:

		//Application Info struct
		VkApplicationInfo appInfo = {
			VK_STRUCTURE_TYPE_APPLICATION_INFO,		//sType
			nullptr,								//pNext
			"BioHazard Basic App",					//pApplicationName
			VK_MAKE_VERSION(1, 0, 0),				//applicationVersion
			"BioHazard Engine",						//pEngineName
			VK_MAKE_VERSION(1,0,0),					//engineVersion
			VK_API_VERSION_1_0						//apiVersion
		};

		VkInstanceCreateInfo instanceCreateInfo = {
			VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,				//sType
			nullptr,											//pNext
			0,													//flags
			&appInfo,											//pApplicationInfo
			0,													//enabledLayerCount
			nullptr,											//ppEnabledLayerNames
			0,													//enabledExtensionCount
			nullptr												//ppEnabledExtensionNames
		};

		VkInstance instance = VK_NULL_HANDLE;
	};

}

#endif

