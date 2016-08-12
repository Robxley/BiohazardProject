#pragma once

#include <functional>
#include <stdexcept>
#include <vector>
#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
//#include "vulkan\vulkan.h"

namespace bhd
{

	class BioVulkan
	{
	public:
		BioVulkan();
		~BioVulkan() { release(); }

		void release()
		{
			if (instance != VK_NULL_HANDLE)
				vkDestroyInstance(instance, nullptr);
			instance = VK_NULL_HANDLE;
		}

		//GLFW stuffs
		//------------------------------------------------------------------------

		bool stuffIsInit = { [] { if (glfwInit() != GLFW_TRUE) throw std::runtime_error("failed on glfwInit"); return true; }() };
		
		std::function<const char **(uint32_t*)> getInstanceExtensions = { [] (uint32_t * n)
			{
				const char ** exts = glfwGetRequiredInstanceExtensions(n);
				std::cout << "GLFW required instance extensions : " << std::endl;
				for (uint32_t i = 0; i < *n; i++)
					std::cout << "\t" << exts[i] << std::endl;
				return exts;
			}
		};

		//Vulkan stuffs
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------

		//All stuffs to create a vulkan instance
		//--------------------------------------

		//Vulkan Instance to create
		VkInstance instance = { VK_NULL_HANDLE };


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

		//Instance Create info struct
		VkInstanceCreateInfo instanceCreateInfo = {
			VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,								//sType
			nullptr,															//pNext
			0,																	//flags
			&appInfo,															//pApplicationInfo
			0,																	//enabledLayerCount
			nullptr,															//ppEnabledLayerNames
			0,																	//enabledExtensionCount
			getInstanceExtensions(&instanceCreateInfo.enabledExtensionCount)	//ppEnabledExtensionNames
		};

		//Result of the instance creation
		VkResult resultInstance = { [&] {
			if(vkCreateInstance(&instanceCreateInfo, nullptr, &instance)!=VK_SUCCESS)
				throw std::runtime_error("failed to create instance!");
			PrintExtension();
			return VK_SUCCESS; }() 
		};

		void PrintExtension()
		{
			uint32_t extensionCount = 0;
			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
			std::vector<VkExtensionProperties> extensions(extensionCount);
			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
			std::cout << "Vulkan available extensions:" << std::endl;

			for (const auto& extension : extensions) {
				std::cout << "\t" << extension.extensionName << std::endl;
			}
		}


	}; //class BioVulkan
}