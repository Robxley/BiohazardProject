#include "vulkanapp.h"


using namespace bhd;

VulkanApp::VulkanApp()
{
}


VulkanApp::~VulkanApp()
{
}

void VulkanApp::initWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window.ptr = glfwCreateWindow(window.width, window.height, window.name.c_str(), nullptr, nullptr);
}

void VulkanApp::mainLoop() 
{
	while (!glfwWindowShouldClose(window.GetGLFWwindow())) 
	{
		glfwPollEvents();
	}
}

void VulkanApp::initVulKan()
{

}