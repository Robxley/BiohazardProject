


#include "Logger.hpp"

#include "BioApp.hpp"
#include "VulkanContext.h"
#include <iostream>

using namespace bhd;

int main() 
{
	BHD_LOG_INSTANCE(nullptr);

	//Method 1
	try {
		VulkanInstance::info();
		BioApp app;
		app.run();
	}
	catch (const std::runtime_error& e) {
		BHD_LOG_ERROR(e.what());
		return EXIT_FAILURE;
	}

	//Method 2
	try {
		glfwSetErrorCallback([](int code, const char *msg) {BHD_LOG_ERROR("Error id: " << code << " - " << msg); });
		if (glfwInit() != GLFW_TRUE)
		{
			BHD_THROW_WITH_LOG("Failed on glfwInit");
		}
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		auto glfwWindow = glfwCreateWindow(800, 600, "Test context", nullptr, nullptr);

		GLFWVulkanContext(glfwWindow);

		glfwDestroyWindow(glfwWindow);
	}
	catch (const std::runtime_error& e) {
		BHD_LOG_ERROR(e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;

}