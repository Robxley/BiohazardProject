#ifndef _BHD_BIOVULKAN_H
#define _BHD_BIOVULKAN_H
#pragma once

#include "Assertion.hpp"

#include <functional>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <string>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
//#include "vulkan\vulkan.h"

namespace bhd
{

	class BioVulkan
	{
	public:
		BioVulkan() {}
		~BioVulkan() { release(); }

		void release()
		{
			if (instance != VK_NULL_HANDLE)
				vkDestroyInstance(instance, nullptr);
			instance = VK_NULL_HANDLE;
		}

		VkResult init();
		static void info();

		std::vector<const char*> extensions = getExtensions();
		std::vector<const char*> layers;


		bool checkSupport(const std::vector<const char*> & exts, const std::vector<const char*> & vulkanExts);


		//GLFW stuffs
		//------------------------------------------------------------------------
		static std::vector<const char*> getExtensions()
		{
				uint32_t n = 0;
				std::vector<const char*> exts;
				if (glfwInit() != GLFW_TRUE) { BHD_THROW_WITH_LOG("Failed on glfwInit"); }
				const char ** glwfexts = glfwGetRequiredInstanceExtensions(&n);
				exts.reserve(n+1);
				BHD_LOG("GLFW required instance extensions : ");
				for (uint32_t i = 0; i < n; i++) 
				{ 
					exts.push_back(glwfexts[i]);
					BHD_LOG("\t" << glwfexts[i]);
				}

				BHD_LOG("Debug extension : ");
				BHD_LOG("\t" << VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
				exts.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
				return exts;
		}
	

		//Vulkan stuffs
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------

		//All stuffs about vulkan instance
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
			VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,			//sType
			nullptr,										//pNext
			0,												//flags
			&appInfo,										//pApplicationInfo
			(uint32_t)layers.size(),						//enabledLayerCount
			layers.empty() ? nullptr : &layers[0],			//ppEnabledLayerNames
			(uint32_t)extensions.size(),					//enabledExtensionCount
			extensions.empty() ? nullptr : &extensions[0]	//ppEnabledExtensionNames
		};

		static std::vector<const char*>&& checkInstanceExtensions();


		//All stuffs about vulkan layouts
		//--------------------------------------

#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif


		static std::vector<const char*>&& checkValidationLayers();


	}; //class BioVulkan
}

#endif //_BHD_BIOVULKAN_H