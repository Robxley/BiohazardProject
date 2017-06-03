#include "BioApp.hpp"


using namespace bhd;

BioApp::BioApp()
{
}


BioApp::~BioApp()
{
	vulkan.release();

#ifdef _glfw3_h_
	glfwDestroyWindow(glfwWindow);
#endif
}

void BioApp::initWindow()
{
#ifdef _glfw3_h_
	glfwSetErrorCallback([](int code, const char *msg) {BHD_LOG_ERROR("Error id: "<<code<<" - "<<msg); });
	if (glfwInit() != GLFW_TRUE)
	{
		BHD_THROW_WITH_LOG("Failed on glfwInit");
	}
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	glfwWindow = glfwCreateWindow(window.width, window.height, window.name.c_str(), nullptr, nullptr);
	if (glfwWindow == nullptr){
		BHD_THROW_WITH_LOG("Failed on glfwCreateWindow");
	}
#endif
}

void BioApp::mainLoop()
{
#ifdef _glfw3_h_
	while (!glfwWindowShouldClose(glfwWindow))
	{
		glfwPollEvents();
	}
#endif
}

void BioApp::initVulKan()
{
#ifdef _glfw3_h_
	vulkan.initWithGlfw(glfwWindow);
#endif
}