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
		//Main BioVulkan functions
		//-------------------------------------
		//------------------------------------

		BioVulkan() {}
		~BioVulkan() { 
			release();
		}

		void release();

		VkResult init(const std::vector<std::string> & extensions, const std::vector<std::string> & layers = {});

		VkResult initWithGlfw()
		{
#ifdef _DEBUG
			return init(getGlfwRequiredInstanceExtensions(), { "VK_LAYER_LUNARG_standard_validation" });
#else
			return init(getGlfwRequiredInstanceExtensions());
#endif
		}

	private:

#ifdef _glfw3_h_
		//GLFW stuffs
		//------------------------------------------------------------------------
		std::vector<std::string> getGlfwRequiredInstanceExtensions();
#endif

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