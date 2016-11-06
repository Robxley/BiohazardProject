#ifndef _BHD_BIOVULKAN_H
#define _BHD_BIOVULKAN_H
#pragma once

#include <functional>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

#include "vulkan/vulkan.h"

#include "VulkanTools.hpp"
#include "VulkanInstance.hpp"
#include "VulkanSurface.hpp"
#include "VulkanDevice.hpp"

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

		//Call init function with extensions and layers required by GLFW
		VkResult init(const std::vector<std::string> & extensions, const std::vector<std::string> & layers = {});

	public:
		//GLFW functions
		//------------------------------------------------------------------------
		
		//Call init function with extensions and layers required by GLFW
		VkResult initWithGlfw(GLFWwindow * window);
		std::vector<std::string> getGlfwRequiredInstanceExtensions();


	private:

#ifdef _glfw3_h_
		GLFWwindow *glfwWindow = nullptr;
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
		//Surface
		VulkanSurface surface;

		//Device
		VulkanDevice device;

		//Swap Chain
		VulkanSwapChain swapChain;



	}; //class BioVulkan

}

#endif //_BHD_BIOVULKAN_H