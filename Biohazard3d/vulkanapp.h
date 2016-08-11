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
		GLFWwindow* GetGLFWwindow() { return (GLFWwindow*)ptr; }
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

