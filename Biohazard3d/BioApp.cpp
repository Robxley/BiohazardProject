#include "BioApp.hpp"


using namespace bhd;

BioApp::BioApp()
{
}


BioApp::~BioApp()
{
}

void BioApp::initWindow()
{
	glfwSetErrorCallback([](int code, const char *msg) {BHD_LOG_ERROR("Error id: "<<code<<" - "<<msg); });
	if (glfwInit() != GLFW_TRUE)
	{
		BHD_THROW_WITH_LOG("Failed on glfwInit");
	}
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window.ptr = glfwCreateWindow(window.width, window.height, window.name.c_str(), nullptr, nullptr);
	if (window.ptr == nullptr)
	{
		BHD_THROW_WITH_LOG("Failed on glfwCreateWindow");
	}
}

void BioApp::mainLoop()
{
	while (!glfwWindowShouldClose(window.GetWindow()))
	{
		glfwPollEvents();
	}
}

void BioApp::initVulKan()
{
	vulkan.initWithGlfw();
}