

#ifndef _BHD_BIOAPP_H
#define _BHD_BIOAPP_H
#pragma once

#include <iostream>
#include <stdexcept>
#include <functional>
#include <memory>

#include <vulkan/vulkan.h>
#include "GLFW/glfw3.h"

#include "BioVulkan.hpp"

namespace bhd
{
	using BioWindow = struct SBioWindow
	{
		int width = 800;
		int height = 600;
		std::string name = "biohazard";
	};

	class BioApp
	{
	public:
		BioApp();
		~BioApp();

		void initApp()
		{
			initWindow();
			initVulKan();
		}

		void run()
		{
			initApp();
			mainLoop();
		}

		const BioWindow & GetBioWindow() { return window; }
		const BioVulkan & GetBioVulkan() { return vulkan; }
	private:

		void initWindow();
		void initVulKan();
		void mainLoop();
		
	private:
		BioWindow window;
		BioVulkan vulkan;


#ifdef _glfw3_h_
		GLFWwindow* glfwWindow = nullptr;
#endif

	};
}

#endif _BHD_BIOAPP_H