#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

//#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <functional>

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

	class BioVulkan
	{
	public:

	};


	class VulkanApp
	{
	public:
		VulkanApp();
		~VulkanApp();

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
	};
}

