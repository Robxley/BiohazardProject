#ifndef _BHD_BIOVULKAN_H
#define _BHD_BIOVULKAN_H
#pragma once

#include <functional>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

//#define GLFW_INCLUDE_VULKAN
#include "vulkan\vulkan.h"
#include <GLFW/glfw3.h>


namespace bhd
{
	class BioVulkan
	{
	public:
		//Main BioVulkan functions
		//-------------------------------------
		//------------------------------------

		BioVulkan() {}
		~BioVulkan() { release(); }

		void release()
		{
			if (instance != VK_NULL_HANDLE)
				vkDestroyInstance(instance, nullptr);
			instance = VK_NULL_HANDLE;
		}

		VkResult init();


	public:
		//Vulkan setting function
		//--------------------------------------
		//--------------------------------------

		//Get available instance extensions and layers.
		static std::vector<std::string> getAvailableExtensions();
		static std::vector<std::string> getAvailableLayers();

		//Log the results of getAvailableExtensions and getAvailableLayers
		static void info();

		//By default the layers are only used in debug

		//Extensions and layers to init with vulkan
		std::vector<std::string> extensions = getRequiredInstanceExtensions();
		std::vector<std::string> layers = { 
#ifdef _DEBUG
			"VK_LAYER_LUNARG_standard_validation" 
#endif
		};

		//To check if the given list of extensions or layers is available
		bool checkAvailability(const std::vector<std::string> & exts, const std::vector<std::string> & vulkanExts);

	private:

#ifdef _glfw3_h_
		//GLFW stuffs
		//------------------------------------------------------------------------
		std::vector<std::string> getGlfwRequiredInstanceExtensions();
		std::vector<std::string> getRequiredInstanceExtensions()
		{
			return std::move(getGlfwRequiredInstanceExtensions());
		}
#else
		std::vector<std::string> getRequiredInstanceExtensions()
		{
			return std::vector<std::string>();
		}
#endif


		//Vulkan stuffs
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------

		//Vulkan instance stuffs
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


		//debug
#ifdef _DEBUG
#endif

		
	}; //class BioVulkan

}

#endif //_BHD_BIOVULKAN_H