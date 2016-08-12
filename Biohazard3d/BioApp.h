#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "BioVulkan.h"

#include <iostream>
#include <stdexcept>
#include <functional>
#include <memory>


namespace bhd
{
	class BioWindow
	{
	public:
		int width = 800;
		int height = 600;
		std::string name = "biohazard";
		void* ptr = nullptr;  //GLFWwindow*
#ifdef _glfw3_h_
		GLFWwindow* GetWindow() { return (GLFWwindow*)ptr; }
#else
		void* GetWindow() { return (void*)ptr; }
#endif
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

	private:

		void initWindow();
		void initVulKan();
		void mainLoop();
		
	private:
		BioWindow window;
		BioVulkan vulkan;
	};
}

