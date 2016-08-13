
#include "Logger.hpp"

#include "BioApp.hpp"
#include <iostream>

using namespace bhd;

int main() 
{
	BioVulkan::info();
	BioApp app;

	try {
		app.run();
	}
	catch (const std::runtime_error& e) {
		BHD_LOG_ERROR(e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;

	return 0;
}