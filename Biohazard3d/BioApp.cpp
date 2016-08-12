#include "BioApp.h"


using namespace bhd;

BioApp::BioApp()
{
}


BioApp::~BioApp()
{
}

void BioApp::initWindow()
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window.ptr = glfwCreateWindow(window.width, window.height, window.name.c_str(), nullptr, nullptr);
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
	
}