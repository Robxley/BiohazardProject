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

#include "VulkanTools.hpp"
#include "VulkanInstance.hpp"

namespace bhd
{
	class BioVulkan
	{
	public:
		//Main functions
		//-------------------------------------

		BioVulkan() {}
		~BioVulkan() { 
			release();
		}

		void release();

		VkResult init(const std::vector<std::string> & extensions, const std::vector<std::string> & layers = {});

		//Call init function with extensions and layers required by GLFW


	public:
		//GLFW functions
		//------------------------------------------------------------------------
		
		//Call init function with extensions and layers required by GLFW
		VkResult initWithGlfw();		
		std::vector<std::string> getGlfwRequiredInstanceExtensions();


	private:
		//Vulkan stuffs
		//------------------------------------------------------------------------
		//------------------------------------------------------------------------

		//Vulkan Instanc
		VulkanInstance instance;

		//debug stuffs
#ifdef _DEBUG
		DebugVulkanLayer debugVulkanLayer;
#endif

	}; //class BioVulkan

}

#endif //_BHD_BIOVULKAN_H